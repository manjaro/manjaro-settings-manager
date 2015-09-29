import QtQuick 2.2
import QtPositioning 5.2
import QtLocation 5.3

GridView {
    id: grid
    anchors.fill: parent
    cellWidth: 150
    cellHeight: 150

    signal itemClicked(string msg)

    model: ListModel {
        ListElement {
            name: "Kernel"
            portrait: "tux-manjaro.png"
            module: "msm_kernel"
        }
        ListElement {
            name: "Keyboard"
            portrait: "tux-manjaro.png"
            module: "msm_keyboard"
        }
        ListElement {
            name: "Language Packages"
            portrait: "tux-manjaro.png"
            module: "msm_language_packages"
        }
        ListElement {
            name: "Locale"
            portrait: "tux-manjaro.png"
            module: "msm_locale"
        }
        ListElement {
            name: "MHWD"
            portrait: "tux-manjaro.png"
            module: "msm_mhwd"
        }
        ListElement {
            name: "Timedate"
            portrait: "tux-manjaro.png"
            module: "msm_timedate"
        }
        ListElement {
            name: "Users"
            portrait: "tux-manjaro.png"
            module: "msm_users"
        }
    }

    delegate: Rectangle {
        width: grid.cellWidth; height: grid.cellHeight

        Image {
            id: icon
            source: portrait
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            text: name
            horizontalAlignment: Text.AlignHCenter
            width: 100
            wrapMode: Text.Wrap
            maximumLineCount: 2
            elide: Text.ElideMiddle
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: icon.bottom
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                grid.itemClicked(module)
            }
        }
    }
}

