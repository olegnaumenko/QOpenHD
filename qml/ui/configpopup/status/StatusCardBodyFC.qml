import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.1
import QtQuick.Controls.Material 2.12

import Qt.labs.settings 1.0

import OpenHD 1.0

import "../../../ui" as Ui
import "../../elements"

ColumnLayout {
    // from https://doc.qt.io/qt-6/qml-qtquick-layouts-rowlayout.html
    anchors.fill: parent
    spacing: 2

    property int rowHeight: 64
    property int text_minHeight: 20

    RowLayout{
        Layout.fillWidth: true
        Layout.minimumHeight: text_minHeight
        spacing: 6
        Text {
            text: qsTr("Autopilot:")
            height: 24
            font.pixelSize: 14
            font.bold: true
            leftPadding: 12
        }
        Text {
            text: _fcMavlinkSystem.autopilot_type
            height: 24
            width: 256
            font.pixelSize: 14
            leftPadding: 6
        }
    }
    RowLayout{
        Layout.fillWidth: true
        Layout.minimumHeight: text_minHeight
        spacing: 6
        Text {
            text: qsTr("MAV type:")
            height: 24
            font.pixelSize: 14
            font.bold: true
            leftPadding: 12
        }
        Text {
            text: _fcMavlinkSystem.mav_type
            height: 24
            width: 256
            font.pixelSize: 14
            leftPadding: 6
        }
    }
    RowLayout{
        Layout.fillWidth: true
        Layout.minimumHeight: text_minHeight
        spacing: 6
        Text {
            text: qsTr("Last Ping:")
            height: 24
            font.pixelSize: 14
            font.bold: true
            leftPadding: 12
        }
        Text {
            text: _fcMavlinkSystem.last_ping_result_flight_ctrl
            height: 24
            width: 256
            font.pixelSize: 14
            leftPadding: 6
        }
    }
    RowLayout{
        Layout.fillWidth: true
        Layout.minimumHeight: text_minHeight
        spacing: 6
        Text {
            text: qsTr("Alive: ")
            height: 24
            font.pixelSize: 14
            font.bold: true
            leftPadding: 12
        }
        Text {
            text: _fcMavlinkSystem.is_alive ? "Yes" : "No"
            color: _fcMavlinkSystem.is_alive ? "GREEN" : "RED"
            height: 24
            width: 256
            font.pixelSize: 14
            leftPadding: 6
        }
    }
    RowLayout{
        Layout.fillWidth: true
        Layout.minimumHeight: text_minHeight
        spacing: 6
        Text {
            text: qsTr("Sys id: ")
            height: 24
            font.pixelSize: 14
            font.bold: true
            leftPadding: 12
        }
        Text {
            text: _fcMavlinkSystem.for_osd_sys_id == -1 ? "na" : qsTr(""+_fcMavlinkSystem.for_osd_sys_id)
            height: 24
            width: 256
            font.pixelSize: 14
            leftPadding: 6
        }
    }
}