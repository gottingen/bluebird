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

/*! \file lcp_bitcompressed.h
    \brief lcp_bitcompressed.h contains a  bitcompressed LCP array.
    \author Simon Gog
*/
#ifndef INCLUDED_SDSL_LCP_BITCOMPRESSED
#define INCLUDED_SDSL_LCP_BITCOMPRESSED

#include "lcp.h"
#include "int_vector.h"
#include "iterators.h"

namespace bluebird {

    template<uint8_t t_width = 0>
    class lcp_bitcompressed {
    public:

        typedef typename int_vector<t_width>::value_type value_type;
        typedef typename int_vector<t_width>::size_type size_type;
        typedef random_access_const_iterator<lcp_bitcompressed> const_iterator;
        typedef const_iterator iterator;
        typedef const value_type const_reference;
        typedef const_reference reference;
        typedef const_reference *pointer;
        typedef const pointer const_pointer;
        typedef ptrdiff_t difference_type;

        typedef lcp_plain_tag lcp_category;
        typedef lcp_tag index_category;

        enum {
            fast_access = 1,
            text_order = 0,
            sa_order = 1
        };

        template<class Cst>
        using type = lcp_bitcompressed;

    private:

        int_vector<t_width> m_lcp;

    public:

        //! Default Constructor
        lcp_bitcompressed() {}

        lcp_bitcompressed(const lcp_bitcompressed &) = default;

        lcp_bitcompressed(lcp_bitcompressed &&) = default;

        lcp_bitcompressed &operator=(const lcp_bitcompressed &) = default;

        lcp_bitcompressed &operator=(lcp_bitcompressed &&) = default;

        //! Constructor taking a cache_config
        lcp_bitcompressed(cache_config &config) {
            std::string lcp_file = cache_file_name(conf::KEY_LCP, config);
            int_vector_buffer<> lcp_buf(lcp_file);
            m_lcp = int_vector<t_width>(lcp_buf.size(), 0, lcp_buf.width());
            for (size_type i = 0; i < m_lcp.size(); ++i) {
                m_lcp[i] = lcp_buf[i];
            }
        }

        //! Number of elements in the instance.
        size_type size() const {
            return m_lcp.size();
        }

        //! Returns the largest size that lcp_bitcompressed can ever have.
        static size_type max_size() {
            return int_vector<t_width>::max_size();
        }

        //! Returns if the data structure is empty.
        bool empty() const {
            return m_lcp.empty();
        }

        //! Swap method for lcp_bitcompressed
        void swap(lcp_bitcompressed &lcp_c) {
            m_lcp.swap(lcp_c.m_lcp);
        }

        //! Returns a const_iterator to the first element.
        const_iterator begin() const {
            return const_iterator(this, 0);
        }

        //! Returns a const_iterator to the element after the last element.
        const_iterator end() const {
            return const_iterator(this, size());
        }

        //! Access operator
        /*! \param i Index of the value. \f$ i \in [0..size()-1]\f$.
         */
        value_type operator[](size_type i) const {
            return m_lcp[i];
        }

        //! Serialize to a stream.
        size_type serialize(std::ostream &out, structure_tree_node *v = nullptr,
                            std::string name = "") const {
            structure_tree_node *child = structure_tree::add_child(v, name,
                                                                   util::class_name(*this));
            size_type written_bytes = 0;
            written_bytes += m_lcp.serialize(out, child, "lcp");
            structure_tree::add_size(child, written_bytes);
            return written_bytes;
        }

        //! Load from a stream.
        void load(std::istream &in) {
            m_lcp.load(in);
        }
    };

} // end namespace bluebird
#endif
