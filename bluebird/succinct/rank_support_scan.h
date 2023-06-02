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

/*! \file rank_support_scan.h
    \brief rank_support_scan.h contains rank_support_scan that support a bluebird::bit_vector with linear time rank information.
    \author Simon Gog
*/
#ifndef INCLUDED_SDSL_RANK_SUPPORT_SCAN
#define INCLUDED_SDSL_RANK_SUPPORT_SCAN

#include "rank_support.h"

//! Namespace for the succinct data structure library.
namespace bluebird {

//! A class supporting rank queries in linear time.
/*! \par Space complexity
 *       Constant.
 *  \par Time complexity
 *       Linear in the size of the supported vector.
 *
 *  \tparam t_b       Bit pattern which should be supported. Either `0`,`1`,`10`,`01`.
 *  \tparam t_pat_len Length of the bit pattern.
 * @ingroup rank_support_group
 */
    template<uint8_t t_b = 1, uint8_t t_pat_len = 1>
    class rank_support_scan : public rank_support {
    private:
        static_assert(t_b == 1u or t_b == 0u or t_b == 10u or t_b == 11u,
                      "rank_support_scan: bit pattern must be `0`,`1`,`10` or `01`");
        static_assert(t_pat_len == 1u or t_pat_len == 2u, "rank_support_scan: bit pattern length must be 1 or 2");
    public:
        typedef bit_vector bit_vector_type;
        enum {
            bit_pat = t_b
        };
        enum {
            bit_pat_len = t_pat_len
        };
    public:
        explicit rank_support_scan(const bit_vector *v = nullptr) {
            set_vector(v);
        }

        rank_support_scan(const rank_support_scan &rs) {
            set_vector(rs.m_v);
        }

        size_type rank(size_type idx) const;

        size_type operator()(size_type idx) const {
            return rank(idx);
        };

        size_type size() const {
            return m_v->size();
        };

        size_type serialize(std::ostream &out, structure_tree_node *v = nullptr, std::string name = "") const {
            return serialize_empty_object(out, v, name, this);
        }

        void load(std::istream &, const int_vector<1> *v = nullptr) {
            set_vector(v);
        }

        void set_vector(const bit_vector *v = nullptr) {
            m_v = v;
        }

        //! Assign Operator
        rank_support_scan &operator=(const rank_support_scan &rs) {
            set_vector(rs.m_v);
            return *this;
        }

        //! swap Operator
        void swap(rank_support_scan &) {}
    };

    template<uint8_t t_b, uint8_t t_pat_len>
    inline typename rank_support_scan<t_b, t_pat_len>::size_type
    rank_support_scan<t_b, t_pat_len>::rank(size_type idx) const {
        assert(m_v != nullptr);
        assert(idx <= m_v->size());
        const uint64_t *p = m_v->data();
        size_type i = 0;
        size_type result = 0;
        while (i + 64 <= idx) {
            result += rank_support_trait<t_b, t_pat_len>::full_word_rank(p, i);
            i += 64;
        }
        return result + rank_support_trait<t_b, t_pat_len>::word_rank(p, idx);
    }

}// end namespace sds

#endif // end file
