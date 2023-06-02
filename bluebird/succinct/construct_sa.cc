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

#include "bluebird/succinct/construct_sa.h"

namespace bluebird {

    void construct_sa_se(cache_config &config) {
        int_vector<8> text;
        load_from_file(text, cache_file_name(conf::KEY_TEXT, config));

        if (text.size() <= 2) {
            // If text is c$ or $ write suffix array [1, 0] or [0]
            int_vector_buffer<> sa(cache_file_name(conf::KEY_SA, config), std::ios::out, 8, 2);
            if (text.size() == 2) {
                sa.push_back(1);
            }
            sa.push_back(0);
        } else {
            _construct_sa_se<int_vector<8>>(text, cache_file_name(conf::KEY_SA, config), 256, 0);
        }
        register_cache_file(conf::KEY_SA, config);
    }

}
