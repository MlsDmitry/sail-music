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

        //    contentHeight: height


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

            asynchronous: true

            source: currentCoverImg
        }

        Row {
            anchors {
                top: coverImage.bottom
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
                    iconUrl = YaClient.playState() === YaClient.PlayingState ? "image://theme/icon-m-play" : "image://theme/icon-m-pause";
                    if (radio.currentTrack) {
                        if (YaClient.playState() === YaClient.PlayingState) {
                            YaClient.pause();
                        } else if (YaClient.playState() === YaClient.PausedState) {
                            YaClient.continuePlay();
                        } else {
                            YaClient.play(radio.currentTrack.getDirectDownloadLink(radio.currentTrack.maxBitrate.toString()));
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
                        if (radio.currentIndex + 1 == radio.rowCount() - 1) {
                            radio.getTracks();
                        } else {
                            radio.setIndex(radio.currentIndex + 1);
                        }
                    }
                }
            }
            Connections {
                target: radio

                onTracksReceived: {
                    radio.sendFeedbackRequest(radio.RadioStarted)
                    if (radio.rowCount() > 0 && radio.currentIndex === -1) {
                        radio.setIndex(0);
                    }
                    checkSetCurrenTrack();
                }

                onCurrentTrackUpdated: {
                    console.log("Here!!!")
                    checkSetCurrenTrack();
                }

                onFeedbackReceived: {
                    radio.prepareCurrentTrackToPlay();
                }

                onCurrentTrackLinkReady: {
//                    YaClient.play(url);

                }
            }

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

        radio.prepareCurrentTrackToPlay()
        console.log("Done!")
    }

    onStatusChanged: {
        console.log("Index is " + radio.currentIndex + " Page status " + status + " === " + PageStatus.Active);
        //        if (status == PageStatus.Active) {
        //            checkSetCurrenTrack();
        //        }
    }
}
