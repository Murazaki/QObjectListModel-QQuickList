import QtQuick 2.0
import QtQml.Models 2.1
import QtQuick.Controls 1.0
import TestComponents 1.0

ApplicationWindow {
    width: 300; height: 300
    Row {
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: row2.top
        Component {
            id: commonDelegate1

            Rectangle {
                id: rect

                clip:true

                property bool selected: false

                width: view.width
                implicitHeight: editor.implicitHeight + 10
                color: selected?"red":"white"
                border.color: "red"
                border.width: 2
                radius: 5
                TextInput {
                    id: editor
                    anchors.margins: 5
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    text: name // "edit" role of the model, to break the binding loop
                    onTextChanged: {
                        name = text; // set the name property of the data object
                    }
                }
                Menu {
                  id: myContextMenu
                  MenuItem { text: "Randomize"; onTriggered: setRandomName() }
                  MenuItem { text: "Remove"; onTriggered: testobject.model.removeAt(index) }
                }
                MouseArea {
                  id: longPressArea
                  anchors.fill: parent
                  acceptedButtons: Qt.LeftButton | Qt.RightButton
                  onClicked: {
                      if (mouse.button == Qt.RightButton)
                          myContextMenu.popup();
                      else
                          rect.selected = !rect.selected;
                  }
                }

                ListView.onAdd: SequentialAnimation {
                    PropertyAction { target: rect; property: "x"; value: -rect.width }
                    PropertyAnimation { target: rect; property: "height"; from: 0; to:rect.implicitHeight; duration: 100; easing.type: Easing.InOutQuad }
                    PropertyAnimation { target: rect; property: "x"; from: -rect.width; to:rect.x; duration: 250; easing.type: Easing.InOutQuad }
                }
                ListView.onRemove: SequentialAnimation {
                    PropertyAction { target: rect; property: "ListView.delayRemove"; value: true }
                    PropertyAnimation { target: rect; property: "x"; from: rect.x; to:rect.width; duration: 250; easing.type: Easing.InOutQuad }
                    PropertyAnimation { target: rect; property: "height"; from:rect.implicitHeight; to: 0; duration: 100; easing.type: Easing.InOutQuad }
                    PropertyAction { target: rect; property: "ListView.delayRemove"; value: false }
                }
            }
        }

        Component {
            id: commonDelegate2

            Rectangle {
                id: rect

                clip:true

                property bool selected: false

                width: view.width
                implicitHeight: editor.implicitHeight + 10
                color: selected?"red":"white"
                border.color: "red"
                border.width: 2
                radius: 5
                TextInput {
                    id: editor
                    anchors.margins: 5
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    text: edit.name // "edit" role of the model, to break the binding loop
                    onTextChanged: {
                        display.name = text; // set the name property of the data object
                    }
                }
                Menu {
                  id: myContextMenu
                  MenuItem { text: "Randomize"; onTriggered: display.setRandomName() }
                  MenuItem { text: "Remove"; onTriggered: testobject.model.removeAt(index) }
                }
                MouseArea {
                  id: longPressArea
                  anchors.fill: parent
                  acceptedButtons: Qt.LeftButton | Qt.RightButton
                  onClicked: {
                      if (mouse.button == Qt.RightButton)
                          myContextMenu.popup();
                      else
                          rect.selected = !rect.selected;
                  }
                }

                ListView.onAdd: SequentialAnimation {
                    PropertyAction { target: rect; property: "x"; value: -rect.width }
//                    PropertyAnimation { target: rect; property: "height"; from: 0; to:rect.implicitHeight; duration: 100; easing.type: Easing.InOutQuad }
                    PropertyAnimation { target: rect; property: "x"; from: -rect.width; to:rect.x; duration:250; easing.type: Easing.InOutQuad }
                }
                ListView.onRemove: SequentialAnimation {
                    PropertyAction { target: rect; property: "ListView.delayRemove"; value: true }
                    PropertyAnimation { target: rect; property: "x"; from: rect.x; to:rect.width; duration: 250; easing.type: Easing.InOutQuad }
//                    PropertyAnimation { target: rect; property: "height"; from:rect.implicitHeight; to: 0; duration: 100; easing.type: Easing.InOutQuad }
                    PropertyAction { target: rect; property: "ListView.delayRemove"; value: false }
                }
            }
        }
        spacing: 2
        ListView {
            id: view
            width: (parent.width - parent.spacing)/2
            height: parent.height
            clip: true
            model: testobject.listProperty
            delegate: commonDelegate1
            spacing: 2

            addDisplaced: Transition {
                    NumberAnimation { properties: "x,y"; duration: 100; easing.type: Easing.InOutQuad }
                }

            moveDisplaced: Transition {
                    NumberAnimation { properties: "x,y"; duration: 500; easing.type: Easing.InOutQuad }
                }

            removeDisplaced: Transition {
                    NumberAnimation { properties: "x,y"; duration: 100; easing.type: Easing.InOutQuad }
                }
        }
        ListView {
            width: (parent.width - parent.spacing)/2
            height: parent.height
            clip: true

            model: testobject.model
            delegate: commonDelegate2
            spacing: 2

            addDisplaced: Transition {
                    NumberAnimation { properties: "x,y"; duration: 100; easing.type: Easing.InOutQuad }
                }

            moveDisplaced: Transition {
                    NumberAnimation { properties: "x,y"; duration: 100; easing.type: Easing.InOutQuad }
                }

            removeDisplaced: Transition {
                    NumberAnimation { properties: "x,y"; duration: 100; easing.type: Easing.InOutQuad }
                }
        }
    }
    Flow {
        id: row2
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        Button {
            text: "appendOne";
            onClicked: testobject.appendOne()
        }
        Button {
            text: "appendList";
            onClicked: testobject.appendList(5)
        }
        Button {
            text: "prependOne";
            onClicked: testobject.prependOne()
        }
        Button {
            text: "replace";
            onClicked: testobject.replace()
        }
        Button {
            text: "removeAt";
            onClicked: testobject.removeAt()
        }
        Button {
            text: "removeAll";
            onClicked: testobject.removeAll()
        }
        Button {
            text: "removeOne";
            onClicked: testobject.removeOne()
        }
        Button {
            text: "takeAt";
            onClicked: testobject.takeAt()
        }Button {
            text: "takeFirst";
            onClicked: testobject.takeFirst()
        }
        Button {
            text: "takeLast";
            onClicked: testobject.takeLast()
        }
        Button {
            text: "move";
            onClicked: testobject.move()
        }
        Button {
            text: "swap";
            onClicked: testobject.swap()
        }
        Button {
            text: "insert";
            onClicked: testobject.insert()
        }
        Button {
            text: "eraseOne";
            onClicked: testobject.eraseOne()
        }
        Button {
            text: "eraseList";
            onClicked: testobject.eraseList()
        }
        Button {
            text: "removeFirst";
            onClicked: testobject.removeFirst()
        }
        Button {
            text: "removeLast";
            onClicked: testobject.removeLast()
        }
        Button {
            text: "appendPlusEqual";
            onClicked: testobject.appendPlusEqual(5)
        }
        Button {
            text: "appendPlus";
            onClicked: testobject.appendPlus(5)
        }
    }
}
