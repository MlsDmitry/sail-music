import QtQuick 2.0
import Sailfish.Silica 1.0
import org.k_bsp.sailmusic 1.0
import Nemo.Notifications 1.0
import Nemo.DBus 2.0


import "../components"

Page {

    id: page
    property bool loggedIn


    RadioListModel {
        id: radioListModel

    }

    SilicaFlickable {
        property bool loggedIn: false

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
        RadioList {
            width: page.width
            height: page.height / 5.5
            radio: radioListModel
        }


    }
    onStatusChanged: {
        if (status === PageStatus.Active) {
            console.log(radioListModel.currentIndex);
            if (radioListModel.currentIndex >= 0)
                console.log(radioListModel.currentTrack.coverUrl.replace("%%", "1000x1000"));
            pageStack.pushAttached(Qt.resolvedUrl("TrackPage.qml"), {
                                       "radio": radioListModel,
                                       "currentCoverImg": radioListModel.currentIndex >= 0 ? radioListModel.currentTrack.coverUrl.replace("%%", "1000x1000") : "",
                                       "currentSongTitle": radioListModel.currentIndex >= 0 ? radioListModel.currentTrack.title : ""
                                   }
                                   );
        }
    }
}
