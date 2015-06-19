import QtQuick 2.2
import QtPositioning 5.2
import QtLocation 5.3

GridView {
    id: grid
    anchors.fill: parent
    cellWidth: 150
    cellHeight: 150

    model: ListModel {
        ListElement {
            name: "Test"
            portrait: "tux-manjaro.png"
        }
        ListElement {
            name: "Implement this method to return the texture with id. The default implementation returns 0."
            portrait: "tux-manjaro.png"
        }
        ListElement {
            name: "a"
            portrait: "tux-manjaro.png"
        }
        ListElement {
            name: "Implementthismethodtoreturnthetexturewithid. Thedefaultimplementationreturns0."
            portrait: "tux-manjaro.png"
        }
    }

    delegate: Rectangle {
        width: grid.cellWidth; height: grid.cellHeight

        Image {
            id: imageTop
            source: portrait
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            text: name
            horizontalAlignment: Text.AlignHCenter
            width: 100
            wrapMode: Text.Wrap
            maximumLineCount: 2
            elide: Text.ElideCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: imageTop.bottom
        }
    }
}

