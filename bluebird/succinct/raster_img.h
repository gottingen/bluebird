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

#ifndef SDSL_INC_RASTER_IMG
#define SDSL_INC_RASTER_IMG

#include <bluebird/succinct/bit_vectors.h>

namespace bluebird
        {

                struct raster_img {
                    typedef uint64_t size_type;

                    uint64_t max_x; // max x value
                    uint64_t max_y; // max y value
                    uint64_t max_z; // max z value in the compacted range
                    uint32_t offset;
                    bit_vector value_map;
                    int_vector<> data;

                    //! Serializes the data structure into the given ostream
                    uint64_t serialize(std::ostream
                    &out, structure_tree_node * v = nullptr, std::string
                    name = "")const;

                    //! Loads the data structure from the given istream.
                    void load(std::istream
                    &in);
                };

        }

#endif
