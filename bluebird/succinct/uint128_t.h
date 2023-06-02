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

/*! \file uint128_t.h
   \brief uint128_t.h contains contains the definition of a 128-bit unsigned integer type.
   \author Simon Gog, Matthias Petri
*/
#ifndef INCLUDED_SDSL_UINT128
#define INCLUDED_SDSL_UINT128

#include <iostream>
#include "bits.h"

namespace bluebird {

#ifdef MODE_TI

    typedef unsigned int uint128_t __attribute__((mode(TI)));

#else

    class uint128_t {
    public:
        friend std::ostream &operator<<(std::ostream &, const uint128_t &);

    private:
        uint64_t m_lo;
        uint64_t m_high;

    public:
        inline uint128_t(uint64_t lo = 0, uint64_t high = 0) : m_lo(lo), m_high(high) {}

        inline uint128_t(const uint128_t &x) : m_lo(x.m_lo), m_high(x.m_high) {}

        inline uint128_t(uint128_t &&x) : m_lo(std::move(x.m_lo)), m_high(std::move(x.m_high)) {}

        uint128_t &operator=(const uint128_t &x) {
            m_lo = x.m_lo;
            m_high = x.m_high;
            return *this;
        }

        uint128_t &operator=(uint128_t &&x) {
            m_lo = std::move(x.m_lo);
            m_high = std::move(x.m_high);
            return *this;
        }

        inline uint8_t popcount() const {
            return (uint8_t) bits::cnt(m_lo) + (uint8_t) bits::cnt(m_high);
        }

        inline uint16_t hi() const {
            if (m_high == 0ULL) {
                return bits::hi(m_lo);
            } else {
                return bits::hi(m_high) + 64;
            }
        }

        inline uint16_t select(uint32_t i) const {
            uint16_t x = 0;
            if ((x = (uint16_t) bits::cnt(m_lo)) >= i) {
                return bits::sel(m_lo, i);
            }
            i -= x;
            return bits::sel(m_high, i) + 64;
        }

        inline uint128_t &operator+=(const uint128_t &x) {
            *this = *this + x;
            return *this;
        }

        inline uint128_t &operator+=(const uint64_t &x) {
            *this = *this + x;
            return *this;
        }

        inline uint128_t operator+(const uint128_t &x) const {
            return uint128_t(m_lo + x.m_lo, m_high + x.m_high + ((m_lo + x.m_lo) < m_lo));
        }

        inline uint128_t operator+(const uint64_t &x) const {
            return uint128_t(m_lo + x, m_high + ((m_lo + x) < m_lo));
        }

        inline uint128_t operator-(const uint128_t &x) const {
            return uint128_t(m_lo - x.m_lo, m_high - x.m_high - ((m_lo - x.m_lo) > m_lo));
        }

        inline uint128_t operator~() const {
            return uint128_t(~m_lo, ~m_high);
        }

        inline uint128_t &operator-=(const uint128_t &x) {
            *this = *this - x;
            return *this;
        }

        inline uint128_t operator|(const uint128_t &x) const {
            return uint128_t(m_lo | x.m_lo, m_high | x.m_high);
        }

        inline uint128_t operator|(const uint64_t &x) const {
            return uint128_t(m_lo | x, m_high);
        }

        inline uint128_t &operator|=(const uint128_t &x) {
            m_lo |= x.m_lo;
            m_high |= x.m_high;
            return *this;
        }

        inline uint128_t operator&(const uint128_t &x) const {
            return uint128_t(m_lo & x.m_lo, m_high & x.m_high);
        }

        /* // is not needed since we can convert uint128_t to uint64_t
        uint64_t operator&(uint64_t x){
        return m_lo & x;
        }
        */

        inline uint128_t operator<<(int x) const {
            if (x < 64) {
                auto high = (m_high << x) | (m_lo >> (64 - x));
                auto lo = m_lo << x;
                return uint128_t(lo, high);
            } else {
                auto high = m_lo << (x - 64);
                return uint128_t(0, high);
            }
        }

        inline uint128_t operator>>(int x) const {
            if (x < 64) {
                auto lo = (m_lo >> x) | (m_high << (64 - x));
                return uint128_t(lo, m_high >> x);
            } else {
                auto lo = m_high >> (x - 64);
                return uint128_t(lo, 0);
            }
        }

        inline uint128_t &operator=(const uint64_t &x) {
            m_high = 0;
            m_lo = x;
            return *this;
        }

        inline bool operator==(const uint128_t &x) const {
            return (m_lo == x.m_lo) and (m_high == x.m_high);
        }

        inline bool operator==(const uint64_t &x) const {
            return (m_lo == x) and (m_high == 0);
        }

        inline bool operator!=(const uint128_t &x) const {
            return !(*this == x);
        }

        inline bool operator>=(const uint128_t &x) const {
            if (m_high != x.m_high) {
                return m_high > x.m_high;
            } else {
                return m_lo >= x.m_lo;
            }
        }

        inline bool operator<=(const uint128_t &x) const {
            if (m_high != x.m_high) {
                return m_high < x.m_high;
            } else {
                return m_lo <= x.m_lo;
            }
        }

        inline bool operator>(const uint128_t &x) const {
            if (m_high != x.m_high) {
                return m_high > x.m_high;
            } else {
                return m_lo > x.m_lo;
            }
        }

        inline bool operator>(const uint64_t &x) const {
            if (m_high > 0) {
                return true;
            }
            return m_lo > x;
        }

        inline bool operator<(const uint128_t &x) const {
            if (m_high != x.m_high) {
                return m_high < x.m_high;
            } else {
                return m_lo < x.m_lo;
            }
        }

        inline operator uint64_t() const {
            return m_lo;
        }
    };

#endif

    std::ostream &operator<<(std::ostream &os, const uint128_t &x);

} // end namespace

#endif
