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

/*!\file structure_tree.h
   \brief structure_tree.h contains a helper class which can represent the memory structure of a class.
   \author Simon Gog
*/
#ifndef INCLUDED_SDSL_STRUCTURE_TREE
#define INCLUDED_SDSL_STRUCTURE_TREE

#include "uintx_t.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include "config.h"

//! Namespace for the succinct data structure library
namespace bluebird {

    class structure_tree_node {
    private:
        using map_type = std::unordered_map<std::string, std::unique_ptr<structure_tree_node>>;
        map_type m_children;
    public:
        const map_type &children = m_children;
        size_t size = 0;
        std::string name;
        std::string type;
    public:
        structure_tree_node(const std::string &n, const std::string &t) : name(n), type(t) {}

        structure_tree_node *add_child(const std::string &n, const std::string &t) {
            auto hash = n + t;
            auto child_itr = m_children.find(hash);
            if (child_itr == m_children.end()) {
                // add new child as we don't have one of this type yet
                structure_tree_node *new_node = new structure_tree_node(n, t);
                m_children[hash] = std::unique_ptr<structure_tree_node>(new_node);
                return new_node;
            } else {
                // child of same type and name exists
                return (*child_itr).second.get();
            }
        }

        void add_size(size_t s) { size += s; }
    };

    class structure_tree {
    public:
        static structure_tree_node *
        add_child(structure_tree_node *v, const std::string &name, const std::string &type) {
            if (v) return v->add_child(name, type);
            return nullptr;
        };

        static void add_size(structure_tree_node *v, uint64_t value) {
            if (v) v->add_size(value);
        };
    };


    template<format_type F>
    void write_structure_tree(const structure_tree_node *v, std::ostream &out, size_t level = 0);


}
#endif
