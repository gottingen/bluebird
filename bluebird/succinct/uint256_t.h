// Copyright 2023 The titan-search Authors.
// Copyright (C) 2008 Simon Gog
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

/*! \file uint256_t.h
   \brief uint256_t.h contains a class for 256-bit unsigned integers.
   \author Simon Gog
*/
#ifndef INCLUDED_SDSL_UINT256
#define INCLUDED_SDSL_UINT256

#include <iostream>
#include "bits.h"
#include "uint128_t.h"

namespace bluebird {

    class uint256_t {
    public:
        friend std::ostream &operator<<(std::ostream &, const uint256_t &);

    private:
        uint64_t m_lo;
        uint64_t m_mid;
        uint128_t m_high;

    public:
        inline uint256_t(uint64_t lo = 0, uint64_t mid = 0, uint128_t high = 0) : m_lo(lo),
                                                                                  m_mid(mid), m_high(high) {}

        inline uint256_t(const uint256_t &x) : m_lo(x.m_lo), m_mid(x.m_mid),
                                               m_high(x.m_high) {}

        inline uint256_t(uint256_t &&x) : m_lo(std::move(x.m_lo)),
                                          m_mid(std::move(x.m_mid)), m_high(std::move(x.m_high)) {}

        uint256_t &operator=(const uint256_t &x) {
            m_lo = x.m_lo;
            m_mid = x.m_mid;
            m_high = x.m_high;
            return *this;
        }

        uint256_t &operator=(uint256_t &&x) {
            m_lo = std::move(x.m_lo);
            m_mid = std::move(x.m_mid);
            m_high = std::move(x.m_high);
            return *this;
        }

        inline uint16_t popcount() {
            return ((uint16_t) bits::cnt(m_lo)) + (uint16_t) bits::cnt(m_mid)
                   + (uint16_t) bits::cnt(m_high >> 64) + (uint16_t) bits::cnt(m_high);
        }

        inline uint16_t hi() {
            if (m_high == (uint128_t) 0ULL) {
                if (m_mid) {
                    return bits::hi(m_mid) + 64;
                } else {
                    return bits::hi(m_lo);
                }
            } else {
                uint64_t hh = (m_high >> 64);
                if (hh) {
                    return bits::hi(hh) + 192;
                } else {
                    return bits::hi(m_high) + 128;
                }
            }
        }

        inline uint16_t select(uint32_t i) {
            uint16_t x = 0;
            if ((x = (uint16_t) bits::cnt(m_lo)) >= i) {
                return bits::sel(m_lo, i);
            }
            i -= x;
            if ((x = (uint16_t) bits::cnt(m_mid)) >= i) {
                return bits::sel(m_mid, i) + 64;
            }
            i -= x;
            uint64_t hh = m_high >> 64;
            uint64_t lh = m_high;
            if ((x = (uint16_t) bits::cnt(lh)) >= i) {
                return bits::sel(lh, i) + 128;
            }
            i -= x;
            return bits::sel(hh, i) + 192;
        }

        inline uint256_t &operator+=(const uint256_t &x) {
            uint128_t lo = (uint128_t) m_lo + x.m_lo;
            uint128_t mid = (uint128_t) m_mid + x.m_mid + (lo >> 64);
            m_lo = lo;
            m_mid = mid;
            m_high += x.m_high + (mid >> 64);
            return *this;
//			return uint256_t(lo, mid, m_high + x.m_high + (mid >> 64));
        }

        inline uint256_t operator+(const uint256_t &x) {
            uint128_t lo = ((uint128_t) m_lo) + x.m_lo;
            uint128_t mid = (uint128_t) m_mid + x.m_mid + (lo >> 64);
            return uint256_t(lo, mid, m_high + x.m_high + (mid >> 64));
        }

        inline uint256_t operator-(const uint256_t &x) {
//			add two's complement of x
            uint128_t lo = (uint128_t) m_lo + (~x.m_lo) + (uint128_t) 1ULL;
            uint128_t mid = (uint128_t) m_mid + (~x.m_mid) + (lo >> 64);
            return uint256_t(lo, mid, m_high + (~x.m_high) + (mid >> 64));
        }

        inline uint256_t &operator-=(const uint256_t &x) {
//			add two's complement of x
            uint128_t lo = (uint128_t) m_lo + (~x.m_lo) + (uint128_t) 1ULL;
            uint128_t mid = (uint128_t) m_mid + (~x.m_mid) + (lo >> 64);
            m_lo = lo;
            m_mid = mid;
            m_high += (~x.m_high) + (mid >> 64);
            return *this;
        }


        inline uint256_t operator|(const uint256_t &x) {
            return uint256_t(m_lo | x.m_lo, m_mid | x.m_mid, m_high | x.m_high);
        }

        inline uint256_t &operator|=(const uint256_t &x) {
            m_lo |= x.m_lo;
            m_mid |= x.m_mid;
            m_high |= x.m_high;
            return *this;
        }

        inline uint256_t operator&(const uint256_t &x) {
            return uint256_t(m_lo & x.m_lo, m_mid & x.m_mid, m_high & x.m_high);
        }

        /* // is not needed since we can convert uint256_t to uint64_t
                uint64_t operator&(uint64_t x){
                    return m_lo & x;
                }
        */

        inline uint256_t operator<<(int x) const {
            if (x < 128) {
                uint128_t high = m_high << x;
                uint128_t low = (((uint128_t) m_mid << 64) | m_lo);
                high |= (low >> (128 - x));
                low = low << x;
                return uint256_t(low, low >> 64, high);
            } else { // x >= 128
                uint128_t high = (((uint128_t) m_mid << 64) | m_lo) << (x - 128); // TODO: check x==128
                return uint256_t(0, 0, high);
            }
        }

        inline uint256_t operator>>(int x) const {
            if (x < 128) {
                uint128_t low = (((uint128_t) m_mid << 64) | m_lo) >> x;
                low |= ((m_high << (127 - x)) << 1);
                return uint256_t(low, low >> 64, m_high >> x);
            } else { // x >= 128
                uint128_t low = (m_high >> (x - 128)); // TODO: check x=128
                return uint256_t(low, low >> 64, 0);
            }
        }

        inline uint256_t &operator=(const uint64_t &x) {
            m_high = 0;
            m_mid = 0;
            m_lo = x;
            return *this;
        }

        inline bool operator==(const uint256_t &x) const {
            return (m_lo == x.m_lo) and (m_mid == x.m_mid) and (m_high == x.m_high);
        }

        inline bool operator!=(const uint256_t &x) const {
            return !(*this == x);
        }

        inline bool operator>=(const uint256_t &x) const {
            if (m_high != x.m_high) {
                return m_high > x.m_high;
            }
            if (m_mid != x.m_mid) {
                return m_mid > x.m_mid;
            } else {
                return m_lo >= x.m_lo;
            }
        }

        inline bool operator<=(const uint256_t &x) const {
            if (m_high != x.m_high) {
                return m_high < x.m_high;
            }
            if (m_mid != x.m_mid) {
                return m_mid < x.m_mid;
            } else {
                return m_lo <= x.m_lo;
            }
        }

        inline bool operator>(const uint256_t &x) const {
            if (m_high != x.m_high) {
                return m_high > x.m_high;
            }
            if (m_mid != x.m_mid) {
                return m_mid > x.m_mid;
            } else {
                return m_lo > x.m_lo;
            }
        }

        inline bool operator>(const uint64_t &x) const {
            if (m_high > (uint128_t) 0ULL or m_mid > (uint128_t) 0ULL) {
                return true;
            }
            return m_lo > x;
        }

        inline bool operator<(const uint256_t &x) const {
            if (m_high != x.m_high) {
                return m_high < x.m_high;
            }
            if (m_mid != x.m_mid) {
                return m_mid < x.m_mid;
            } else {
                return m_lo < x.m_lo;
            }
        }

        inline operator uint64_t() {
            return m_lo;
        }
    };

    std::ostream &operator<<(std::ostream &os, const uint256_t &x);

} // end namespace

#endif
