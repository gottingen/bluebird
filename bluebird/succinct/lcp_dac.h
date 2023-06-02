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

/*! \file lcp_dac.h
    \brief lcp_dac.h contains an implementation of a (compressed) LCP array.
    \author Simon Gog
*/
#ifndef INCLUDED_SDSL_LCP_DAC
#define INCLUDED_SDSL_LCP_DAC

#include "lcp.h"
#include "vectors.h"
#include "rank_support_v5.h"

namespace bluebird
{

//! A class for the compressed version of LCP information of an suffix array
/*! A dac_vector is used to compress represent the values compressed.
 *  The template parameter are forwarded to the dac_vector.
 *  \tparam t_b    Split block size.
 *  \tparam t_rank Rank structure to navigate between the different levels.
 */
template<uint8_t  t_b    = 4,
         typename t_rank = rank_support_v5<>>
using lcp_dac = lcp_vlc<dac_vector<t_b, t_rank>>;

} // end namespace bluebird
#endif
