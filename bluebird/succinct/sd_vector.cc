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

#include "bluebird/succinct/sd_vector.h"

//! Namespace for the succinct data structure library
namespace bluebird {

    sd_vector_builder::sd_vector_builder() :
            m_size(0), m_capacity(0),
            m_wl(0),
            m_tail(0), m_items(0),
            m_last_high(0), m_highpos(0) {
    }

    sd_vector_builder::sd_vector_builder(size_type n, size_type m) :
            m_size(n), m_capacity(m),
            m_wl(0),
            m_tail(0), m_items(0),
            m_last_high(0), m_highpos(0) {
        if (m_capacity > m_size) {
            throw std::runtime_error("sd_vector_builder: requested capacity is larger than vector size.");
        }

        size_type logm = bits::hi(m_capacity) + 1, logn = bits::hi(m_size) + 1;
        if (logm == logn) {
            logm--; // to ensure logn-logm > 0
        }
        m_wl = logn - logm;
        m_low = int_vector<>(m_capacity, 0, m_wl);
        m_high = bit_vector(m_capacity + (1ULL << logm), 0);
    }

    void
    sd_vector_builder::swap(sd_vector_builder &sdb) {
        std::swap(m_size, sdb.m_size);
        std::swap(m_capacity, sdb.m_capacity);
        std::swap(m_wl, sdb.m_wl);
        std::swap(m_tail, sdb.m_tail);
        std::swap(m_items, sdb.m_items);
        std::swap(m_last_high, sdb.m_last_high);
        std::swap(m_highpos, sdb.m_highpos);
        m_low.swap(sdb.m_low);
        m_high.swap(sdb.m_high);
    }

    template<>
    sd_vector<>::sd_vector(sd_vector_builder &builder) {
        if (builder.items() != builder.capacity()) {
            throw std::runtime_error("sd_vector: the builder is not full.");
        }

        m_size = builder.m_size;
        m_wl = builder.m_wl;
        m_low.swap(builder.m_low);
        m_high.swap(builder.m_high);
        util::init_support(m_high_1_select, &m_high);
        util::init_support(m_high_0_select, &m_high);

        builder = sd_vector_builder();
    }

} // end namespace
