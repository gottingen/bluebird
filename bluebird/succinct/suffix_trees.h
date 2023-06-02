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

/*! \file suffix_trees.h
    \brief suffix_trees.h contains generic classes for different suffix tree classes.
	\author Simon Gog
*/
#ifndef INCLUDED_SDSL_SUFFIX_TREES
#define INCLUDED_SDSL_SUFFIX_TREES

/** \defgroup cst Compressed Suffix Trees (CST)
 *   This group contains data structures for compressed suffix trees. The following methods are supported:
 *    - root()
 *    - child(v,c)
 *    - select_child(v)
 *    - select_leaf(i)
 *    - parent(v)
 *    - sl(v)
 *    - lca(v,w)
 *    - ..
 */

#include "cst_sct3.h"
#include "cst_sada.h"
#include "cst_fully.h"

#endif
