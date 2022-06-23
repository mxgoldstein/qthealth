// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

import QtHealth

Item {
    required property StackView stackView
    property var workouts

    function typeToString(x) {
        switch (x) {
        case HealthWorkout.Unknown: return "N/A";
        case HealthWorkout.Running: return "Run";
        case HealthWorkout.Walking: return "Walk";
        case HealthWorkout.Swimming: return "Swimm";
        case HealthWorkout.Cycling: return "Bike";
        case HealthWorkout.CoreTraining: return "Core";
        default: return "???";
        }
    }

    function statusStr(x) {
        switch (x) {
        case 0: return "StatusDenied";
        case 1: return "StatusGranted";
        case 2: return"StatusNotRequested";
        case 3: return"StatusUnavailable";
        default: return "???"
        }
    }

    HealthQuery {
        id: workout
        dataType: HealthData.Workout
        startDate: { var date = new Date(); date.setDate(date.getDate()-90); return date; }

        onSuccess: (data) => {
                       console.log("Got "+data.length+" entry(s)");
                       workouts = data;
                       for (var i = 0; i < workouts.length; i++)
                       listView.model.append({
                                                 "type": typeToString(workouts[i].type),
                                                 "time": workouts[i].startDate.toLocaleString(Qt.locale("en_GB"), Locale.ShortFormat),
                                                 "duration": Math.round(workouts[i].duration.value(HealthValue.Minutes))+" min",
                                                 "distance": Math.round(workouts[i].distance.value(HealthValue.Kilometers))+" km",
                                                 "header": false,
                                                 "index": i
                                             });
                   }
        onFailure: (error, message) => { console.log(error, message); }
        onAborted: console.log("Aborted!");
        sortBy: HealthQuery.ByDate
        sortOrder: HealthQuery.Descending
        workoutType: HealthWorkout.Running
    }

    HealthProvider {
        id: provider
        Component.onCompleted: {
            if (!provider.isAvailable()) {
                msgDialog.title = "Qt Health Error";
                msgDialog.text = "The selected health plugin is unavailable";
                msgDialog.open();
                return;
            }
            provider.requestPermissions(HealthData.Workout | HealthData.HeartData | HealthData.GeoData, HealthData.Workout | HealthData.HeartData | HealthData.GeoData);
        }

        onPermissionsRequested:
            (success, error, message) => {
                if (!success) {
                    console.log(error, message);
                    msgDialog.title = "Qt Health Error";
                    msgDialog.text = "An error has occured: "+message;
                    msgDialog.open();
                } else {
                    console.log("Success!");
                    /*console.log("workoutsRead: "+statusStr(provider.permission(HealthData.Workout, HealthProvider.Read)));
                    console.log("workoutsWrite: "+statusStr(provider.permission(HealthData.Workout, HealthProvider.Write)));
                    console.log("heartrateRead: "+statusStr(provider.permission(HealthData.HeartRate, HealthProvider.Read)));
                    console.log("heartrateWrite: "+statusStr(provider.permission(HealthData.HeartData, HealthProvider.Write)));
                    console.log("locationRead: "+statusStr(provider.permission(HealthData.LocationData, HealthProvider.Read)));
                    console.log("locationWrite: "+statusStr(provider.permission(HealthData.LocationData, HealthProvider.Write)));*/
                    provider.executeQuery(workout);
                }
        }
    }

    MessageDialog {
        id: msgDialog
        title: "Error"
        text: ""
        onAccepted: {
        }
    }

    ListView {
        anchors.leftMargin: 10
        anchors.rightMargin: 20
        anchors.fill: parent
        id: listView
        model: ListModel {
            ListElement {
                type: "Type"
                time: "Time"
                distance: "Distance"
                duration: "Duration"
                header: true
                index: -1
            }
        }
        delegate: RowLayout {
            height: 50
            Text {
                text: model.type;
                color: model.header ? "#f3f3f4" : "#cecfd5"
                Layout.fillWidth: true
                Layout.preferredWidth: 45
                font.bold: model.header
            }
            Text {
                text: model.time
                color: model.header ? "#f3f3f4" : "#cecfd5"
                Layout.fillWidth: true
                Layout.preferredWidth: 150
                font.bold: model.header
            }
            Text {
                text: model.duration
                color: model.header ? "#f3f3f4" : "#cecfd5"
                Layout.fillWidth: true
                Layout.preferredWidth: 75
                font.bold: model.header
            }
            Text {
                text: model.distance
                color: model.header ? "#f3f3f4" : "#cecfd5"
                Layout.fillWidth: true
                Layout.preferredWidth: 60
                font.bold: model.header
            }
            Button {
                visible: !model.header
                onPressed: {
                    var detail = Qt.createComponent("WorkoutDetail.qml", Component.PreferSynchronous);
                    if (detail.status === Component.Ready) {
                        var obj = detail.createObject(null, {model: model, workout: workouts[model.index], provider: provider });
                        stackView.push(obj);
                    } else {
                        console.log(detail.status, detail.errorString());
                    }
                }
                Layout.preferredWidth: 30
                contentItem: Text { text: ">"; color: "#41cd52"; }
                background: Rectangle { color: "transparent" }
            }
        }
    }
}
