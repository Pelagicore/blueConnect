import QtQuick 2.0

Item {
    ListView {
        id: deviceList
        clip: true
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        height: parent.height / 3
        anchors.margins: 10

        model: btHandsfree

        delegate: Rectangle {
            id: btDelegate
            width: parent.width
            height: bttext.height + 10

            color: model.selected ? "grey" : "transparent"

            property bool expended: false;

            clip: true
            Image {
                id: bticon
                source: "qrc:/default.png";
                width: bttext.height;
                height: bttext.height;
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 5
            }

            Text {
                anchors.left: bticon.right
                anchors.leftMargin: 5
                anchors.verticalCenter: bticon.verticalCenter
                id: bttext
                text: model.deviceName
                font.family: "FreeSerif"
                font.pointSize: 16
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                btHandsfree.select (index);
            }
        }

        Behavior on height { NumberAnimation { duration: 200} }
        }
        focus: true
    }

    Dialer {
        anchors.top: deviceList.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        onDial: {
            console.log("Dial number: " + number)
            btHandsfree.dial(number)
        }

        onHangUp: {
            console.log("Hang up")
            btHandsfree.hangUp()
        }
    }

}
