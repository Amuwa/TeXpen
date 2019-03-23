import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.3
import QQTeXEdit 1.0

//import "sources"

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 800
    height: 600
    title: qsTr("TeXPen")

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                text: qsTr("&New")
            }
            Action {
                text: qsTr("&Open")
            }
            Action {
                text: qsTr("&Save")
            }
            MenuSeparator{}
            Action {
                text: qsTr("&Exit")
            }
        }
        Menu {
            title: qsTr("&Edit")
            Action {
                text: qsTr("&Undo")
            }
            Action {
                text: qsTr("&Redo")
            }
            MenuSeparator{}
            Action {
                text: qsTr("&Copy")
            }
            Action {
                text: qsTr("&Cut")
            }
            Action {
                text: qsTr("&Paste")
            }
            MenuSeparator{}
            Action {
                text: qsTr("Insert &Table")
            }
            Action {
                text: qsTr("Insert &Figure")
            }
        }
        Menu {
            title: qsTr("&View")
            MenuItem {
                checkable: true
                checked: false
                text: qsTr("&Preview PDF")
                onTriggered: {
                    pdfView.visible = checked;
                    if (pdfView.visible) mainView.anchors.right = pdfView.left
                    else mainView.anchors.right = mainView.parent.right
                }
            }
            Action {
                text: qsTr("View PDF")
            }
        }
        Menu {
            title: qsTr("&Help")
            Action {
                text: qsTr("&HomePage")
            }
            Action {
                text: qsTr("&About")
            }
        }
    }

    Page {
        id: leftFrame
        width: 100
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        background: Rectangle {
            //border.width: 1
            //border.color: "#000000"
            color: "#aaaaaa"
        }

        ListView {
            id: leftBar
            width: parent.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            ListModel {
                id: editorTabs
                ListElement {
                    name: "home"
                    index: 0
                    dtext: "Home"
                }
                ListElement {
                    name: "edit"
                    index: 1
                    dtext: "Edit"
                }
                ListElement {
                    name: "build"
                    index: 2
                    dtext: "Build"
                }
                ListElement {
                    name: "settings"
                    index: 3
                    dtext: "Settings"
                }
            }
            model: editorTabs
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            //highlightFollowsCurrentItem: false

            delegate: Frame{
                width: parent.width
                Button {
                    text: dtext
                    width: parent.width
                    onClicked: leftBar.currentIndex = index
                    background: Rectangle {
                        color: "#00000000"
                    }
                }
            }
        }
    }

    StackLayout {
        id: mainView
        anchors.left: leftFrame.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        currentIndex: leftBar.currentIndex
        Page {
            id: homeTab
            Text {
                text: qsTr("home")
            }
        }
        Page {
            id: editTab
            Layout.fillWidth: true
            ScrollView {
                anchors.fill: parent
                QQTeXEdit {
                    anchors.fill: parent
                    focus: true
                    //text: "12345678"
                }
            }
        }
        Page {
            id: buildTab
            Text {
                text: qsTr("build")
            }
        }
        Page {
            id: setTab
            SettingPage {
                anchors.fill: parent
            }
        }
    }

    Frame {
        id: pdfView
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width / 2
        visible: false
        Label {
            text: qsTr("text")
        }
    }
}
