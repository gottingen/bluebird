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

/*! \file suffix_arrays.h
    \brief suffix_arrays.h contains generic classes for different suffix array classes.
	\author Simon Gog
*/
#ifndef INCLUDED_SDSL_SUFFIX_ARRAYS
#define INCLUDED_SDSL_SUFFIX_ARRAYS

#include "sdsl_concepts.h"

/** \defgroup csa Compressed Suffix Arrays (CSA) */

#include "csa_bitcompressed.h"
#include "csa_wt.h"
#include "csa_sada.h"
#include "wavelet_trees.h"
#include "construct.h"
#include "suffix_array_algorithm.h"

namespace bluebird {

//! Typedef for convenient usage of std integer alphabet strategy
    template<class t_wt               = wt_int<>,
            uint32_t t_dens = 32,
            uint32_t t_inv_dens = 64,
            class t_sa_sample_strat  = sa_order_sa_sampling<>,
            class t_isa_sample_strat = isa_sampling<>
    >
    using csa_wt_int = csa_wt<t_wt, t_dens, t_inv_dens, t_sa_sample_strat, t_isa_sample_strat, int_alphabet<>>;

    template<class t_enc_vec          = enc_vector<>,          // Vector type used to store the Psi-function
            uint32_t t_dens = 32,                    // Sample density for suffix array (SA) values
            uint32_t t_inv_dens = 64,                    // Sample density for inverse suffix array (ISA) values
            class t_sa_sample_strat  = sa_order_sa_sampling<>,// Policy class for the SA sampling. Alternative text_order_sa_sampling.
            class t_isa_sample_strat = isa_sampling<>         // Policy class for the ISA sampling.
    >
    using csa_sada_int = csa_sada<t_enc_vec, t_dens, t_inv_dens, t_sa_sample_strat, t_isa_sample_strat, int_alphabet<>>;

}

#endif
