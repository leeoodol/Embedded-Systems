import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "Text Processing Example"

    Column {
        anchors.centerIn: parent
        spacing: 20

        TextInput {
            id: inputText
            width: 200
            placeholderText: "Enter text here..."
        }

        Button {
            text: "Process Text"
            onClicked: backend.processText(inputText.text)
        }

        Text {
            text: "Processed: " + backend.processedText
        }
    }
}
