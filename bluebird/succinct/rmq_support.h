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

/*! \file rmq_support.h
    \brief rmq_support.h contains different range minimum support data structures.
	\author Simon Gog
*/
#ifndef INCLUDED_SDSL_RMQ_SUPPORT
#define INCLUDED_SDSL_RMQ_SUPPORT

/** \defgroup rmq_group Range Minimum/Maximum Support (RMS) */

template<class RandomAccessContainer, bool Minimum>     // for range minimum queries
struct min_max_trait {
    static inline bool strict_compare(const typename RandomAccessContainer::value_type v1,
                                      const typename RandomAccessContainer::value_type v2) {
        return v1 < v2;
    }

    static inline bool
    compare(const typename RandomAccessContainer::value_type v1, const typename RandomAccessContainer::value_type v2) {
        return v1 <= v2;
    }
};

template<class RandomAccessContainer> // for range maximum queries
struct min_max_trait<RandomAccessContainer, false> {
    static inline bool strict_compare(const typename RandomAccessContainer::value_type v1,
                                      const typename RandomAccessContainer::value_type v2) {
        return v1 > v2;
    }

    static inline bool
    compare(const typename RandomAccessContainer::value_type v1, const typename RandomAccessContainer::value_type v2) {
        return v1 >= v2;
    }
};

#include "rmq_support_sparse_table.h"
#include "rmq_succinct_sct.h"
#include "rmq_succinct_sada.h"

#endif
