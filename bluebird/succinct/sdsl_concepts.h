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

/*! \file sdsl_concepts.h
    \brief Contains declarations and definitions of data structure concepts.
	\author Simon Gog
*/
#ifndef INCLUDED_SDSL_CONCEPTS
#define INCLUDED_SDSL_CONCEPTS

#include "uintx_t.h" // for uint8_t

namespace bluebird
{

struct bv_tag {}; // bitvector tag
struct iv_tag {}; // int_vector tag

struct csa_tag {}; // compressed suffix array (CSAs) tag
struct cst_tag {}; // compressed suffix tree (CST) tag
struct wt_tag {};  // wavelet tree tag

struct psi_tag {}; // tag for CSAs based on the psi function
struct lf_tag {}; // tag for CSAs based on the LF function

struct csa_member_tag {}; // tag for text, bwt, LF, \Psi members of CSA

struct lcp_tag {};
struct lcp_plain_tag {};
struct lcp_permuted_tag {};
struct lcp_tree_compressed_tag {};
struct lcp_tree_and_lf_compressed_tag {};

struct alphabet_tag {};
struct byte_alphabet_tag { static const uint8_t WIDTH=8; };
struct int_alphabet_tag { static const uint8_t WIDTH=0; };

struct sa_sampling_tag {};
struct isa_sampling_tag {};


template<class t_T, class t_r = void>
struct enable_if_type {
    typedef t_r type;
};

template<class t_idx, class t_enable = void>
struct index_tag {
    typedef t_enable type;
};

template<class t_idx>
struct index_tag<t_idx, typename enable_if_type<typename t_idx::index_category>::type> {
    using type = typename t_idx::index_category;
};

template<class t_sampling, class t_enable = void>
struct sampling_tag {
    typedef t_enable type;
};

template<class t_sampling>
struct sampling_tag<t_sampling, typename enable_if_type<typename t_sampling::sampling_category>::type> {
    using type = typename t_sampling::sampling_category;
};

template<class t_enc_vec, class t_enable = void>
struct is_enc_vec {
    static const bool value = false;
};

template<class t_enc_vec>
struct is_enc_vec<t_enc_vec, typename enable_if_type<typename t_enc_vec::enc_vec_type>::type> {
    static const bool value = true;
};

template<class t_alphabet, class t_enable = void>
struct is_alphabet {
    static const bool value = false;
};

template<class t_alphabet>
struct is_alphabet<t_alphabet, typename enable_if_type<typename t_alphabet::alphabet_category>::type> {
    static const bool value = true;
};

} // end namespace bluebird

#endif
