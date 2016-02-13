import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    id: appWindow
    visible: true
    width: 1200
    height: 800
    title: qsTr("Tombola")

    property int stageTransitionAnimationDuration

    Component.onCompleted: stageTransitionAnimationDuration = 1000

    MessageDialog {
        property int indexToBeDeleted
        id: deleteBlockMsgBox
        icon: StandardIcon.Question
        title: "Tombola"
        text: "Tutira törölhető az egész tömb?\n(" + ticketsSellingPoint.oneBlockSoldTickets + " db eladott jegyet tartalmaz)"
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: ticketsSellingPoint.blockSelectionList.doDeleteRow(indexToBeDeleted);
    }

    Image
    {
        anchors.fill: parent
        source: "Images/prosecco_background.jpg"
    }

    ColumnLayout
    {
        id: stageOfTicketsInput
        Behavior on opacity { NumberAnimation { duration: 1000 } }

        states:
            State {
                name: "invisible"
            }

        transitions:
            Transition {
                to: "invisible"
                SequentialAnimation {
                    NumberAnimation {
                        target: stageOfTicketsInput
                        property: "opacity"
                        to: 0
                        duration: appWindow.stageTransitionAnimationDuration / 2
                    }
                    PropertyAction { target: stageOfTicketsInput; property: "visible"; value: false }
                }
            }

        Rectangle
        {
            anchors.fill: parent
            color: "white"
            opacity: 0.5
            Behavior on opacity { NumberAnimation { duration: 1000 } }
        }

        Row
        {
            id: header
            property int headerHeight: 100
            Layout.preferredHeight: headerHeight

            ColumnLayout
            {
                width: appWindow.width - gotoPrizeDrawing.width

                Item { Layout.preferredHeight: 10 }
                Text
                {
                    Layout.leftMargin: 20
                    font.pointSize: 15
                    font.bold: true
                    color: "brown"
                    text: "Összes jegy: " + ticketsSellingPoint.totalTickets + " db"
                }
                Text
                {
                    Layout.leftMargin: 20
                    font.pointSize: 15
                    font.bold: true
                    color: "brown"
                    text: "Összes vásárolt jegy: " + ticketsSellingPoint.totalSoldTickets + " db"
                }
                SpinBox
                {
                    id: prizesCountSpinBox
                    width: 210
                    value: ticketsSellingPoint.prizesCount
                    horizontalAlignment: Text.AlignLeft
                    prefix: "Nyerhető "
                    suffix: " díj"
                    minimumValue: 1
                    maximumValue: 1000
                    stepSize: 1
                    Layout.leftMargin: 10
                    font.pointSize: 15
                    font.bold: true
                    style: SpinBoxStyle{
                        background: Rectangle {
                            width: 200
                            implicitHeight: 20
                            border.width: 0
                            color: "transparent"
                            radius: 2
                        }
                        textColor: "brown"
                        selectionColor: "orange"
                    }
                    onEditingFinished: sellPoint.forceActiveFocus()
                    Binding {
                        target: ticketsSellingPoint
                        property: "prizesCount"
                        value: prizesCountSpinBox.value
                    }
                }

                Item { Layout.fillHeight: true }
            }
            MouseArea
            {
                id: gotoPrizeDrawing
                height: header.headerHeight
                width: 250
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true
                scale: containsMouse && ticketsSellingPoint.canProceedToPrizeDrawing ? 1.1 : 1
                Behavior on scale {
                    SpringAnimation { damping: 0.1; spring: 1.0; duration: 100 }
                }

                Image {
                    width: 64
                    height: 64
                    anchors.verticalCenter: gotoPrizeDrawing.verticalCenter
                    source: "Images/CheckMark.png"
                }
                Text {
                    x: 74
                    anchors.verticalCenter: gotoPrizeDrawing.verticalCenter
                    font.pointSize: 15
                    font.underline: gotoPrizeDrawing.containsMouse && ticketsSellingPoint.canProceedToPrizeDrawing
                    font.bold: true
                    color: "brown"
                    text: "Sorsolás indul!"
                }

                opacity: ticketsSellingPoint.canProceedToPrizeDrawing ? 1 : 0
                Behavior on opacity {
                    NumberAnimation { duration: appWindow.stageTransitionAnimationDuration }
                }
                onClicked:
                {
                    if (ticketsSellingPoint.canProceedToPrizeDrawing)
                    {
                        stageOfTicketsInput.state = "invisible";
                        stageOfPrizeDrawing.state = "visible";
                        stageOfPrizeDrawing.forceActiveFocus() // important to move focus from block name input to Action of "space" which will start prize drawing!
                    }
                }
            }
        }

        Row
        {
            id: blocksScroller
            Layout.fillHeight: true
            width: appWindow.width

            Column {
                width: appWindow.width / 4

                ListView {
                    id: ticketBlocksListView
                    width: parent.width
                    height: appWindow.height - header.headerHeight - addTicketsBlockButton.height - 5
                    clip: true
                    model: ticketsSellingPoint.blockSelectionList
                    currentIndex: ticketsSellingPoint.blockIndex

                    add: Transition {
                        ParallelAnimation {
                            NumberAnimation { property: "opacity"; from: 0; duration: 500 }
                            NumberAnimation { properties: "x"; from: -100; duration: 500 }
                        }
                    }
                    remove: Transition {
                        ParallelAnimation {
                            NumberAnimation { property: "opacity"; to: 0; duration: 500 }
                            NumberAnimation { properties: "x"; to: -100; duration: 500 }
                        }
                    }
                    displaced: Transition {
                        NumberAnimation { properties: "y"; duration: 500 }
                    }
                    populate: Transition {
                        NumberAnimation { properties: "x"; duration: 1000 }
                    }
                    delegate: MouseArea {
                        width: parent.width
                        height: 100

                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: isSelected ? 0 : 5
                            color: backgroundColor
                            opacity: isSelected ? 1 : 0.7
                        }
                        Text {
                            anchors.fill: parent
                            anchors.margins: isSelected ? 0 : 8
                            clip: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            color: textColor
                            text: name + "\n" + soldTickets + "/100 eladott"
                            font.bold: isSelected
                            font.pixelSize: height / 5
                        }
                        MouseArea
                        {
                            anchors.verticalCenter: parent.verticalCenter
                            x: 4
                            width: 24
                            height: 24
                            Image
                            {
                                anchors.fill: parent
                                source: "Images/DeleteTrashBin.png"
                            }
                            visible: isSelected && ticketBlocksListView.count > 1
                            onClicked: {
                                deleteBlockMsgBox.indexToBeDeleted = index;
                                deleteBlockMsgBox.open();
                            }
                        }

                        onClicked: ticketsSellingPoint.blockSelectionList.doTicketsBlockSelection(index)
                    }
                }

                Button {
                    id: addTicketsBlockButton
                    width: parent.width
                    height: 50
                    text: "Új tömb hozzáadása"
                    onClicked:
                    {
                        ticketsSellingPoint.blockSelectionList.doAddRow();
                        blockName.focus = true;
                        blockName.selectAll();
                    }
                }
            }

            Rectangle
            {
                id: sellPoint
                width: appWindow.width * 0.75
                height: appWindow.height - header.headerHeight - 5
                color: ticketsSellingPoint.blockColor
                Column
                {
                    Row
                    {
                        id: colorSelectionRow
                        height: 20
                        spacing: 0
                        Repeater
                        {
                            model: ticketsSellingPoint.blockColorsList
                            MouseArea {
                                width: sellPoint.width / ticketsSellingPoint.blockColorsList.colorsCount;
                                height: 20
                                onClicked: ticketsSellingPoint.doColorSelection(index)
                                Rectangle
                                {
                                    anchors.fill: parent
                                    color: backgroundColor
                                }
                            }
                        }
                    }
                    Row
                    {
                        width: sellPoint.width
                        height: 40

                        Text
                        {
                            width: 0
                            text: ticketsSellingPoint.blockIndex + 1 + ". tömb neve: "
                            color: ticketsSellingPoint.textColor
                        }

                        TextInput
                        {
                            id: blockName
                            clip: true
                            height: parent.height
                            width: sellPoint.width
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter
                            text: ticketsSellingPoint.blockName
                            font.pointSize: 20
                            selectByMouse: true
                            color: ticketsSellingPoint.textColor
                            Binding {
                                target: ticketsSellingPoint
                                property: "blockName"
                                value: blockName.text
                            }
                            onEditingFinished: sellPoint.forceActiveFocus()
                        }
                    }

                    MouseArea {
                        width: sellPoint.width
                        height: sellPoint.height - colorSelectionRow.height - blockName.height

                        function hitTestForTicketId(xPos, yPos)
                        {
                            var ticketWidth = width / ticketsGrid.columns;
                            var ticketHeight = height / ticketsGrid.rows;
                            return Math.floor(xPos/ticketWidth) + Math.floor(yPos/ticketHeight)*ticketsGrid.columns + 1;
                        }

                        hoverEnabled: true
                        onPressed: { ticketsSellingPoint.beginRangeOperation(hitTestForTicketId(mouse.x, mouse.y)); }
                        onReleased: { ticketsSellingPoint.endRangeOperation(hitTestForTicketId(mouse.x, mouse.y)); }
                        onPositionChanged:
                        {
                            if (pressed && mouse.x >= 0 && mouse.x < width)
                                ticketsSellingPoint.rangeOperationInProgress(hitTestForTicketId(mouse.x, mouse.y));
                        }

                        Grid
                        {
                            id: ticketsGrid
                            anchors.fill: parent
                            spacing: 0
                            rows: 7
                            columns: 15
                            populate: Transition {
                                NumberAnimation { properties: "x"; from: appWindow.width; duration: 500 }
                                NumberAnimation { properties: "y"; from: 0; duration: 500 }
                            }


                            Repeater
                            {
                                model: ticketsSellingPoint

                                Item {
                                    width: sellPoint.width / ticketsGrid.columns
                                    height: ticketsGrid.height / ticketsGrid.rows

                                    Rectangle {
                                        color: isSold ? "transparent" : ticketsSellingPoint.unsoldTicketColor
                                        border.width: 1
                                        border.color: ticketsSellingPoint.blockColor
                                        opacity: 0.5
                                        anchors.fill: parent
                                        Behavior on color {
                                            ColorAnimation {
                                                duration: 200
                                            }
                                        }
                                    }
                                    Text {
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                        anchors.fill: parent
                                        text: model.index + 1 + (isSold ? "\n\u2713" : "")
                                        font.pixelSize: parent.width / 2
                                        font.bold: true
                                        color: isSold ? ticketsSellingPoint.textColor : ticketsSellingPoint.blockColor
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    MouseArea {
        id: stageOfPrizeDrawing
        visible: false
        opacity: 0
        anchors.fill: parent

        property int ticketsHistoryListWidth;
        property int columnLayoutGap;

        Button {
            visible: false
            action: Action {
                shortcut: " "
                onTriggered: ticketDrawExecutor.onTriggerByUser();
            }
        }

        onClicked: ticketDrawExecutor.onTriggerByUser();
//        Keys.onPressed: ticketDrawExecutor.onTriggerByUser();

        Component.onCompleted: {
            ticketsHistoryListWidth = 200;
            columnLayoutGap = 50;
        }

        states:
            State {
                name: "visible"
            }

        transitions:
            Transition {
                to: "visible"
                SequentialAnimation {
                    PauseAnimation { duration: appWindow.stageTransitionAnimationDuration / 2 }
                    ScriptAction { script: ticketDrawExecutor.onPrizeDrawingStartUp(); }
                    ScriptAction { script: ticketDrawExecutor.onTriggerByUser(); } // auto-start LEFT prize drawing
                    PropertyAction { target: stageOfPrizeDrawing; property: "visible"; value: true }
                    NumberAnimation {
                        target: stageOfPrizeDrawing
                        property: "opacity"
                        to: 1
                        duration: appWindow.stageTransitionAnimationDuration / 2
                    }
                    PauseAnimation { duration: 1000 }
                    ScriptAction { script: ticketDrawExecutor.onTriggerByUser(); } // auto-start RIGHT prize drawing
                }
            }

        ListView {
            height: parent.height
            width: stageOfPrizeDrawing.ticketsHistoryListWidth

            model: ticketDrawExecutor.ticketDrawLeft
            delegate: MouseArea {
                width: parent.width
                height: 100
                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 5
                    color: backgroundColor
                    TicketInnerVisual {
                        blockName: name
                        ticketName: ticketId
                        fontColor: textColor
                    }
                }
                onClicked: ticketDrawExecutor.ticketDrawLeft.removeFromWinningTicketsHistory(index);
            }

            add: Transition {
                NumberAnimation { property: "opacity"; from: 0; duration: 500 }
            }
            remove: Transition {
                NumberAnimation { property: "x"; to: -width; duration: 500 }
            }
            displaced: Transition {
                NumberAnimation { properties: "y"; duration: 500 }
            }
        }

        Rectangle {
            id: ticketRectLeft
            width: 300
            height: 150
            x: stageOfPrizeDrawing.ticketsHistoryListWidth + stageOfPrizeDrawing.columnLayoutGap
            y: parent.height
            color: ticketDrawExecutor.ticketDrawLeft.flyingTicketBgColor
            TicketInnerVisual {
                blockName: ticketDrawExecutor.ticketDrawLeft.flyingTicketBlockName
                ticketName: ticketDrawExecutor.ticketDrawLeft.flyingTicketId
                fontColor: ticketDrawExecutor.ticketDrawLeft.flyingTicketTextColor
            }

            transitions: [
                Transition {
                    to: "up"
                    SequentialAnimation {
                        NumberAnimation { properties: "y"; duration: ticketDrawExecutor.ticketDrawLeft.animationDuration }
                        ScriptAction { script: ticketDrawExecutor.ticketDrawLeft.onFlyUpTicketStateAchieved(); }
                    }
                },
                Transition {
                    to: "winningTicket"
                    SequentialAnimation {
                        NumberAnimation {
                            properties: "y"
                            duration: 3.5 * ticketDrawExecutor.ticketDrawLeft.animationDuration
                            easing {
                                type: Easing.OutElastic
                                amplitude: 1.0
                                period: 0.5
                            }
                        }
                        ScriptAction { script: ticketDrawExecutor.ticketDrawLeft.onWinningTicketStateAchieved(); }
                    }
                },
                Transition {
                    to: "vanishedTicket"
                    SequentialAnimation {
                        NumberAnimation { properties: "x, y, scale"; duration: 500 }
                        ScriptAction { script: ticketDrawExecutor.ticketDrawLeft.onVanishedTicketStateAchieved(); }
                    }
                }
            ]

            states: [
                State {
                    name: "up"
                    PropertyChanges {
                        target: ticketRectLeft
                        y: -ticketRectLeft.height
                    }
                },
                State {
                    name: "down"
                    PropertyChanges {
                        target: ticketRectLeft
                        y: stageOfPrizeDrawing.height
                    }
                },
                State {
                    name: "winningTicket"
                    PropertyChanges {
                        target: ticketRectLeft
                        y: stageOfPrizeDrawing.height / 2 - ticketRectLeft.height / 2
                    }
                },
                State {
                    name: "vanishedTicket"
                    PropertyChanges {
                        target: ticketRectLeft
                        x: 0
                        y: 0
                        scale: 200/300
                    }
                }

            ]
        }

//        onClicked: {
////            trialRect.animationDuration /= 0.95
//            trialRect.state = "down";
//            trialRect.state = "up";
//        }
//        onDoubleClicked: {
//            trialRect.state = "down";
//            trialRect.state = "winningTicket";
//        }

//        Text { text: ticketDrawExecutor.ticketDrawLeft.animationDuration }

        Connections {
            target: ticketDrawExecutor.ticketDrawLeft
            onTicketFlyThroughRequested: {
                ticketRectLeft.state = "down";
                ticketRectLeft.state = "up";
            }
        }

        Connections {
            target: ticketDrawExecutor.ticketDrawLeft
            onTicketWinningPositionRequested: {
                ticketRectLeft.state = "down";
                ticketRectLeft.state = "winningTicket";
            }
        }

        Connections {
            target: ticketDrawExecutor.ticketDrawLeft
            onTicketVanishRequested: {
                ticketRectLeft.state = "vanishedTicket";
            }
        }





        ListView {
            height: parent.height
            width: stageOfPrizeDrawing.ticketsHistoryListWidth
            x: appWindow.width - width

            model: ticketDrawExecutor.ticketDrawRight
            delegate: MouseArea {
                width: parent.width
                height: 100
                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 5
                    color: backgroundColor
                    TicketInnerVisual {
                        blockName: name
                        ticketName: ticketId
                        fontColor: textColor
                    }
                }
                onClicked: ticketDrawExecutor.ticketDrawRight.removeFromWinningTicketsHistory(index);
            }

            add: Transition {
                NumberAnimation { property: "opacity"; from: 0; duration: 500 }
            }
            remove: Transition {
                NumberAnimation { property: "x"; to: 2*width; duration: 500 }
            }
            displaced: Transition {
                NumberAnimation { properties: "y"; duration: 500 }
            }
        }

        Rectangle {
            id: ticketRectRight
            width: 300
            height: 150
            x: appWindow.width - stageOfPrizeDrawing.ticketsHistoryListWidth - stageOfPrizeDrawing.columnLayoutGap - width
            y: parent.height
            color: ticketDrawExecutor.ticketDrawRight.flyingTicketBgColor
            TicketInnerVisual {
                blockName: ticketDrawExecutor.ticketDrawRight.flyingTicketBlockName
                ticketName: ticketDrawExecutor.ticketDrawRight.flyingTicketId
                fontColor: ticketDrawExecutor.ticketDrawRight.flyingTicketTextColor
            }

            transitions: [
                Transition {
                    to: "up"
                    SequentialAnimation {
                        NumberAnimation { properties: "y"; duration: ticketDrawExecutor.ticketDrawRight.animationDuration }
                        ScriptAction { script: ticketDrawExecutor.ticketDrawRight.onFlyUpTicketStateAchieved(); }
                    }
                },
                Transition {
                    to: "winningTicket"
                    SequentialAnimation {
                        NumberAnimation {
                            properties: "y"
                            duration: 3.5 * ticketDrawExecutor.ticketDrawRight.animationDuration
                            easing {
                                type: Easing.OutElastic
                                amplitude: 1.0
                                period: 0.5
                            }
                        }
                        ScriptAction { script: ticketDrawExecutor.ticketDrawRight.onWinningTicketStateAchieved(); }
                    }
                },
                Transition {
                    to: "vanishedTicket"
                    SequentialAnimation {
                        NumberAnimation { properties: "x, y, scale"; duration: 500 }
                        ScriptAction { script: ticketDrawExecutor.ticketDrawRight.onVanishedTicketStateAchieved(); }
                    }
                }
            ]

            states: [
                State {
                    name: "up"
                    PropertyChanges {
                        target: ticketRectRight
                        y: -ticketRectRight.height
                    }
                },
                State {
                    name: "down"
                    PropertyChanges {
                        target: ticketRectRight
                        y: stageOfPrizeDrawing.height
                    }
                },
                State {
                    name: "winningTicket"
                    PropertyChanges {
                        target: ticketRectRight
                        y: stageOfPrizeDrawing.height / 2 - ticketRectRight.height / 2
                    }
                },
                State {
                    name: "vanishedTicket"
                    PropertyChanges {
                        target: ticketRectRight
                        x: appWindow.width - ticketRectRight.width
                        y: 0
                        scale: 200/300
                    }
                }

            ]
        }

        Connections {
            target: ticketDrawExecutor.ticketDrawRight
            onTicketFlyThroughRequested: {
                ticketRectRight.state = "down";
                ticketRectRight.state = "up";
            }
        }

        Connections {
            target: ticketDrawExecutor.ticketDrawRight
            onTicketWinningPositionRequested: {
                ticketRectRight.state = "down";
                ticketRectRight.state = "winningTicket";
            }
        }

        Connections {
            target: ticketDrawExecutor.ticketDrawRight
            onTicketVanishRequested: {
                ticketRectRight.state = "vanishedTicket";
            }
        }

        Rectangle {
            x: (appWindow.width - width)/2
            width: 250
            height: 150
            y: 20
            color: "white"
            opacity: 0.8
            Text {
                y: 20
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 28
                font.bold: true
                color: "brown"
                text: "Sorsolható\ndíjak:\n" + ticketDrawExecutor.remainingPrizesCount + " db"
            }
        }
    }
}