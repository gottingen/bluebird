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

/*! \file construct.h
    \brief construct.h contains methods to construct indexes (compressed suffix arrays and trees).
	\author Simon Gog
*/

#ifndef INCLUDED_SDSL_CONSTRUCT
#define INCLUDED_SDSL_CONSTRUCT

#include "sdsl_concepts.h"
#include "int_vector.h"
#include "construct_lcp.h"
#include "construct_bwt.h"
#include "construct_sa.h"
#include <string>

namespace bluebird {

    template<class int_vector>
    bool contains_no_zero_symbol(const int_vector &text, const std::string &file) {
        for (int_vector_size_type i = 0; i < text.size(); ++i) {
            if ((uint64_t) 0 == text[i]) {
                throw std::logic_error(std::string("Error: File \"") + file + "\" contains zero symbol.");
                return false;
            }
        }
        return true;
    }

    template<class int_vector>
    void append_zero_symbol(int_vector &text) {
        text.resize(text.size() + 1);
        text[text.size() - 1] = 0;
    }


    template<class t_index>
    void construct(t_index &idx, std::string file, uint8_t num_bytes = 0) {
        tMSS file_map;
        cache_config config;
        if (is_ram_file(file)) {
            config.dir = "@";
        }
        construct(idx, file, config, num_bytes);
    }

    template<class t_index, class t_data>
    void construct_im(t_index &idx, t_data data, uint8_t num_bytes = 0) {
        std::string tmp_file = ram_file_name(util::to_string(util::pid()) + "_" + util::to_string(util::id()));
        store_to_file(data, tmp_file);
        construct(idx, tmp_file, num_bytes);
        ram_fs::remove(tmp_file);
    }

//! Constructs an index object of type t_index for a text stored on disk.
/*!
 * \param idx       	t_index object.  Any sdsl suffix array of suffix tree.
 * \param file      	Name of the text file. The representation of the file
 *                  	is dependent on the next parameter.
 * \
 * \param num_bytes 	If `num_bytes` equals 0, the file format is a serialized
 *				    	int_vector<>. Otherwise the file is interpreted as sequence
 *                  	of `num_bytes`-byte integer stored in big endian order.
 */
    template<class t_index>
    void construct(t_index &idx, const std::string &file, cache_config &config, uint8_t num_bytes = 0) {
        // delegate to CSA or CST construction
        typename t_index::index_category index_tag;
        construct(idx, file, config, num_bytes, index_tag);
    }

// Specialization for WTs
    template<class t_index>
    void construct(t_index &idx, const std::string &file, cache_config &config, uint8_t num_bytes, wt_tag) {
        auto event = memory_monitor::event("construct wavelet tree");
        if ((t_index::alphabet_category::WIDTH == 8 and num_bytes <= 1)
            or (t_index::alphabet_category::WIDTH == 0 and num_bytes != 'd')) {
            int_vector_buffer<t_index::alphabet_category::WIDTH> text_buf(file, std::ios::in, 1024 * 1024,
                                                                          num_bytes * 8, (bool) num_bytes);
            t_index tmp(text_buf, text_buf.size());
            idx.swap(tmp);
        } else {
            int_vector<t_index::alphabet_category::WIDTH> text;
            load_vector_from_file(text, file, num_bytes);
            std::string tmp_key = util::to_string(util::pid()) + "_" + util::to_string(util::id());
            std::string tmp_file_name = cache_file_name(tmp_key, config);
            store_to_file(text, tmp_file_name);
            util::clear(text);
            {
                int_vector_buffer<t_index::alphabet_category::WIDTH> text_buf(tmp_file_name);
                t_index tmp(text_buf, text_buf.size());
                idx.swap(tmp);
            }
            bluebird::remove(tmp_file_name);
        }
    }

// Specialization for CSAs
    template<class t_index>
    void construct(t_index &idx, const std::string &file, cache_config &config, uint8_t num_bytes, csa_tag) {
        auto event = memory_monitor::event("construct CSA");
        const char *KEY_TEXT = key_text_trait<t_index::alphabet_category::WIDTH>::KEY_TEXT;
        const char *KEY_BWT = key_bwt_trait<t_index::alphabet_category::WIDTH>::KEY_BWT;
        typedef int_vector<t_index::alphabet_category::WIDTH> text_type;
        {
            auto event = memory_monitor::event("parse input text");
            // (1) check, if the text is cached
            if (!cache_file_exists(KEY_TEXT, config)) {
                text_type text;
                load_vector_from_file(text, file, num_bytes);
                if (contains_no_zero_symbol(text, file)) {
                    append_zero_symbol(text);
                    store_to_cache(text, KEY_TEXT, config);
                }
            }
            register_cache_file(KEY_TEXT, config);
        }
        {
            // (2) check, if the suffix array is cached
            auto event = memory_monitor::event("SA");
            if (!cache_file_exists(conf::KEY_SA, config)) {
                construct_sa<t_index::alphabet_category::WIDTH>(config);
            }
            register_cache_file(conf::KEY_SA, config);
        }
        {
            //  (3) construct BWT
            auto event = memory_monitor::event("BWT");
            if (!cache_file_exists(KEY_BWT, config)) {
                construct_bwt<t_index::alphabet_category::WIDTH>(config);
            }
            register_cache_file(KEY_BWT, config);
        }
        {
            //  (4) use BWT to construct the CSA
            auto event = memory_monitor::event("construct CSA");
            t_index tmp(config);
            idx.swap(tmp);
        }
        if (config.delete_files) {
            auto event = memory_monitor::event("delete temporary files");
            util::delete_all_files(config.file_map);
        }
    }

// Specialization for standalone LCPs
    template<class t_index, uint8_t t_width>
    void construct(t_index &idx, const std::string &file, cache_config &config, uint8_t num_bytes, lcp_tag) {
        auto event = memory_monitor::event("construct compressed LCP");
        const char *KEY_TEXT = key_text_trait<t_width>::KEY_TEXT;
        typedef int_vector<t_width> text_type;
        {
            // (2) check, if the longest common prefix array is cached
            auto event = memory_monitor::event("LCP");
            if (!cache_file_exists(conf::KEY_LCP, config)) {
                {
                    auto event = memory_monitor::event("parse input text");
                    // (1) check, if the text is cached
                    if (!cache_file_exists(KEY_TEXT, config)) {
                        text_type text;
                        load_vector_from_file(text, file, num_bytes);
                        if (contains_no_zero_symbol(text, file)) {
                            append_zero_symbol(text);
                            store_to_cache(text, KEY_TEXT, config);
                        }
                    }
                    register_cache_file(KEY_TEXT, config);
                }
                {
                    // (2) check, if the suffix array is cached
                    auto event = memory_monitor::event("SA");
                    if (!cache_file_exists(conf::KEY_SA, config)) {
                        construct_sa<t_width>(config);
                    }
                    register_cache_file(conf::KEY_SA, config);
                }
                if (t_width == 8) {
                    construct_lcp_semi_extern_PHI(config);
                } else {
                    construct_lcp_PHI<t_width>(config);
                }
            }
            register_cache_file(conf::KEY_LCP, config);
        }
        {
            auto event = memory_monitor::event("compressed LCP");
            t_index tmp(config);
            tmp.swap(idx);
        }
        if (config.delete_files) {
            auto event = memory_monitor::event("delete temporary files");
            util::delete_all_files(config.file_map);
        }
    }

// Specialization for standalone LCPs
    template<class t_index>
    void construct(t_index &idx, const std::string &file, cache_config &config, uint8_t num_bytes, lcp_tag tag) {
        if (1 == num_bytes) {
            construct<t_index, 8>(idx, file, config, num_bytes, tag);
        } else {
            construct<t_index, 0>(idx, file, config, num_bytes, tag);
        }
    }


// Specialization for CSTs
    template<class t_index>
    void construct(t_index &idx, const std::string &file, cache_config &config, uint8_t num_bytes, cst_tag) {
        auto event = memory_monitor::event("construct CST");
        const char *KEY_TEXT = key_text_trait<t_index::alphabet_category::WIDTH>::KEY_TEXT;
        const char *KEY_BWT = key_bwt_trait<t_index::alphabet_category::WIDTH>::KEY_BWT;
        csa_tag csa_t;
        {
            // (1) check, if the compressed suffix array is cached
            typename t_index::csa_type csa;
            if (!cache_file_exists(std::string(conf::KEY_CSA) + "_" + util::class_to_hash(csa), config)) {
                cache_config csa_config(false, config.dir, config.id, config.file_map);
                construct(csa, file, csa_config, num_bytes, csa_t);
                auto event = memory_monitor::event("store CSA");
                config.file_map = csa_config.file_map;
                store_to_cache(csa, std::string(conf::KEY_CSA) + "_" + util::class_to_hash(csa), config);
            }
            register_cache_file(std::string(conf::KEY_CSA) + "_" + util::class_to_hash(csa), config);
        }
        {
            // (2) check, if the longest common prefix array is cached
            auto event = memory_monitor::event("LCP");
            register_cache_file(KEY_TEXT, config);
            register_cache_file(KEY_BWT, config);
            register_cache_file(conf::KEY_SA, config);
            if (!cache_file_exists(conf::KEY_LCP, config)) {
                if (t_index::alphabet_category::WIDTH == 8) {
                    construct_lcp_semi_extern_PHI(config);
                } else {
                    construct_lcp_PHI<t_index::alphabet_category::WIDTH>(config);
                }
            }
            register_cache_file(conf::KEY_LCP, config);
        }
        {
            auto event = memory_monitor::event("CST");
            t_index tmp(config);
            tmp.swap(idx);
        }
        if (config.delete_files) {
            auto event = memory_monitor::event("delete temporary files");
            util::delete_all_files(config.file_map);
        }
    }

} // end namespace bluebird
#endif
