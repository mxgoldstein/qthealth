# SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
#
# SPDX-License-Identifier: BSD-3-Clause

find_library(HealthKit_LIBRARY HealthKit REQUIRED)
find_library(CoreLocation_LIBRARY CoreLocation REQUIRED)

add_library(QtHealthPluginHealthKit qthealthpluginhealthkit.h qthealthpluginhealthkit.mm)

target_link_libraries(QtHealthPluginHealthKit PRIVATE QtHealth)

target_link_libraries(QtHealthPluginHealthKit
  PRIVATE Qt6::Core)

target_link_libraries(QtHealthPluginHealthKit
    PUBLIC ${HealthKit_LIBRARY}
    ${CoreLocation_LIBRARY})

set_target_properties(QtHealthPluginHealthKit PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

target_compile_definitions(QtHealthPluginHealthKit
    PUBLIC
      QT_STATICPLUGIN
)
