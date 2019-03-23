import QtQuick 2.4
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

Item {
    id: container
    anchors.fill: parents
    Page {
        header: TabBar {
            id: settingBar
            anchors.fill: parents
            TabButton {
                text: qsTr("General")
                //width: implicitWidth
                width: container.width / 3
            }
            TabButton {
                text: qsTr("Editor")
                width: container.width / 3
            }
            TabButton {
                text: qsTr("TeX")
                width: container.width / 3
            }
        }
        StackLayout {
            id: stackLayout
            anchors.fill: parent
            currentIndex: settingBar.currentIndex
            Page {
                anchors.fill: parent
                ColumnLayout {
                    anchors.fill: parent
                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        Label {
                            text: qsTr("Show detail log")
                        }
                        Switch {
                            id: detailLog
                        }
                    }
                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        Label {
                            text: qsTr("Show detail error")
                        }
                        Switch {
                            id: detailError
                        }
                    }
                }
            }
            Page { // page editor
                anchors.fill: parent
                GridLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    anchors.horizontalCenter: parent.horizontalCenter
                    columnSpacing: Theme.baseSize * 0.5
                    rowSpacing: Theme.baseSize * 0.5
                    rows: 4
                    columns: 2
                    FontDialog{
                        id: fontDialog
                        onAccepted: fontSelector.font = fontDialog.font
                    }
                    Label {
                        text: qsTr("Select Font")
                    }
                    Button {
                        id: fontSelector
                        text: qsTr("The quick brown fox jumps over the lazy dog.")
                        font: fontDialog.fonts
                        Layout.fillWidth: true
                        onClicked: fontDialog.open()
                    }
                    Label {
                        text: qsTr("Auto Save")
                    }
                    Switch {
                        id: autoSave
                        Layout.fillWidth: true
                    }
                }
            }
            Page { // page TeX
                anchors.fill: parent
                GridLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    anchors.horizontalCenter: parent.horizontalCenter
                    columnSpacing: Theme.baseSize * 0.5
                    rowSpacing: Theme.baseSize * 0.5
                    rows: 4
                    columns: 2

                    Label {
                        text: qsTr("latex location:")
                    }
                    TextInput {
                        Layout.fillWidth: true
                    }
                    Label {
                        text: qsTr("viewer location:")
                    }
                    TextInput {
                        Layout.fillWidth: true
                    }
                }
            }
        }
    }
}

