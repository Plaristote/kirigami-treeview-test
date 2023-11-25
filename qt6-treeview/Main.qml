import QtQuick
import QItemModelTest

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    TreeView {
        anchors.fill: parent
        //clip: true
        //anchors.centerIn: parent
        model: rootFolder
        //implicitWidth: 500
        //implicitHeight: 100
        delegate: Rectangle {
            required property string displayName
            implicitWidth: leTexte.width + 15
            implicitHeight: 50
            color: "red"
            Text {
              id: leTexte
              anchors.centerIn: parent
              text: displayName
            }
        }
    }
    /*
    Column {
        anchors.centerIn: parent
        Repeater {
            model: itemModelTest
            delegate: Rectangle {
                implicitWidth: 500
                implicitHeight: 50
                Text {
                  anchors.centerIn: parent
                  text: model.displayName
                }
            }
        }
    }
    */
}
