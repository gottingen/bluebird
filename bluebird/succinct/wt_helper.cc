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

#include "bluebird/succinct/wt_helper.h"

namespace bluebird {

    bool empty(const range_type &r) {
        return std::get<0>(r) == (std::get<1>(r) + 1);
    }

    int_vector<>::size_type size(const range_type &r) {
        return std::get<1>(r) - std::get<0>(r) + 1;
    }


    pc_node::pc_node(uint64_t freq, uint64_t sym, uint64_t parent,
                     uint64_t child_left, uint64_t child_right) :
            freq(freq), sym(sym), parent(parent) {
        child[0] = child_left;
        child[1] = child_right;
    }

    pc_node &pc_node::operator=(const pc_node &v) {
        freq = v.freq;
        sym = v.sym;
        parent = v.parent;
        child[0] = v.child[0];
        child[1] = v.child[1];
        return *this;
    }

}
