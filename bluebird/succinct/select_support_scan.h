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

/*! \file select_support_scan.h
    \brief select_support_scan.h contains classes that support a bluebird::bit_vector with linear time select.
    \author Simon Gog
*/
#ifndef INCLUDED_SDSL_SELECT_SUPPORT_SCAN
#define INCLUDED_SDSL_SELECT_SUPPORT_SCAN

#include "int_vector.h"
#include "util.h"
#include "select_support.h"

//! Namespace for the succinct data structure library.
namespace bluebird {


//! A class supporting linear time select queries.
/*! \par Space complexity
 *       Constant.
 *  \par Time complexity
 *       Linear in the size of the supported vector.
 *
 *  \tparam t_b       Bit pattern which should be supported. Either `0`,`1`,`10`,`01`.
 *  \tparam t_pat_len Length of the bit pattern.
 * @ingroup select_support_group
 */
    template<uint8_t t_b = 1, uint8_t t_pat_len = 1>
    class select_support_scan : public select_support {
    private:
        static_assert(t_b == 1u or t_b == 0u or t_b == 10u,
                      "select_support_scan: bit pattern must be `0`,`1`,`10` or `01`");
        static_assert(t_pat_len == 1u or t_pat_len == 2u, "select_support_scan: bit pattern length must be 1 or 2");
    public:
        typedef bit_vector bit_vector_type;
        enum {
            bit_pat = t_b
        };
    public:
        explicit select_support_scan(const bit_vector *v = nullptr) : select_support(v) {}

        select_support_scan(const select_support_scan<t_b, t_pat_len> &ss) : select_support(ss.m_v) {}

        inline size_type select(size_type i) const;

        inline size_type operator()(size_type i) const {
            return select(i);
        }

        size_type serialize(std::ostream &out, structure_tree_node *v = nullptr, std::string name = "") const {
            return serialize_empty_object(out, v, name, this);
        }

        void load(std::istream &, SDSL_UNUSED const bit_vector *v = nullptr) {
            set_vector(v);
        }

        void set_vector(const bit_vector *v = nullptr) {
            m_v = v;
        }

        select_support_scan<t_b, t_pat_len> &operator=(const select_support_scan &ss) {
            set_vector(ss.m_v);
            return *this;
        }

        void swap(select_support_scan<t_b, t_pat_len> &) {}
    };

    template<uint8_t t_b, uint8_t t_pat_len>
    inline typename select_support_scan<t_b, t_pat_len>::size_type
    select_support_scan<t_b, t_pat_len>::select(size_type i) const {
        const uint64_t *data = m_v->data();
        size_type word_pos = 0;
        size_type word_off = 0;
        uint64_t carry = select_support_trait<t_b, t_pat_len>::init_carry(data, word_pos);
        size_type args = select_support_trait<t_b, t_pat_len>::args_in_the_first_word(*data, word_off, carry);
        if (args >= i) {
            return (word_pos << 6) +
                   select_support_trait<t_b, t_pat_len>::ith_arg_pos_in_the_first_word(*data, i, word_off, carry);
        }
        word_pos += 1;
        size_type sum_args = args;
        carry = select_support_trait<t_b, t_pat_len>::get_carry(*data);
        uint64_t old_carry = carry;
        args = select_support_trait<t_b, t_pat_len>::args_in_the_word(*(++data), carry);
        while (sum_args + args < i) {
            sum_args += args;
            assert(data + 1 < m_v->data() + (m_v->capacity() >> 6));
            old_carry = carry;
            args = select_support_trait<t_b, t_pat_len>::args_in_the_word(*(++data), carry);
            word_pos += 1;
        }
        return (word_pos << 6) +
               select_support_trait<t_b, t_pat_len>::ith_arg_pos_in_the_word(*data, i - sum_args, old_carry);
    }

} // end namespace
#endif