import QtQuick 2.0
import Sailfish.Silica 1.0
import QtQuick.Layouts 1.1
import org.k_bsp.sailmusic 1.0
import Nemo.Notifications 1.0
import Nemo.DBus 2.0


import "../components"

Page {

    id: page
    property bool loggedIn



    SilicaFlickable {
        property bool loggedIn: false

        //        height: page.height

        anchors.fill: parent

        width: parent.width
        contentHeight: parent.height
        contentWidth: parent.width

        PullDownMenu {
            MenuItem {
                text: qsTr("Log in")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("LoginPage.qml"))
            }
        }


        PageHeader {
            id: header

            title: "Sail Music"
        }

        TextField {
            anchors {
                top: header.bottom
                topMargin: Theme.paddingLarge
            }

            id: searchField

            placeholderText: "Enter Artist/Track/Album name"
        }

        SilicaFlickable {
            anchors {
                top: searchField.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }

            clip: true

            contentHeight: contentLayout.height

            Column {
                id: contentLayout

                width: parent.width

                RadioList {
                    width: parent.width
                    height: 450

                    listName: "Radios"
                }

                //                PlaylistList {
                //                    width: parent.width
                //                    height: 450

                //                    radio: radioListModel
                //                    listName: "Radios"
                //                }

                //                PlaylistList {
                //                    width: parent.width
                //                    height: 450

                //                    radio: radioListModel
                //                    listName: "Recommended"
                //                }

            }

            VerticalScrollDecorator {}
        }

    }
    //        SilicaFlickable {
    //            id: playlists

    //            anchors {
    //                top: searchField.bottom
    //                bottom: parent.bottom
    //                left: parent.left
    //                right: parent.right
    //            }

    //            width: parent.width
    //            height: parent.height

    //            clip: true

    ////            ColumnLayout {
    //                anchors.fill: parent

    //    PlaylistList {
    //        id: radioPlaylist
    //        width: parent.width
    //        height: 500

    //        radio: radioListModel
    //        listName: "Radios"
    //    }

    //                PlaylistList {
    //                    id: continuePlaylist
    //                    anchors.top: radioPlaylist.bottom

    //                    width: parent.width
    //                    height: 500

    //                    radio: radioListModel
    //                    listName: "Radios"
    //                }

    //                PlaylistList {
    //                    anchors.top: continuePlaylist.bottom

    //                    width: parent.width
    //                    height: 500

    //                    radio: radioListModel
    //                    listName: "Radios"
    //                }
    ////            }

    //            VerticalScrollDecorator {
    //                flickable: playlists
    //            }
    //        }


    Notification {
        id: notification

        appName: "SailMusic"
        expireTimeout: 3000

        function showPopup(title, message, icn)
        {
            replacesId = 0
            previewSummary = title
            previewBody = message
            icon = icn
            publish()
        }
    }

    Connections {
        target: YaClient

        onError: {
            switch(error) {
            case YaClient.CaptchaRequired:
                notification.showPopup("Captcha required.", "Please log in again.", "image://theme/icon-lock-warning")
                console.log("Captcha required:() Try to log in again");
                break;
            case YaClient.InvalidCredentials:
                notification.showPopup("Invalid credentials.", "Please log in again.", "image://theme/icon-lock-warning")
                console.log("Invalid credentials! ");
                break;
            case YaClient.UndefinedError:
                notification.showPopup("I'm sorry but we have an undefined error...", "Shit happens:(", "image://theme/icon-lock-warning")
                console.log("Undefined error. " + YaClient.lastErrorString);
                break;
            }
        }

        onAuthorized: {
            loggedIn = true
            console.log('User ID: ' + userId)
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
//            console.log(radioListModel.currentIndex);
//            if (radioListModel.currentIndex >= 0)
//                console.log(radioListModel.currentTrack.coverUrl.replace("%%", "1000x1000"));
//            pageStack.pushAttached(Qt.resolvedUrl("TrackPage.qml"), {
//                                       "radio": radioListModel,
//                                       "currentCoverImg": radioListModel.currentIndex >= 0 ? radioListModel.currentTrack.coverUrl.replace("%%", "1000x1000") : "",
//                                       "currentSongTitle": radioListModel.currentIndex >= 0 ? radioListModel.currentTrack.title : ""
//                                   }
//                                   );
        }
    }
}
