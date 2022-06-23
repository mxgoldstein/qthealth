// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

import QtQuick
import QtQuick.Layouts
import QtWebView
import QtQuick.Controls
import QtCharts
import QtHealth

Item {
    required property var model
    required property HealthWorkout workout
    required property HealthProvider provider
    function mapUrl(coord1, coord2, coord11, coord12) { return "https://www.openstreetmap.org/export/embed.html?bbox="+coord2+"%2C"+coord1+"%2C"+coord12+"%2C"+coord11+"&layer=mapnik&marker="+coord1+"%2C"+coord2 }

    HealthQuery {
        id: heartbeatQuery
        dataType: HealthData.HeartData
        workout: workout

        onSuccess: (data) => {
                       console.log("Got "+data.length+" heartbeat entry(s)");
                       axisX.min = data[0].startDate
                       axisX.max = data[data.length-2].endDate
                       axisX.tickCount = 10;
                       for (var i = 0; i < data.length; i++) {
                           var bpm = data[i].value.value(HealthValue.BeatsPerMinute);
                           heartbeatSeries.append(data[i].startDate.getTime(), bpm);
                       }
                       chartFlickable.opacity = 1;
                   }
        onFailure: (error, message) => { console.log("Error!", error, message); }
        onAborted: console.log("Aborted!");
    }

    HealthQuery {
        id: routeQuery
        dataType: HealthData.GeoData
        workout: workout

        onSuccess: (data) => {
                       console.log("Got "+data.length+" route entry(s)");

                       if (data.length > 0) {
                           webView.url = mapUrl(data[0].latitude, data[0].longitude, data[data.length-1].latitude, data[data.length-1].longitude);
                           webView.opacity = 1;
                       }
                   }
        onFailure: (error, message) => { console.log("Error!", error, message); }
        onAborted: console.log("Aborted!");
    }

    GridLayout {
        id: gridLayout
        anchors.fill: parent
        anchors.leftMargin: 25
        anchors.rightMargin: 25
        columns: 2
        Text { text: "Details"; font.pixelSize: 25; color: "#f3f3f4"; Layout.columnSpan: 2 }

        property int spaceWidth: 70

        Text { text: "Type"; color: "#f3f3f4";  Layout.fillWidth: true; Layout.preferredWidth: parent.spaceWidth}
        Text { text: model.type ; color: "#cecfd5"; Layout.fillWidth: true;}

        Text { text: "Start"; color: "#f3f3f4"; Layout.fillWidth: true; Layout.preferredWidth: parent.spaceWidth }
        Text { text: workout.startDate.toLocaleString(Qt.locale("en_GB")); color: "#cecfd5"; Layout.fillWidth: true; }

        Text { text: "End"; color:"#f3f3f4"; Layout.fillWidth: true; Layout.preferredWidth: parent.spaceWidth }
        Text { text: workout.endDate.toLocaleString(Qt.locale("en_GB")); color: "#cecfd5"; Layout.fillWidth: true; }

        Text { text: "Duration"; color: "#f3f3f4"; Layout.fillWidth: true; Layout.preferredWidth: parent.spaceWidth }
        Text { text: Math.round(workout.duration.value(HealthValue.Minutes))+" min"; color: "#cecfd5" ; Layout.fillWidth: true;}

        Text { text: "Distance"; color: "#f3f3f4"; Layout.fillWidth: true; Layout.preferredWidth: parent.spaceWidth }
        Text { text: model.distance ; color: "#cecfd5"; Layout.fillWidth: true; }

        Text { text: "Energy"; color: "#f3f3f4"; Layout.fillWidth: true; Layout.preferredWidth: parent.spaceWidth }
        Text { text: Math.round(workout.energyBurned.value(HealthValue.KiloCalories))+" kcal" ; color: "#cecfd5"; Layout.fillWidth: true;  }

        WebView {
            id: webView
            opacity: 0
            Layout.columnSpan: 2
            Layout.fillWidth: true
            Layout.margins: 0
            Layout.topMargin: -25
            Layout.bottomMargin: -10
            height: 175
            url: mapUrl(52.52437, 13.41053)
        }

        Flickable {
            id: chartFlickable
            Layout.columnSpan: 2
            opacity: 0
            Layout.topMargin: -25
            Layout.fillWidth: true
            flickableDirection: Flickable.HorizontalFlick
            height: 200

            contentWidth: chartView.width
            contentHeight: chartView.height

            ChartView {
                id: chartView
                width: 1000
                height: 250
                theme: ChartView.ChartThemeDark
                backgroundColor: "#09102b"

                antialiasing: true

                legend.visible: false

                ValueAxis {
                    id: axisY
                    min: 50
                    max: 250
                    titleText: "Hearbeat [bpm]"
                }

                DateTimeAxis {
                    id: axisX
                    titleText: "Time"
                    format: "h:mm"
                }

                LineSeries {
                    id: heartbeatSeries
                    color: "red"
                    axisX: axisX
                    axisY: axisY
                }
            }
        }
    }


    Component.onCompleted: {
        routeQuery.workout = workout;
        heartbeatQuery.workout = workout;
        provider.executeQuery(heartbeatQuery);
        provider.executeQuery(routeQuery);
    }
}

