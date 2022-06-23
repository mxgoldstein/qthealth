# SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(
    QtHealth
    DESCRIPTION "A cross-platform Qt library to interface with Health and Fitness APIs"
    LANGUAGES CXX OBJCXX
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core REQUIRED)

add_subdirectory(src)
add_subdirectory(examples)

