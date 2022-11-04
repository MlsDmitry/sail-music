import QtQuick 2.0
import Sailfish.Silica 1.0
import org.k_bsp.sailmusic 1.0

import "../components"

Page {
    id: page

    property var radio

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Log in")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("LoginPage.qml"))
            }
        }

        //    contentHeight: height


        Column {
            id: mainView

            Button {
                text: 'get tracks!'

                onClicked: {
                    radio.sendTracksRequest()

                }
            }

            Button {
                text: "Play!"
                onClicked: {
                    radio.setIndex(0);
                    radio.prepareCurrentTrackToPlay();

                }
            }

            Label {
                id: songTitle
            }

            Image {
                id: coverImage

                asynchronous: true
            }

            Connections {
                target: radio

                onTracksReceived: {
                    radio.sendFeedbackRequest(radio.RadioStarted)
                    if (radio.rowCount() > 0) {
                        radio.setIndex(0);
                    }
                }

                onRowsInserted: {

                }

                onFeedbackReceived: {
                    radio.prepareCurrentTrackToPlay();
                }

                onCurrentTrackLinkReady: {
                    console.log("Playing song from " + url)
                    YaClient.play(url);

                }
            }

        }
    }
    onStatusChanged: {
        console.log("Index is " + radio.currentIndex + " Page status " + status + " === " + PageStatus.Active);
        if (status == PageStatus.Active && radio.currentIndex !== -1) {
            console.log(radio.get(radio.currentIndex).coverUrl.replace("%%", "1000x1000"));
            var coverUrl = radio.get(radio.currentIndex).coverUrl.replace("%%", "1000x1000");
            coverImage.source = coverUrl;
            songTitle.text = radio.get(radio.currentIndex).title;
        }
    }
}
