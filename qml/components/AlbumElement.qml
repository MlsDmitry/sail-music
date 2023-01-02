import QtQuick 2.0
import Sailfish.Silica 1.0
import org.k_bsp.sailmusic 1.0


Item {
    property bool backgroundClicked: false
    property string coverUrl: ""
    property string title: ""


    Rectangle {

        id: itemContainer
        anchors.fill: parent

        anchors.centerIn: parent
        color: Theme.darkPrimaryColor
        opacity: 0.7

        radius: 30

        //        z: 1

        Behavior on opacity {
            FadeAnimator {
                duration: 30
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                YaClient.setPlaylist(radio)
                radio.changeStation(station);
                radio.getTracks();
//                pageStack.push(Qt.resolvedUrl("../pages/PlaylistPage.qml"), { "playlistModel": radio });
                //                        pageStack.navigateForward(PageStackAction.Animated)
            }
            onPressed: {
                itemContainer.opacity = 1.0;
                cover.width = cover.width - 10;
                cover.height = cover.height - 10;
            }
            onReleased: {
                itemContainer.opacity = 0.7;
                cover.width = cover.width + 10;
                cover.height = cover.height + 10;

            }
        }


        Column {
            id: playlistContent

            anchors {
                fill: parent
                margins: Theme.paddingMedium
            }

            //            z: 1

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                //            anchors.centerIn: parent
                height: parent.height - radioLabel.height
                width: height

                source: coverUrl.replace("%%", "200x200")
            }


            Label {
                id: radioLabel

                anchors {
                    horizontalCenter: parent.horizontalCenter                }

                text: title
            }
        }

    }
}
