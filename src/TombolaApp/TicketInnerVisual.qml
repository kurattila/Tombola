import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

Item {
    id: ticket
    anchors.fill: parent

    property alias blockName: blockNameText.text
    property alias ticketName: ticketIdText.text
    property color fontColor
//    property alias paperColor: ticket.color
    property color paperColor
    property string shaderBlendMode



    // Coloured paper background
    Item {
        id: coloredPaperTexture
        visible: false
        anchors.fill: parent

        Image
        {
            id: whitePaperTexture
            anchors.fill: parent
            source: "Images/PaperTexture_Equalized.jpg"
        }

        ColorOverlay {
            anchors.fill: parent
            source: whitePaperTexture
            color: Qt.rgba(ticket.paperColor.r, ticket.paperColor.g, ticket.paperColor.b, 0.85)
        }
    }



    // Cut rectangular to ticket shape
    Image {
        id: ticketMask
        visible: false
        anchors.fill: parent
        source: "Images/TicketMask.png"
    }

    OpacityMask {
        anchors.fill: parent
        source: coloredPaperTexture
        maskSource: ticketMask
    }



    // Black/White stroke around the ticket shape
    Image
    {
        id: ticketBorderMask
        anchors.fill: parent
        source: "Images/TicketBorderMask.png"
    }

    ColorOverlay {
        anchors.fill: parent
        source: ticketBorderMask
        color: ticket.fontColor
    }



    // Texts: block name & ticket number
    Item {
        id: ticketText
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
                font.weight: Font.Black
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
                font.weight: Font.Black
            }
        }

    }

//    Blend {
//        anchors.fill: parent
////        source: ticketText
////        foregroundSource: coloredPaperTexture
//        source: coloredPaperTexture
//        foregroundSource: ticketText
////        mode: "average"
////        mode: "softLight"
////        mode: "screen"
//        mode: ticket.shaderBlendMode
//    }

}
