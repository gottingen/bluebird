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

#include "bluebird/succinct/config.h"
#include "bluebird/succinct/util.h"

namespace bluebird
{
cache_config::cache_config(bool f_delete_files, std::string f_dir, std::string f_id, tMSS f_file_map) : delete_files(f_delete_files), dir(f_dir), id(f_id), file_map(f_file_map)
{
    if ("" == id) {
        id = util::to_string(util::pid())+"_"+util::to_string(util::id());
    }
}

template<>
const char* key_text_trait<0>::KEY_TEXT = conf::KEY_TEXT_INT;
template<>
const char* key_text_trait<8>::KEY_TEXT = conf::KEY_TEXT;

template<>
const char* key_bwt_trait<0>::KEY_BWT = conf::KEY_BWT_INT;
template<>
const char* key_bwt_trait<8>::KEY_BWT = conf::KEY_BWT;

}// end namespace bluebird
