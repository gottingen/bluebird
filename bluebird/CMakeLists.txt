#
# Copyright 2023 The titan-search Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

file(GLOB_RECURSE ROAR_SRC "bits/roaring/*.c")

carbin_cc_library(
        NAMESPACE bluebird
        NAME bits
        SOURCES
        ${ROAR_SRC}
        COPTS
        ${USER_CXX_FLAGS}
        DEPS
        ${CARBIN_DEPS_LINK}
        PUBLIC
)

file(GLOB_RECURSE MAT_SRC "matcher/*.cc")

carbin_cc_library(
        NAMESPACE bluebird
        NAME matcher
        SOURCES
        ${MAT_SRC}
        COPTS
        ${USER_CXX_FLAGS}
        DEPS
        ${CARBIN_DEPS_LINK}
        PUBLIC
)
option(BUILD_DIVSUFSORT64 " " ON)
add_subdirectory(divsufsort)

set(divsufsort_SRCS
        divsufsort/divsufsort.c
        divsufsort/sssort.c
        divsufsort/trsort.c
        divsufsort/utils.c)

file(GLOB_RECURSE SUC_SRC "succinct/*.cc")

carbin_cc_library(
        NAMESPACE bluebird
        NAME divsufsort
        SOURCES
        ${divsufsort_SRCS}
        COPTS
        ${USER_CXX_FLAGS}
        DEPS
        ${CARBIN_DEPS_LINK}
        PUBLIC
)

carbin_cc_library(
        NAMESPACE bluebird
        NAME divsufsort64
        SOURCES
        ${divsufsort_SRCS}
        COPTS
        ${USER_CXX_FLAGS}
        DEPS
        ${CARBIN_DEPS_LINK}
        DEFINITIONS
        "-DBUILD_DIVSUFSORT64"
        PUBLIC
)

carbin_cc_library(
        NAMESPACE bluebird
        NAME succinct
        SOURCES
        ${SUC_SRC}
        COPTS
        ${USER_CXX_FLAGS}
        DEPS
        ${CARBIN_DEPS_LINK}
        PUBLIC
)