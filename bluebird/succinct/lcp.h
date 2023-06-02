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

/*! \file lcp.hpp
    \brief lcp.hpp contains classes for lcp information.
    \author Simon Gog
*/
#ifndef INCLUDED_SDSL_LCP
#define INCLUDED_SDSL_LCP

#include "bluebird/succinct/sdsl_concepts.h"

#include "bluebird/succinct/int_vector.h"
#include "bluebird/succinct/csa_alphabet_strategy.h"// for key_trait
#include "bluebird/succinct/select_support_mcl.h"
#include "bluebird/succinct/construct_isa.h"
#include <istream>

//! Namespace for the succinct data structure library.
namespace bluebird {

// construct lcp arrays
    template<class t_lcp, class t_cst>
    void construct_lcp(t_lcp &lcp, const t_cst &cst, cache_config &config) {
        typename t_lcp::lcp_category tag;
        construct_lcp(lcp, cst, config, tag);
    }

    template<class t_lcp, class t_cst>
    void construct_lcp(t_lcp &lcp, const t_cst &, cache_config &config, lcp_plain_tag) {
        t_lcp tmp_lcp(config);
        lcp.swap(tmp_lcp);
    }

    template<class t_lcp, class t_cst>
    void construct_lcp(t_lcp &lcp, const t_cst &cst, cache_config &config, lcp_permuted_tag) {
        t_lcp tmp_lcp(config, &(cst.csa));
        lcp.swap(tmp_lcp);
    }

    template<class t_lcp, class t_cst>
    void construct_lcp(t_lcp &lcp, const t_cst &cst, cache_config &config, lcp_tree_compressed_tag) {
        t_lcp tmp_lcp(config, &cst);
        lcp.swap(tmp_lcp);
    }

    template<class t_lcp, class t_cst>
    void construct_lcp(t_lcp &lcp, const t_cst &cst, cache_config &config, lcp_tree_and_lf_compressed_tag) {
        t_lcp tmp_lcp(config, &cst);
        lcp.swap(tmp_lcp);
    }

// copy lcp arrays
    template<class t_lcp, class t_cst>
    void copy_lcp(t_lcp &lcp, const t_lcp &lcp_c, const t_cst &cst) {
        typename t_lcp::lcp_category tag;
        copy_lcp(lcp, lcp_c, cst, tag);
    }

    template<class t_lcp, class t_cst>
    void copy_lcp(t_lcp &lcp, const t_lcp &lcp_c, const t_cst &, lcp_plain_tag) {
        lcp = lcp_c;
    }

    template<class t_lcp, class t_cst>
    void copy_lcp(t_lcp &lcp, const t_lcp &lcp_c, const t_cst &cst, lcp_permuted_tag) {
        lcp = lcp_c;
        lcp.set_csa(&(cst.csa));
    }

    template<class t_lcp, class t_cst>
    void copy_lcp(t_lcp &lcp, const t_lcp &lcp_c, const t_cst &cst, lcp_tree_compressed_tag) {
        lcp = lcp_c;
        lcp.set_cst(&cst);
    }

    template<class t_lcp, class t_cst>
    void copy_lcp(t_lcp &lcp, const t_lcp &lcp_c, const t_cst &cst, lcp_tree_and_lf_compressed_tag) {
        lcp = lcp_c;
        lcp.set_cst(&cst);
    }

// move lcp arrays
    template<class t_lcp, class t_cst>
    void move_lcp(t_lcp &lcp, t_lcp &lcp_c, const t_cst &cst) {
        typename t_lcp::lcp_category tag;
        move_lcp(lcp, lcp_c, cst, tag);
    }

    template<class t_lcp, class t_cst>
    void move_lcp(t_lcp &lcp, t_lcp &lcp_c, const t_cst &, lcp_plain_tag) {
        lcp = std::move(lcp_c);
    }

    template<class t_lcp, class t_cst>
    void move_lcp(t_lcp &lcp, t_lcp &lcp_c, const t_cst &cst, lcp_permuted_tag) {
        lcp = std::move(lcp_c);
        lcp.set_csa(&(cst.csa));
    }

    template<class t_lcp, class t_cst>
    void move_lcp(t_lcp &lcp, t_lcp &lcp_c, const t_cst &cst, lcp_tree_compressed_tag) {
        lcp = std::move(lcp_c);
        lcp.set_cst(&cst);
    }

    template<class t_lcp, class t_cst>
    void move_lcp(t_lcp &lcp, t_lcp &lcp_c, const t_cst &cst, lcp_tree_and_lf_compressed_tag) {
        lcp = std::move(lcp_c);
        lcp.set_cst(&cst);
    }


// swap lcp arrays
    template<class t_lcp, class t_cst>
    void swap_lcp(t_lcp &lcp1, t_lcp &lcp2, const t_cst &cst1, const t_cst &cst2) {
        typename t_lcp::lcp_category tag;
        swap_lcp(lcp1, lcp2, cst1, cst2, tag);
    }

    template<class t_lcp, class t_cst>
    void swap_lcp(t_lcp &lcp1, t_lcp &lcp2, const t_cst &, const t_cst &, lcp_plain_tag) {
        lcp1.swap(lcp2);
    }

    template<class t_lcp, class t_cst>
    void swap_lcp(t_lcp &lcp1, t_lcp &lcp2, const t_cst &cst1, const t_cst &cst2, lcp_permuted_tag) {
        lcp1.swap(lcp2);
        lcp1.set_csa(&(cst1.csa));
        lcp2.set_csa(&(cst2.csa));
    }

    template<class t_lcp, class t_cst>
    void swap_lcp(t_lcp &lcp1, t_lcp &lcp2, const t_cst &cst1, const t_cst &cst2, lcp_tree_compressed_tag) {
        lcp1.swap(lcp2);
        lcp1.set_cst(&cst1);
        lcp2.set_cst(&cst2);
    }

    template<class t_lcp, class t_cst>
    void swap_lcp(t_lcp &lcp1, t_lcp &lcp2, const t_cst &cst1, const t_cst &cst2, lcp_tree_and_lf_compressed_tag) {
        lcp1.swap(lcp2);
        lcp1.set_cst(&cst1);
        lcp2.set_cst(&cst2);
    }

// load lcp arrays
    template<class t_lcp, class t_cst>
    void load_lcp(t_lcp &lcp, std::istream &in, const t_cst &cst) {
        typename t_lcp::lcp_category tag;
        load_lcp(lcp, in, cst, tag);
    }

    template<class t_lcp, class t_cst>
    void load_lcp(t_lcp &lcp, std::istream &in, const t_cst &, lcp_plain_tag) {
        lcp.load(in);
    }

    template<class t_lcp, class t_cst>
    void load_lcp(t_lcp &lcp, std::istream &in, const t_cst &cst, lcp_permuted_tag) {
        lcp.load(in, &(cst.csa));
    }

    template<class t_lcp, class t_cst>
    void load_lcp(t_lcp &lcp, std::istream &in, const t_cst &cst, lcp_tree_compressed_tag) {
        lcp.load(in, &cst);
    }

    template<class t_lcp, class t_cst>
    void load_lcp(t_lcp &lcp, std::istream &in, const t_cst &cst, lcp_tree_and_lf_compressed_tag) {
        lcp.load(in, &cst);
    }

} // end namespace bluebird

#include "lcp_support_sada.h"     // type (b)
#include "lcp_byte.h"             // type (a)
#include "lcp_wt.h"               // type (a)
#include "lcp_vlc.h"              // type (a)
#include "lcp_dac.h"              // type (a)
#include "lcp_bitcompressed.h"    // type (a)
#include "lcp_support_tree.h"     // type (c)
#include "lcp_support_tree2.h"    // type (c)


#endif
