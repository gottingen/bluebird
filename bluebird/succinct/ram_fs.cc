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

#include "bluebird/succinct/ram_fs.h"
#include "bluebird/succinct/util.h"
#include <cstdio>
#include <iostream>
#include <algorithm>

static int nifty_counter = 0;

bluebird::ram_fs::mss_type bluebird::ram_fs::m_map;
std::recursive_mutex bluebird::ram_fs::m_rlock;


bluebird::ram_fs_initializer::ram_fs_initializer()
{
    if (0 == nifty_counter++) {
        if (!ram_fs::m_map.empty()) {
            throw std::logic_error("Static preinitialized object is not empty.");
        }
    }
}

bluebird::ram_fs_initializer::~ram_fs_initializer()
{
    if (0 == --nifty_counter) {
        // clean up
    }
}

namespace bluebird
{

ram_fs::ram_fs() {}

void
ram_fs::store(const std::string& name, content_type data)
{
    std::lock_guard<std::recursive_mutex> lock(m_rlock);
    if (!exists(name)) {
        std::string cname = name;
        m_map.insert(std::make_pair(std::move(cname), std::move(data)));
    } else {
        m_map[name] = std::move(data);
    }
}

bool
ram_fs::exists(const std::string& name)
{
    std::lock_guard<std::recursive_mutex> lock(m_rlock);
    return m_map.find(name) != m_map.end();
}

ram_fs::content_type&
ram_fs::content(const std::string& name)
{
    std::lock_guard<std::recursive_mutex> lock(m_rlock);
    return m_map[name];
}

size_t
ram_fs::file_size(const std::string& name)
{
    std::lock_guard<std::recursive_mutex> lock(m_rlock);
    if (exists(name)) {
        return m_map[name].size();
    } else {
        return 0;
    }
}

int
ram_fs::remove(const std::string& name)
{
    std::lock_guard<std::recursive_mutex> lock(m_rlock);
    m_map.erase(name);
    return 0;
}

int
ram_fs::rename(const std::string old_filename, const std::string new_filename)
{
    std::lock_guard<std::recursive_mutex> lock(m_rlock);
    m_map[new_filename] = std::move(m_map[old_filename]);
    remove(old_filename);
    return 0;
}

bool is_ram_file(const std::string& file)
{
    if (file.size() > 0) {
        if (file[0]=='@') {
            return true;
        }
    }
    return false;
}

std::string ram_file_name(const std::string& file)
{
    if (is_ram_file(file)) {
        return file;
    } else {
        return "@" + file;
    }
}

std::string disk_file_name(const std::string& file)
{
    if (!is_ram_file(file)) {
        return file;
    } else {
        return file.substr(1);
    }
}

int remove(const std::string& file)
{
    if (is_ram_file(file)) {
        return ram_fs::remove(file);
    } else {
        return std::remove(file.c_str());
    }
}

int rename(const std::string& old_filename, const std::string& new_filename)
{
    if (is_ram_file(old_filename)) {
        if (!is_ram_file(new_filename)) {  // error, if new file is not also RAM-file
            return -1;
        }
        return ram_fs::rename(old_filename, new_filename);
    } else {
        return std::rename(old_filename.c_str(), new_filename.c_str());
    }
}

} // end namespace bluebird
