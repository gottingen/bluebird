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

/*! \file construct_lcp.h
    \brief construct_lcp.h contains a space and time efficient construction method for lcp arrays
	\author Simon Gog, Timo Beller
*/
#ifndef INCLUDED_SDSL_CONSTRUCT_LCP
#define INCLUDED_SDSL_CONSTRUCT_LCP

#include "config.h"
#include "int_vector.h"
#include "sfstream.h"
#include "rank_support.h"
#include "select_support.h"
#include "util.h"
#include "construct_isa.h"
#include "construct_bwt.h"
#include "wt_huff.h"
#include "wt_algorithm.h"
#include "construct_lcp_helper.h"

#include <iostream>
#include <stdexcept>
#include <algorithm>

//#define STUDY_INFORMATIONS

namespace bluebird {

//! Construct the LCP array for text over byte- or integer-alphabet.
/*!	The algorithm computes the lcp array and stores it to disk.
 *  \tparam t_width Width of the text. 0==integer alphabet, 8=byte alphabet.
 *  \param config	Reference to cache configuration
 *  \pre Text and Suffix array exist in the cache. Keys:
 *         * conf::KEY_TEXT for t_width=8  or conf::KEY_TEXT_INT for t_width=0
 *         * conf::KEY_SA
 *  \post LCP array exist in the cache. Key
 *         * conf::KEY_LCP
 *  \par Time complexity
 *         \f$ \Order{n} \f$
 *  \par Space complexity
 *         \f$ n (\log \sigma + \log n) \f$ bits
 *  \par Reference
 *         Toru Kasai, Gunho Lee, Hiroki Arimura, Setsuo Arikawa, Kunsoo Park:
 *         Linear-Time Longest-Common-Prefix Computation in Suffix Arrays and Its Applications.
 *         CPM 2001: 181-192
 */
    template<uint8_t t_width>
    void construct_lcp_kasai(cache_config &config) {
        static_assert(t_width == 0 or t_width == 8,
                      "construct_lcp_kasai: width must be `0` for integer alphabet and `8` for byte alphabet");
        int_vector<> lcp;
        typedef int_vector<>::size_type size_type;
        construct_isa(config);
        {
            int_vector<t_width> text;
            if (!load_from_cache(text, key_text_trait<t_width>::KEY_TEXT, config)) {
                return;
            }
            int_vector_buffer<> isa_buf(cache_file_name(conf::KEY_ISA, config), std::ios::in,
                                        1000000); // init isa file_buffer
            int_vector<> sa;
            if (!load_from_cache(sa, conf::KEY_SA, config)) {
                return;
            }
            // use Kasai algorithm to compute the lcp values
            for (size_type i = 0, j = 0, sa_1 = 0, l = 0, n = isa_buf.size(); i < n; ++i) {
                sa_1 = isa_buf[i]; // = isa[i]
                if (sa_1) {
                    j = sa[sa_1 - 1];
                    if (l) --l;
                    assert(i != j);
                    while (text[i + l] == text[j +
                                               l]) { // i+l < n and j+l < n are not necessary, since text[n]=0 and text[i]!=0 (i<n) and i!=j
                        ++l;
                    }
                    sa[sa_1 - 1] = l; //overwrite sa array with lcp values
                } else {
                    l = 0;
                    sa[n - 1] = 0;
                }
            }

            for (size_type i = sa.size(); i > 1; --i) {
                sa[i - 1] = sa[i - 2];
            }
            sa[0] = 0;
            lcp.swap(sa);
        }
        store_to_cache(lcp, conf::KEY_LCP, config);
    }


//! Construct the LCP array for text over byte- or integer-alphabet.
/*!	The algorithm computes the lcp array and stores it to disk.
 *  \pre Text and Suffix array exist in the cache. Keys:
 *         * conf::KEY_TEXT for t_width=8  or conf::KEY_TEXT_INT for t_width=0
 *         * conf::KEY_SA
 *  \post LCP array exist in the cache. Key
 *         * conf::KEY_LCP
 *  \par Time complexity
 *         \f$ \Order{n} \f$
 *  \par Space complexity
 *         \f$ n( \log \sigma + \log n ) \f$ bits
 *  \par Reference
 *         Juha Kärkkäinen, Giovanni Manzini, Simon J. Puglisi:
 *         Permuted Longest-Common-Prefix Array.
 *         CPM 2009: 181-192
 */
    template<uint8_t t_width>
    void construct_lcp_PHI(cache_config &config) {
        static_assert(t_width == 0 or t_width == 8,
                      "construct_lcp_PHI: width must be `0` for integer alphabet and `8` for byte alphabet");
        typedef int_vector<>::size_type size_type;
        typedef int_vector<t_width> text_type;
        const char *KEY_TEXT = key_text_trait<t_width>::KEY_TEXT;
        int_vector_buffer<> sa_buf(cache_file_name(conf::KEY_SA, config));
        size_type n = sa_buf.size();

        assert(n > 0);
        if (1 == n) {  // Handle special case: Input only the sentinel character.
            int_vector<> lcp(1, 0);
            store_to_cache(lcp, conf::KEY_LCP, config);
            return;
        }

//	(1) Calculate PHI (stored in array plcp)
        int_vector<> plcp(n, 0, sa_buf.width());
        for (size_type i = 0, sai_1 = 0; i < n; ++i) {
            size_type sai = sa_buf[i];
            plcp[sai] = sai_1;
            sai_1 = sai;
        }

//  (2) Load text from disk
        text_type text;
        load_from_cache(text, KEY_TEXT, config);

//  (3) Calculate permuted LCP array (text order), called PLCP
        size_type max_l = 0;
        for (size_type i = 0, l = 0; i < n - 1; ++i) {
            size_type phii = plcp[i];
            while (text[i + l] == text[phii + l]) {
                ++l;
            }
            plcp[i] = l;
            if (l) {
                max_l = std::max(max_l, l);
                --l;
            }
        }
        util::clear(text);
        uint8_t lcp_width = bits::hi(max_l) + 1;

//	(4) Transform PLCP into LCP
        std::string lcp_file = cache_file_name(conf::KEY_LCP, config);
        size_type buffer_size = 1000000; // buffer_size is a multiple of 8!
        int_vector_buffer<> lcp_buf(lcp_file, std::ios::out, buffer_size, lcp_width);   // open buffer for lcp
        lcp_buf[0] = 0;
        sa_buf.buffersize(buffer_size);
        for (size_type i = 1; i < n; ++i) {
            size_type sai = sa_buf[i];
            lcp_buf[i] = plcp[sai];
        }
        lcp_buf.close();
        register_cache_file(conf::KEY_LCP, config);
    }


//! Construct the LCP array (only for byte strings)
/*!	The algorithm computes the lcp array and stores it to disk.
 *  \param config	Reference to cache configuration
 *  \pre Text and Suffix array exist in the cache. Keys:
 *         * conf::KEY_TEXT
 *         * conf::KEY_SA
 *  \post LCP array exist in the cache. Key
 *         * conf::KEY_LCP
 *  \par Time complexity
 *         \f$ \Order{n*q} \f$ implmented with \f$ q=64 \f$
 *  \par Space complexity
 *         \f$ n + \frac{n*\log{n}}{q} \f$ bytes, implmented with \f$ q=64 \f$
 *  \par Reference
 *         Juha Kärkkäinen, Giovanni Manzini, Simon J. Puglisi:
 *         Permuted Longest-Common-Prefix Array.
 *         CPM 2009: 181-192
 */
    void construct_lcp_semi_extern_PHI(cache_config &config);


//! Construct the LCP array (only for byte strings)
/*!	The algorithm computes the lcp array and stores it to disk.
 *  Our new 2 phases lcp algorithm
 *  \param config	Reference to cache configuration
 *  \pre Text, Suffix array and BWT exist in the cache. Keys:
 *         * conf::KEY_TEXT
 *         * conf::KEY_SA
 *         * conf::KEY_BWT
 *  \post LCP array exist in the cache. Key
 *         * conf::KEY_LCP
 *  \par Time complexity
 *         \f$ \Order{n^2} \f$, but usually faster than goPHI
 *  \par Space complexity
 *         Usually \f$ 2n \f$ bytes, worst case \f$5n bytes\f$
 *  \par Reference
 *         Simon Gog, Enno Ohlebusch:
 *         Fast and Lightweight LCP-Array Construction Algorithms.
 *         ALENEX 2011: 25-34
 */
    void construct_lcp_go(cache_config &config);


//! Construct the LCP array (only for byte strings)
/*!	The algorithm computes the lcp array and stores it to disk.
 *  Our new 2 phases lcp algorithm
 *  \param config	Reference to cache configuration
 *  \pre Text, Suffix array and BWT exist in the cache. Keys:
 *         * conf::KEY_TEXT
 *         * conf::KEY_SA
 *         * conf::KEY_BWT
 *  \post LCP array exist in the cache. Key
 *         * conf::KEY_LCP
 *  \par Time complexity
 *         \f$ \Order{n} \f$
 *  \par Space complexity
 *         Usually \f$ 2n \f$ bytes
 *  \par Reference
 *         Simon Gog, Enno Ohlebusch:
 *         Lightweight LCP-Array Construction in Linear Time.
 *         CoRR abs/1012.4263 (2010)
 */
    void construct_lcp_goPHI(cache_config &config);


//! Construct the LCP array out of the BWT (only for byte strings)
/*!	The algorithm computes the lcp array and stores it to disk. It needs only the Burrows and Wheeler transform.
 *  \param config	Reference to cache configuration
 *  \pre BWT exist in the cache. Keys:
 *         * conf::KEY_BWT
 *  \post LCP array exist in the cache. Key
 *         * conf::KEY_LCP
 *  \par Time complexity
 *         \f$ \Order{n \log{\sigma}} \f$
 *  \par Space complexity
 *         Usually not more than \f$ 2.5n \f$ bytes
 *  \par Reference
 *         Timo Beller, Simon Gog, Enno Ohlebusch, Thomas Schnattinger:
 *         Computing the Longest Common Prefix Array Based on the Burrows-Wheeler Transform.
 *         SPIRE 2011: 197-208
 */
    void construct_lcp_bwt_based(cache_config &config);


//! Construct the LCP array out of the BWT (only for byte strings)
/*!	The algorithm computes the lcp array and stores it to disk. It needs only the Burrows and Wheeler transform.
 *  \param config	Reference to cache configuration
 *  \pre BWT exist in the cache. Keys:
 *         * conf::KEY_BWT
 *  \post LCP array exist in the cache. Key
 *         * conf::KEY_LCP
 *  \par Time complexity
 *         \f$ \Order{n \log{\sigma}} \f$
 *  \par Space complexity
 *         Usually not more than \f$ 1.5n \f$ bytes
 *  \par Reference
 *         Timo Beller, Simon Gog, Enno Ohlebusch, Thomas Schnattinger:
 *         Computing the longest common prefix array based on the Burrows-Wheeler transform.
 *         J. Discrete Algorithms 18: 22-31 (2013)
 */
    void construct_lcp_bwt_based2(cache_config &config);

}// end namespace

#endif
