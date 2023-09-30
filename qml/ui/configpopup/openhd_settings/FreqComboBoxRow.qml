import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.0
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4

import Qt.labs.settings 1.0

import OpenHD 1.0

import "../../../ui" as Ui
import "../../elements"

Rectangle{
    property int m_background_color_type: 0

    property string m_main_text: "[] XXXXMhz"

    // 0: this row matches current openhd selected frequency
    // 1: otherwise
    property int m_selection_tpye: 0

    property bool m_is_2G: false
    property bool m_2G_5G_show: false
    property bool m_show_radar: false
    property bool m_show_good_channel: false
    property string m_pollution_text: "P:XX"
    property color m_pollution_color: "green"

    color: "transparent"
    anchors.fill: parent
    anchors.margins: 6
    // Background
    Rectangle{
        anchors.fill: parent
        //color: "transparent"
        color: m_selection_tpye===0 ? "transparent" : "#c5c3c7" // light gray
        //border.color: "red"
        //color: "red"
    }
    // The acual row, contains text and icons
    RowLayout{
        //width:parent.width
        //height:parent.height
        anchors.fill: parent
        Text {
            Layout.alignment: Qt.AlignLeft
            Layout.preferredWidth: 110
            text: m_main_text
            color: "black"
            font: comboBoxFreq.font
        }
        Text{
            Layout.alignment: Qt.AlignLeft
            text: m_is_2G ? "5.8G" : "2.4G"
            //color: value > 3000 ? "green" : "#ff8c00" //"orange"
            color: "#706F1D" // dark green
            visible: m_2G_5G_show
        }
        Text { // Radar icon
            Layout.alignment: Qt.AlignLeft
            text: qsTr("\uf7c0");
            font.family: "Font Awesome 5 Free"
            color: "red"
            visible: m_show_radar
        }
        Item{
            Layout.fillWidth: true
            // filler
        }
        Text{ // smiley icon - indicates good channel
            Layout.alignment: Qt.AlignRight
            Layout.preferredWidth: 30
            text: qsTr("\uf585")
            visible: m_show_good_channel
            font.family: "Font Awesome 5 Free"
            color: "green"
        }
        Text{
            Layout.alignment: Qt.AlignRight
            Layout.preferredWidth: 60
            text: m_pollution_text
            color: m_pollution_color
            font.family: "Font Awesome 5 Free"
        }
    }
}