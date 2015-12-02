import QtQuick 2.3
import QtQuick.Controls 1.2

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 600
    height: 500

    signal mapClick(var e)

    Item {
        id: topColumn

        height: 60

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        Text {
            id: descriptionText
            text: qsTr("Click the map for a location of pick a pre defined location in the drop down")
            font.pixelSize: 12

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 10
            anchors.leftMargin: 10
        }

        ComboBox {
            id: locationBox

            anchors.top: descriptionText.bottom
            anchors.left: parent.left
            anchors.topMargin: 5
            anchors.leftMargin: 10
        }

        Button {
            id: confirmButton

            text: qsTr("Confirm")

            anchors.top: descriptionText.bottom
            anchors.right: descriptionText.right
            anchors.topMargin: 5
            anchors.leftMargin: 10
        }
    }


    MouseArea {
        onClicked: mainWindow.mapClick()
        Image {
            id: mapImage

            fillMode: Image.PreserveAspectFit

            anchors.top: topColumn.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: bottomColumn.top
            anchors.topMargin: 10
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            anchors.bottomMargin: 10

            source: "qrc:/qtquickplugin/images/template_image.png"
        }
    }

    Item {
        id: bottomColumn

        height: 80

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        ScrollView {
            id: logView
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            anchors.bottomMargin: 10

            Text {
                id: logBox
                text: qsTr("Logghjsdd\nddd\ndddddd\n\n\n\n\nddddddddddd\nddddddddddd")
                textFormat: Text.PlainText
            }
        }
    }
}
