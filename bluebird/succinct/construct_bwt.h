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

/*! \file construct_bwt.h
    \brief construct_bwt.h contains a space and time efficient construction method for the Burrows and Wheeler Transform (BWT).
    \author Simon Gog
*/
#ifndef INCLUDED_SDSL_CONSTRUCT_BWT
#define INCLUDED_SDSL_CONSTRUCT_BWT

#include "int_vector.h"
#include "sfstream.h"
#include "util.h"
#include "config.h" // for cache_config

#include <iostream>
#include <stdexcept>
#include <list>

namespace bluebird {

    //! Constructs the Burrows and Wheeler Transform (BWT) from text over byte- or integer-alphabet and suffix array.
    /*!	The algorithm constructs the BWT and stores it to disk.
     *  \tparam t_width Width of the text. 0==integer alphabet, 8=byte alphabet.
     *  \param config	Reference to cache configuration
     *  \par Space complexity
     *		\f$ n \log \sigma \f$ bits
     *  \pre Text and Suffix array exist in the cache. Keys:
     *         * conf::KEY_TEXT for t_width=8 or conf::KEY_TEXT_INT for t_width=0
     *         * conf::KEY_SA
     *  \post BWT exist in the cache. Key
     *         * conf::KEY_BWT for t_width=8 or conf::KEY_BWT_INT for t_width=0
     */
    template<uint8_t t_width>
    void construct_bwt(cache_config &config) {
        static_assert(t_width == 0 or t_width == 8,
                      "construct_bwt: width must be `0` for integer alphabet and `8` for byte alphabet");

        typedef int_vector<>::size_type size_type;
        typedef int_vector<t_width> text_type;
        typedef int_vector_buffer<t_width> bwt_type;
        const char *KEY_TEXT = key_text_trait<t_width>::KEY_TEXT;
        const char *KEY_BWT = key_bwt_trait<t_width>::KEY_BWT;

        //  (1) Load text from disk
        text_type text;
        load_from_cache(text, KEY_TEXT, config);
        size_type n = text.size();
        uint8_t bwt_width = text.width();

        //  (2) Prepare to stream SA from disc and BWT to disc
        size_type buffer_size = 1000000; // buffer_size is a multiple of 8!, TODO: still true?
        int_vector_buffer<> sa_buf(cache_file_name(conf::KEY_SA, config), std::ios::in, buffer_size);
        std::string bwt_file = cache_file_name(KEY_BWT, config);
        bwt_type bwt_buf(bwt_file, std::ios::out, buffer_size, bwt_width);

        //  (3) Construct BWT sequentially by streaming SA and random access to text
        size_type to_add[2] = {(size_type) -1, n - 1};
        for (size_type i = 0; i < n; ++i) {
            bwt_buf[i] = text[sa_buf[i] + to_add[sa_buf[i] == 0]];
        }
        bwt_buf.close();
        register_cache_file(KEY_BWT, config);
    }

}// end namespace

#endif
