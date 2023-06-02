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

/*! \file construct_isa.h
    \brief construct_isa.h contains a space and time efficient construction method for the inverse suffix array
	\author Simon Gog
*/
#ifndef INCLUDED_SDSL_CONSTRUCT_ISA
#define INCLUDED_SDSL_CONSTRUCT_ISA

#include "int_vector.h"
#include "util.h"

#include <iostream>
#include <stdexcept>
#include <list>

namespace bluebird {

    void construct_isa(cache_config &config);

}// end namespace

#endif
