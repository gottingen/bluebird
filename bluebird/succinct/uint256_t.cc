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

#include "bluebird/succinct/uint256_t.h"

//! Namespace for the succinct data structure library
namespace bluebird {
    std::ostream &operator<<(std::ostream &os, const uint256_t &x) {
        uint64_t X[4] = {(uint64_t) (x.m_high >> 64), (uint64_t) x.m_high, x.m_mid, x.m_lo};
        for (int j = 0; j < 4; ++j) {
            for (int i = 0; i < 16; ++i) {
                os << std::hex << ((X[j] >> 60) & 0xFULL) << std::dec;
                X[j] <<= 4;
            }
        }
        return os;
    }
} // end namespace
