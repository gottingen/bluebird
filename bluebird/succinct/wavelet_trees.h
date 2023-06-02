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

/*! \file wavelet_trees.h
    \brief wavelet_trees.h contains wavelet tree implementations.
	\author Simon Gog
*/
#ifndef INCLUDED_SDSL_WAVELET_TREES
#define INCLUDED_SDSL_WAVELET_TREES

/** \defgroup wt Wavelet Trees (WT)
 *   This group contains data structures for wavelet trees. The following methods are supported:
 *    - []-operator
 *    - rank(i, c)
 *    - select(i, c)
 *    - inverse_select(i)
 */

#include "wt_pc.h"
#include "wt_blcd.h"
#include "wt_gmr.h"
#include "wt_huff.h"
#include "wt_hutu.h"
#include "wt_int.h"
#include "wm_int.h"
#include "wt_rlmn.h"
#include "wt_ap.h"
#include "construct.h"
#include "wt_algorithm.h"

namespace bluebird {

    template<class t_bitvector   = bit_vector,
            class t_rank        = typename t_bitvector::rank_1_type,
            class t_select      = typename t_bitvector::select_1_type,
            class t_select_zero = typename t_bitvector::select_0_type
    >
    using wt_hutu_int = wt_pc<hutu_shape,
            t_bitvector,
            t_rank,
            t_select,
            t_select_zero,
            int_tree<>>;

    template<class t_bitvector   = bit_vector,
            class t_rank        = typename t_bitvector::rank_1_type,
            class t_select      = typename t_bitvector::select_1_type,
            class t_select_zero = typename t_bitvector::select_0_type>
    using wt_huff_int = wt_pc<huff_shape,
            t_bitvector,
            t_rank,
            t_select,
            t_select_zero,
            int_tree<>>;

    template<class t_bitvector   = bit_vector,
            class t_rank        = typename t_bitvector::rank_1_type,
            class t_select_one  = typename t_bitvector::select_1_type,
            class t_select_zero = typename t_bitvector::select_0_type>
    using wt_blcd_int = wt_pc<balanced_shape,
            t_bitvector,
            t_rank,
            t_select_one,
            t_select_zero,
            int_tree<>>;
}

#endif
