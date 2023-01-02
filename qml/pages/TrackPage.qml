import QtQuick 2.0
import Sailfish.Silica 1.0
import org.k_bsp.sailmusic 1.0

import "../components"

Page {
    id: page

    property bool waitingPlay: false
    property var radio
    property string currentCoverUrl: ""
    property string currentSongTitle: ""

    SilicaFlickable {
        anchors.fill: parent

        Label {
            id: songTitle

            anchors.horizontalCenter: parent.horizontalCenter
            text: currentSongTitle

            font {
                pixelSize: Theme.fontSizeLarge
                italic: true
            }
            y: parent.height / 4

        }

        Image {
            id: coverImage

            anchors {
                top: songTitle.bottom
                horizontalCenter: parent.horizontalCenter
                margins: Theme.paddingLarge
            }

            width: 500
            height: 500

            source: currentCoverUrl.replace("%%", "1000x1000")
        }

        Item {
            id: progressBar

            anchors {
                top: coverImage.bottom
                horizontalCenter: parent.horizontalCenter
                margins: Theme.paddingLarge
            }
            height: Theme.fontSizeLarge
            width: parent.width

            Rectangle{
                id: playingProgressItem
                width: 0
                height: Theme.dp(3)
                color: Theme.primaryColor
                anchors{
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    margins: Theme.paddingLarge
                }
            }

            MouseArea{
                id: seekArea
                anchors.fill: parent
                onClicked: {
                    var current_poz = (mouseX-x)/width
                    YaClient.seek(radio.currentTrack.duration * current_poz)
                }
            }
        }

        Row {
            anchors {
                top: progressBar.bottom
                horizontalCenter: parent.horizontalCenter
                margins: Theme.paddingLarge
            }

            MediaButton {
                iconUrl: "image://theme/icon-m-media-rewind"

                onMediaClicked: {
                    yaclientSM.change_song(-1)
                }
            }

            MediaButton {
                id: playButton
                iconUrl: stPlaying.active ? "image://theme/icon-m-pause" : "image://theme/icon-m-play"

                onMediaClicked: {
                    if (stPlaying.active) {
                        yaclientSM.pause();
                    } else {
                        if (YaClient.currentPlaylist.currentTrack.getMaxBitrateAvailable() === "0") {
                            yaclientSM.waitingPlay = true;
                            console.log(yaclientSM.waitingPlay);
                            yaclientSM.playlistSM.load_track_info();
                        } else {
                            yaclientSM.play();
                        }
                    }
                }
            }

            MediaButton {
                iconUrl: "image://theme/icon-m-media-forward"

                onMediaClicked: {
                    yaclientSM.change_song(1)
                }
            }
        }
    }


    function checkSetCurrenTrack()
    {

        var cover = YaClient.currentPlaylist.currentTrack.coverUrl;
        var title = YaClient.currentPlaylist.currentTrack.title;

        if (cover && currentCoverUrl !== cover) {
            console.log("Set cover", currentCoverUrl, cover)
            currentCoverUrl = cover;
        }
        if(title && currentSongTitle !== title) {
            console.log("Set title", currentSongTitle, title)
            currentSongTitle = title;
        }
    }

    Connections {
        target: yaclientSM

        onReadyChanged: {
            if (yaclientSM.waitingPlay && yaclientSM.ready) {
                console.log("shit...");
                yaclientSM.waitingPlay = false;
                yaclientSM.play();
            } else {
                checkSetCurrenTrack();
            }
        }
    }

    Connections {
        target: YaClient.currentPlaylist

        ignoreUnknownSignals: true

        onCurrentTrackUpdated: {
            console.log("Current track updated");
            checkSetCurrenTrack();
        }
    }



    onStatusChanged: {
        if (status == PageStatus.Active && (yaclientSM.ready || yaclientSM.playing || yaclientSM.paused)) {
            checkSetCurrenTrack();
            console.log("ready");
//            console.log(YaClient.currentPlaylist.currentTrack)
        }
    }

//        console.log("Index is " + radio.currentIndex + " Page status " + status + " === " + PageStatus.Active);
        //        if (status == PageStatus.Active) {
        //            checkSetCurrenTrack();
        //        }
}
