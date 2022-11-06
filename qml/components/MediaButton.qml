import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {
    property string iconUrl: ""

    signal mediaClicked

    width: Theme.itemSizeExtraLarge
    height: Theme.itemSizeExtraLarge
    color: "transparent"

    MouseArea {
        anchors.fill: parent
        onClicked: mediaClicked()
    }

    Image {
        anchors.fill: parent
        source: iconUrl
    }

}
