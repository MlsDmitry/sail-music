import QtQuick 2.0
import Sailfish.Silica 1.0
import org.k_bsp.sailmusic 1.0


Item {
    property var playlist

    BusyIndicator {
             size: BusyIndicatorSize.Large
             anchors.centerIn: parent
   }

    SilicaListView {
        id: tracksView
        anchors.fill: parent

        spacing: 50

        model: playlist


        delegate: Item {

            Component.onCompleted: {
//                if (playlist.currentTrack)
//                    console.log("State: " + playlist.currentTrack.state);
            }

            width: tracksView.width - 10
            height: 250


            Rectangle {
                color: Theme.backgroundGlowColor

                width: parent.width

                anchors {
                    fill: parent
                    rightMargin: 10
                    margins: Theme.paddingSmall
                }

                border {
                    width: 2
                    color: Theme.presenceColor(12)
                }

                Connections {
                    target: playlist

                    onDataChanged: {
//                        console.log('Data Changed! topLeft: ' + topLeft.row);
                    }
                }

//                Connections {
//                    target: YaClient

//                    onAudioStateChanged: {
//                        console.log(playlist.getCurrentTrackId(), playlist.get(index).id)
//                        if (playlist.getCurrentTrackId() === playlist.get(index).id) {
//                            playButton.visible = state === YaClient.PlayingState;
//                            playButton.iconUrl = state === YaClient.PlayingState ? "image://theme/icon-m-play" :  "image://theme/icon-m-pause";
//                        }
//                    }
//                }
//                Connections {
//                    target: playlist

//                    onCurrentTrackLinkReady: {
//                        console.log("Here is link... " + url);
//                        playButton.visible = true;
//                    }
//                }

                MouseArea {
                    id: mouseArea

                    z: 1

                    anchors.fill: parent

                    width: parent.width
                    height: parent.height

                    onClicked: {
//                        YaClient.currentPlaylist = playlist;
//                        YaClient.currentPlaylist.setIndex(index)
//                        YaClient.currentPlaylist.prepareCurrentTrackToPlay();
//                        console.log("My index is: " + index);

                    }
                }

                Image {
                    id: trackImage

                    width: 250
                    height: 250

                    anchors {
                        left: parent.left
                        top: parent.top
                        bottom: parent.bottom
                        margins: 5
                    }

                    source: coverUrl.replace("%%", "400x400")

                    MediaButton {
                        id: playButton

                        z: 2

                        iconUrl: "image://theme/icon-m-play"

                        anchors.centerIn: parent
                        width: Theme.itemSizeLarge
                        height: Theme.itemSizeLarge

                        visible: false

                        onMediaClicked: {
//                            var track = playlist.get(index)
//                            YaClient.play(track.getDirectDownloadLink(track.getMaxBitrateAvailable()))
                        }
                    }
                }


                TextArea {
                    id: trackLabel

                    anchors {
                        left: trackImage.right
                        top: parent.top
                        margins: Theme.paddingSmall
                        //                        right: parent.right

                        //                        verticalCenter: parent.verticalCenter
                    }

                    readOnly: true

                    width: parent.width - trackImage.width

                    text: title
                }

                IconButton {
                    property bool isFavorite: false

                    z: 2

                    anchors {
                        bottom: parent.bottom
                        left: trackImage.right

                        leftMargin: Theme.paddingSmall
                        bottomMargin: Theme.paddingSmall
                    }

                    icon.source: isFavorite ? 'image://theme/icon-m-favorite-selected' : 'image://theme/icon-m-favorite'

                    onClicked:  {
                        isFavorite = !isFavorite;
                    }
                }

                IconButton {
                    id: addPlaylistButton

                    z: 2

                    anchors {
                        right: parent.right
                        bottom: parent.bottom

                        rightMargin: Theme.paddingSmall
                        bottomMargin: Theme.paddingSmall
                    }

                    icon.source: 'image://theme/icon-m-add'

                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("../pages/SelectPlaylistPage.qml"))
                    }

                }

            }

        }


        VerticalScrollDecorator {}
    }

}
