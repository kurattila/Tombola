import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: ticket

    property alias blockName: blockNameText.text
    property alias ticketName: ticketIdText.text
    property color fontColor
    property alias paperColor: ticket.color

    border.color: fontColor
    border.width: 1
    anchors.fill: parent

    Column {
        anchors.fill: parent
        Text {
            id: blockNameText
            width: parent.width
            height: parent.height / 2
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            clip: true
            color: ticket.fontColor
            font.pixelSize: parent.height / 4.5
        }
        Text {
            id: ticketIdText
            width: parent.width
            height: parent.height / 2
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            clip: true
            color: ticket.fontColor
            font.pixelSize: parent.height / 2.5
        }
    }
}
