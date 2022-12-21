import QtQuick 2.0
import Sailfish.Silica 1.0
import org.k_bsp.sailmusic 1.0

import "../components"

Page {
    id: page

    property var radio
    property string currentCoverImg: ""
    property string currentSongTitle: ""

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Log in")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("LoginPage.qml"))
            }
        }

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

            source: currentCoverImg.replace("%%", "1000x1000")
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
                    if (radio.currentTrack) {
                        playButton.iconUrl = "image://theme/icon-m-play"
                        YaClient.stop();
                        radio.setIndex(radio.currentIndex - 1)
                    }
                }
            }
            MediaButton {
                id: playButton
                iconUrl: YaClient.playState() === YaClient.PlayingState ? "image://theme/icon-m-pause" : "image://theme/icon-m-play"

                onMediaClicked: {
                    if (radio.currentTrack) {
                        if (YaClient.playState() === YaClient.PlayingState) {
                            YaClient.pause();
                        } else if (YaClient.playState() === YaClient.PausedState) {
                            YaClient.continuePlay();
                        } else {
                            console.log("Track started");
                            radio.prepareCurrentTrackToPlay ();
                        }
                    }
                }
            }
            MediaButton {
                iconUrl: "image://theme/icon-m-media-forward"

                onMediaClicked: {
                    if (radio.currentTrack) {
                        playButton.iconUrl = "image://theme/icon-m-play"
                        YaClient.stop();
                        radio.sendFeedbackRequest(3)
                        console.log("radio.rowCount()" << radio.rowCount())
                        if (radio.currentIndex + 1 === radio.rowCount()) {
                            radio.getTracks();
                        } else {
                            radio.setIndex(radio.currentIndex + 1);
                        }
                    }
                }
            }

//            Connections {
//                target: radio

//                onTracksReceived: {
//                    console.log("Tracks received. total tracks: " + radio.rowCount());
//                    if (radio.rowCount() > 0 && radio.currentIndex === -1) {
//                        radio.sendFeedbackRequest(radio.RadioStarted);
//                        radio.setIndex(0);
//                        console.log("Set current index to 0. Radio Started");
//                    }

//                    for (var i = radio.currentIndex; i < radio.rowCount(); i++) {
//                        console.log(radio.get(i).title);
//                    }

//                }

//                onCurrentTrackUpdated: {
//                    console.log("Here!!!")
//                    checkSetCurrenTrack();
//                }

//                onFeedbackReceived: {
//                    console.log("Preparing current track...");
//                }

//                onCurrentTrackLinkReady: {
//                    console.log("Current track is ready to play");
//                    //                    console.log(url);
//                    YaClient.play(url);
//                    radio.sendFeedbackRequest(1)

//                }
//            }

//            Connections {
//                target: YaClient

//                onAudioProgress: {
////                    console.log((progressBar.width - Theme.paddingLarge * 2) * (position / radio.currentTrack.duration));
//                    playingProgressItem.width = (progressBar.width - Theme.paddingLarge * 2) * (position / radio.currentTrack.duration);
//                }

//                onAudioStateChanged: {
//                    console.log(state + ":" + YaClient.PlayingState)
//                    if (state === YaClient.PlayingState) {
//                        playButton.iconUrl = "image://theme/icon-m-pause"
//                    } else if (state === YaClient.PausedState) {
//                        playButton.iconUrl = "image://theme/icon-m-play"
//                    }
//                }
//            }

//            Connections {
//                target: YaClient.player

//                onMediaStatusChanged: {
//                    console.log(radio.currentTrack.totalPlayedMs, radio.currentTrack.duration);
//                    if (status === YaClient.EndOfMedia && radio.currentTrack.duration <= radio.currentTrack.totalPlayedMs) {
//                        YaClient.stop();
//                        radio.sendFeedbackRequest(2);
//                        var prevIndex = radio.currentIndex;

//                        if (radio.currentIndex + 1 === radio.rowCount()) {
//                            radio.getTracks();
//                        } else {
//                            radio.setIndex(prevIndex + 1);
//                            radio.prepareCurrentTrackToPlay();
//                        }
//                    }
//                }
//            }

        }
    }

    function checkSetCurrenTrack()
    {
        if (radio.currentIndex === -1)
            return;

        var cover = radio.currentTrack.coverUrl.replace("%%", "1000x1000");
        var title = radio.currentTrack.title;

        if (currentCoverImg !== cover) {
            currentCoverImg = cover;
        }
        if(currentSongTitle !== title) {
            currentSongTitle = title;
        }

        //        radio.prepareCurrentTrackToPlay()
        //        console.log("Preparing current track...");
        console.log("Done!")
    }

    onStatusChanged: {
//        console.log("Index is " + radio.currentIndex + " Page status " + status + " === " + PageStatus.Active);
        //        if (status == PageStatus.Active) {
        //            checkSetCurrenTrack();
        //        }
    }
}
