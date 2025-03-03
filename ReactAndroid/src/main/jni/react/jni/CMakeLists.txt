# Copyright (c) Meta Platforms, Inc. and affiliates.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

cmake_minimum_required(VERSION 3.13)
set(CMAKE_VERBOSE_MAKEFILE on)

# TODO Those two libraries are building against the same sources
# and should probably be merged
file(GLOB reactnativejni_SRC CONFIGURE_DEPENDS *.cpp)

add_compile_options(
        -fexceptions
        -frtti
        -Wno-unused-lambda-capture
        -std=c++17)

##########################
### React Native Utils ###
##########################

add_library(
        reactnativeutilsjni
        SHARED
        ${reactnativejni_SRC}
)

# TODO This should not be ../../
target_include_directories(reactnativeutilsjni PUBLIC ../../)

target_link_libraries(reactnativeutilsjni
        android
        callinvokerholder
        fb
        fbjni
        folly_json
        glog_init
        react_render_runtimescheduler
        reactnative
        runtimeexecutor
        yoga
        )

######################
### reactnativejni ###
######################


add_library(
        reactnativejni
        SHARED
        ${reactnativejni_SRC}
)

# TODO This should not be ../../
target_include_directories(reactnativejni PUBLIC ../../)

target_link_libraries(reactnativejni
        android
        callinvokerholder
        fb
        fbjni
        folly_json
        glog_init
        logger
        react_render_runtimescheduler
        reactnative
        reactnativeutilsjni
        runtimeexecutor
        yoga
        )
