// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    visibility: Window.FullScreen
    id: window

    header: ToolBar {
        height: 80
        background: Rectangle {
            color: "#41cd52"
        }

        Label {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Qt Health"
            color: "#f3f3f4"
            font.pixelSize: 20
            font.bold: true
        }

        ToolButton {
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 25
            background: Rectangle { color: "transparent" }
            anchors.bottomMargin: 10

            id: navigateBackAction
            contentItem: Text {
                text: "Back"
                color: "#f3f3f4"
            }

            visible: stackView.depth > 1
            onPressed: {
                if (stackView.depth > 1)
                    stackView.pop()
            }
        }
    }

    StackView {
        id: stackView
        initialItem: list
        anchors.fill: parent
        background: Rectangle {
            color: "#09102b"
        }
    }

    WorkoutList {
        id: list
        stackView: stackView
    }
}
