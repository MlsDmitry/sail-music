import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import Nemo.DBus 2.0
import org.k_bsp.sailmusic 1.0

Page {
    property bool loggedIn: false

    allowedOrientations: Orientation.Portrait

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
        }
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("Log in")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("LoginPage.qml"))
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("UI Template")
            }
            Label {
                x: Theme.horizontalPageMargin
                text: qsTr("Hello Sailors")
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }
        }
    }
}
