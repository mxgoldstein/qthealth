# SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
#
# SPDX-License-Identifier: BSD-3-Clause

qt_add_executable(iosExample
    main.cpp
    Launch.storyboard
)

set_source_files_properties(Assets.xcassets PROPERTIES
    MACOSX_PACKAGE_LOCATION Resources
)

qt_add_qml_module(iosExample
    URI iosExample
    VERSION 1.0
    QML_FILES main.qml WorkoutList.qml WorkoutDetail.qml
    DEPENDENCIES QtHealth
    NO_LINT
)

target_compile_definitions(iosExample
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

find_package(Qt6 COMPONENTS Quick QuickControls2 QuickDialogs2 Widgets REQUIRED)

set_target_properties(iosExample PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
    MACOSX_BUNDLE_BUNDLE_NAME "io.qt.example.health"
    MACOSX_BUNDLE_GUI_IDENTIFIER "io.qt.example.health"
    XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_CURRENT_SOURCE_DIR}/iosExample.entitlements"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

target_link_libraries(iosExample
    PRIVATE
    Qt6::Quick
    Qt6::Widgets
    Qt6::QuickLayouts
    Qt6::QuickControls2
    Qt6::qtchartsqml2
    Qt6::QuickDialogs2
    QtHealthQmlplugin
    QtHealthQml
)
