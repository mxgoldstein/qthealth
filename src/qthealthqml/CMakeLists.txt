# SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
#
# SPDX-License-Identifier: BSD-3-Clause

find_package(Qt6 COMPONENTS Qml REQUIRED)

qt_add_qml_module(QtHealthQml
    URI QtHealth
    VERSION 1.0
    SOURCES qml.h
)

target_link_libraries(QtHealthQml PRIVATE QtHealth)

target_link_libraries(QtHealthQml
  PRIVATE Qt6::Qml
  ${HealthKit_LIBRARY})

set_target_properties(QtHealthQml PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
