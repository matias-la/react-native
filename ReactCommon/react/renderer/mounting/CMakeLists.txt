# Copyright (c) Meta Platforms, Inc. and affiliates.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

cmake_minimum_required(VERSION 3.13)
set(CMAKE_VERBOSE_MAKEFILE on)

add_compile_options(-fexceptions -frtti -std=c++17 -Wall -DLOG_TAG=\"Fabric\")

file(GLOB react_render_mounting_SRC CONFIGURE_DEPENDS *.cpp)
add_library(react_render_mounting SHARED ${react_render_mounting_SRC})

target_include_directories(react_render_mounting PRIVATE .)
target_include_directories(react_render_mounting PUBLIC ${REACT_COMMON_DIR})

target_link_libraries(react_render_mounting
        butter
        folly_futures
        folly_json
        glog
        glog_init
        jsi
        react_debug
        react_render_core
        react_render_debug
        react_render_graphics
        react_render_telemetry
        react_utils
        rrc_root
        rrc_view
        yoga)
