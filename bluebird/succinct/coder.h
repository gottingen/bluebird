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

/*! \file coder.h
    \brief coder.h contains the coder namespace and includes the header files of bluebird::coder::fibonacci, bluebird::coder::elias_delta, and bluebird::coder::run_length
	\author Simon Gog
 */
#ifndef SDSL_CODER
#define SDSL_CODER

#include "bluebird/succinct/int_vector.h"
#include "bluebird/succinct/coder_fibonacci.h"
#include "bluebird/succinct/coder_elias_delta.h"
#include "bluebird/succinct/coder_elias_gamma.h"
#include "bluebird/succinct/coder_comma.h"

namespace bluebird {

    //! Namespace for the different coder of the sdsl.
    namespace coder {

        template<class Coder>
        class run_length {
        public:
            typedef uint64_t size_type;

            static void encode(uint64_t x, uint64_t *&z, uint8_t offset);

            static uint64_t encoding_length(const uint64_t *s, uint8_t s_offset, size_type bit_length);
        };

        template<class Coder>
        typename run_length<Coder>::size_type
        run_length<Coder>::encoding_length(const uint64_t *s, uint8_t s_offset, size_type bit_length) {
            assert(s_offset < 64);
            size_type i = 0;
            uint64_t w = (*s >> s_offset);
            uint8_t last_bit = w & 1;
            size_type result = 0;
            while (i < bit_length) {
                size_type len = 0;
                while (last_bit == (w & 1) and i < bit_length) {
//			std::cout<<w<<" "<<i<<std::endl;
                    ++len;
                    ++i;
                    ++s_offset;
                    w >>= 1;
                    if (s_offset == 64) {
                        s_offset = 0;
                        w = *(++s);
                    }
                }
//		std::cout<<"len="<<Coder::encoding_length(len)<<std::endl;
                last_bit = (w & 1);
                result += Coder::encoding_length(len);
            }
            return result;
        }


    } // end namespace coder

} // end namespace bluebird

#endif
