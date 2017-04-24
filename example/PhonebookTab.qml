import QtQuick 2.0
import QtQuick.Controls 1.4

ScrollView {
    ListView {
        enabled: !blueConnect.isConnected
        id: contactList
        clip: true
        anchors.margins: 10

        model: btPhoneBook

        delegate: Rectangle {
            width: parent.width
            height: column.height + 10

            property bool expended: false;

            clip: true

            Column {
                id: column
                anchors.left: parent.left
                anchors.leftMargin: 5
                Text {
                    id: bttext
                    text: model.contactName
                    font.family: "FreeSerif"
                    font.pointSize: 16
                }
            }
        Behavior on height { NumberAnimation { duration: 200} }
        }
        focus: true
    }
}
