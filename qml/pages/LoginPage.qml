import QtQuick 2.0
import Sailfish.Silica 1.0
import org.k_bsp.sailmusic 1.0


Dialog {

    id: loginDialog

    allowedOrientations: Orientation.Portrait

    canAccept: usernameField.text &&
               passwordField.text &&
               usernameField.acceptableInput &&
               passwordField.acceptableInput


    Column {
        width: parent.width

        DialogHeader {
            id: header

            acceptText: "Log in"
            cancelText: "Cancel"
            dialog: loginDialog
        }

        anchors.fill: parent
        TextField {
            id: usernameField

            width: parent.width

            text: YaClient.lastUsername
            placeholderText: "username (email)"
            label: "Username"

            validator: RegExpValidator {
                regExp: new RegExp("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$")
            }
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.enabled: text && acceptableInput
            EnterKey.onClicked: passwordField.forceActiveFocus()
        }

        PasswordField {
            id: passwordField

            text: YaClient.lastPassword
            placeholderText: "password"
            label: "Password"

            validator: RegExpValidator { regExp: /^.{1,}$/ }
        }

        Image {
            id: captchaImage

            width: parent.width
            height: Theme.iconSizeExtraLarge

            visible: YaClient.lastError === YaClient.CaptchaRequired

            source: YaClient.captchaUrl
        }


        TextField {
            id: captchaAnswer

            visible: YaClient.lastError === YaClient.CaptchaRequired
            placeholderText: "captcha answer"
        }

    }

    onAccepted: {
        YaClient.lastUsername = usernameField.text
        YaClient.lastPassword = passwordField.text

        YaClient.requestAuth(usernameField.text, passwordField.text, captchaAnswer.text);
    }

}
