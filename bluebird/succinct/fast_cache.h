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

#ifndef INCLUDED_SDSL_FAST_CACHE
#define INCLUDED_SDSL_FAST_CACHE

#include "int_vector.h"

namespace bluebird {

#define CACHE_SIZE 0x3FFULL

    struct fast_cache {
        typedef int_vector<>::size_type size_type;
        size_type m_table[2 * (CACHE_SIZE + 1)];

        // Constructor
        fast_cache() {
            for (size_type i = 0; i < (CACHE_SIZE + 1); ++i) {
                m_table[i << 1] = (size_type) -1;
            }
        }

        // Returns true if the request i is cached and
        // x is set to the answer of request i
        bool exists(size_type i, size_type &x) {
            if (m_table[(i & CACHE_SIZE) << 1] == i) {
                x = m_table[((i & CACHE_SIZE) << 1) + 1];
                return true;
            } else
                return false;
        }

        // Writes the answer for request i to the cache
        void write(size_type i, size_type x) {
            m_table[(i & CACHE_SIZE) << 1] = i;
            m_table[((i & CACHE_SIZE) << 1) + 1] = x;
        }
    };

} // end namespace bluebird

#endif
