import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Item {
    property string number: dialinput.text
    signal dial(string number)
    signal hangUp

    Label {
        id: dialinput
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: eraseButton.left
        text: "1234"
        font.family: "FreeSerif"
        font.pointSize: 36
    }

    Button {
        id: eraseButton
        text: "< ERASE"
        anchors.right: parent.right
        height: dialinput.height
        onClicked: {
            if (dialinput.text.length > 0) {
                dialinput.text = dialinput.text.substring(
                            0, dialinput.text.length - 1)
            }
        }
    }

    Grid {
        columns: 3
        id: dialgrid
        height: parent.height * 0.66
        anchors.top: dialinput.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Button {
            id: button1
            text: "1"
            width: parent.width / 3
            height: parent.height / 4
            onClicked: {
                dialinput.text += text
            }
        }
        Button {
            text: "2"
            width: button1.width
            height: button1.height
            onClicked: {
                dialinput.text += text
            }
        }
        Button {
            text: "3"
            width: button1.width
            height: button1.height
            onClicked: {
                dialinput.text += text
            }
        }
        Button {
            text: "4"
            width: button1.width
            height: button1.height
            onClicked: {
                dialinput.text += text
            }
        }
        Button {
            text: "5"
            width: button1.width
            height: button1.height
            onClicked: {
                dialinput.text += text
            }
        }
        Button {
            text: "6"
            width: button1.width
            height: button1.height
            onClicked: {
                dialinput.text += text
            }
        }
        Button {
            text: "7"
            width: button1.width
            height: button1.height
            onClicked: {
                dialinput.text += text
            }
        }
        Button {
            text: "8"
            width: button1.width
            height: button1.height
            onClicked: {
                dialinput.text += text
            }
        }
        Button {
            text: "9"
            width: button1.width
            height: button1.height
            onClicked: {
                dialinput.text += text
            }
        }
        Button {
            text: "*"
            width: button1.width
            height: button1.height
            onClicked: {
                dialinput.text += text
            }
        }
        Button {
            text: "0"
            width: button1.width
            height: button1.height
            onClicked: {
                dialinput.text += text
            }
        }
        Button {
            text: "#"
            width: button1.width
            height: button1.height
            onClicked: {
                dialinput.text += text
            }
        }
    }
    Grid {
        columns: 2
        anchors.top: dialgrid.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        Button {
            text: "Dial"
            width: parent.width / 2
            height: button1.height
            onClicked: {
                dial(dialinput.text)
            }
        }
        Button {
            text: "Hang up"
            width: parent.width / 2
            height: button1.height
            onClicked: {
                hangUp()
            }
        }
    }
}
