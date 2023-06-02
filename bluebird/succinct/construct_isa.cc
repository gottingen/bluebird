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


#include "bluebird/succinct/construct_isa.h"
#include <string>

namespace bluebird {

    void construct_isa(cache_config &config) {
        typedef int_vector<>::size_type size_type;
        if (!cache_file_exists(conf::KEY_ISA, config)) {   // if isa is not already on disk => calculate it
            int_vector_buffer<> sa_buf(cache_file_name(conf::KEY_SA, config));
            if (!sa_buf.is_open()) {
                throw std::ios_base::failure("cst_construct: Cannot load SA from file system!");
            }
            int_vector<> isa(sa_buf.size());
            for (size_type i = 0; i < isa.size(); ++i) {
                isa[sa_buf[i]] = i;
            }
            store_to_cache(isa, conf::KEY_ISA, config);
        }
    }

}// end namespace
