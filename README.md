<!--
SPDX-FileCopyrightText: 2022 The Qt Company Ltd.

SPDX-License-Identifier: BSD-3-Clause
-->

# QtHealth

**QtHealth is not an official product of the Qt Company nor an official Qt module, no support is provided**

QtHealth is a hackathon project to create a proof-on-concept cross-platform Qt framework for accessing health and fitness data.
It currently only supports HealthKit but uses a plugin system that allows for supporting any number of APIs.

## Requirements
- Xcode
- Qt 6.2+ built for iOS

## Building

You can build the project in the following way:
```sh
mkdir build && cd build
$IOS_QT_INSTALL_DIR/bin/qt-cmake -GXCode ..
xcodebuild
```

## Examples

There is an example iOS app available that shows off most of Qt Health's features.

## License

This project is licensed under the BSD 3-Clause ["New" or "Revised" License](LICENSES/BSD-3-Clause.txt).
