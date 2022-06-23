# SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
#
# SPDX-License-Identifier: BSD-3-Clause

add_library(QtHealth
    qhealthprovider.h qhealthprovider.cpp
    qhealthquery.h qhealthquery.cpp
    qhealthdata.h qhealthdata.cpp
    qhealthworkout.h qhealthworkout.cpp
    qhealthvalue.h qhealthvalue.cpp
    qhealthvaluedata.h qhealthvaluedata.cpp
    qhealthgeodata.h qhealthgeodata.cpp
)

target_sources(QtHealth PUBLIC
    qthealthplugininterface.h
    qhealthprovider.h
    qhealthquery.h
    qhealthdata.h
    qhealthworkout.h
    qhealthvalue.h
    qhealthgeodata.h
)

target_include_directories(QtHealth INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(QtHealth
  PRIVATE Qt6::Core)

set_target_properties(QtHealth PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

if (IOS)
    target_link_libraries(QtHealth PUBLIC
        QtHealthPluginHealthkit
    )
endif()
