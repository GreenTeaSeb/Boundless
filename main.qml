import QtQuick
import BoardCanvas 1.0
import QtQuick.Layouts
import QtQuick.Controls 6.3
import Qt5Compat.GraphicalEffects

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    SystemPalette { id: palette; colorGroup: SystemPalette.Active }

    Text {
        id: cur_cord
        y: 456
        text: 'X: Y:'
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        color:  palette.text
        font.pixelSize: 12
        anchors.leftMargin: 25
        anchors.bottomMargin: 8
        z: 2
    }

    Text {
        id: offset_label
        y: 456
        color:  palette.text
        text: `${Math.round(canvas.zoom/20 * 100)}% ${canvas.zoom}\n${canvas.offset}`
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        font.pixelSize: 12
        anchors.rightMargin: 25
        font.family: "Arial"
        anchors.bottomMargin: 8
        z: 2
    }
    Text {
        id: zoomto
        color:  palette.text
        text: mousearea.drawing
        anchors.right: parent.right
        anchors.top: parent.top
        font.pixelSize: 12
        anchors.rightMargin: 25
        font.family: "Arial"
        anchors.topMargin: 8;
        z: 2
    }


    ToolBar {
        id: toolbar
        height: 45
        z: 3
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        background: Rectangle{
            color: palette.button
            radius: 8
            border.color: palette.dark
            border.width: 1
        }

        RowLayout{
            anchors.fill: parent
            Button{
                icon.height: 20
                icon.width: 20
                checkable: true
                display: AbstractButton.IconOnly
                antialiasing: true
                icon.source: "qrc:icons/pen.svg"
                Layout.fillHeight: true
                Layout.preferredWidth: height
                icon.color: palette.text


                background: Rectangle {
                    color: parent.down ? "red" : (parent.hovered ? "blue" : "transparent")
                }

            }
            Button{
                icon.height: 20
                icon.width: 20
                checkable: true
                display: AbstractButton.IconOnly
                antialiasing: true
                icon.source: "qrc:icons/eraser.svg"
                Layout.fillHeight: true
                Layout.preferredWidth: height
                icon.color: palette.text
                background: Rectangle {
                    color: parent.down ? "red" : (parent.hovered ? "blue" : "transparent")
                }
            }

        }

    }
    DropShadow{
        anchors.fill: toolbar
        radius: 8
        samples: radius*2 + 1
        color: '#40000000'
        source: toolbar
        z: 2
    }

    Slider{
        x: 543
        width: 107
        height: 47
        anchors.verticalCenter: parent.verticalCenter
        rotation: 90
        anchors.right: parent.right
        anchors.rightMargin: -10
        from: 1
        to: 10
    }
    BoardCanvas{
        id: canvas
        anchors.fill: parent
        fillColor: palette.window
        color: palette.text
        MouseArea{
            id: mousearea
            property int prevX
            property int prevY
            property bool drawing: false
            anchors.fill: parent
            cursorShape: Qt.CrossCursor
            onPositionChanged:  {
                if(mouse.modifiers){
                    canvas.offset = Qt.point(prevX-mouse.x,prevY-mouse.y)
                    mousearea.cursorShape = Qt.BlankCursor
                }
                else{
                    // draw
                    canvas.draw_line(Qt.point(mouse.x,mouse.y))
                    canvas.last_point = Qt.point(mouse.x,mouse.y)
                }
                cur_cord.text = `prev-X: ${prevX} prev-Y: ${prevY}\nX: ${mouse.x} Y: ${mouse.y}\nDiff X: ${prevX-mouse.x} Y: ${prevY-mouse.y}`

            }
            onWheel: {
//                canvas.zoom += (wheel.angleDelta.y / 120) * 0.2
                canvas.zoom_to(wheel.angleDelta.y/ 120 , Qt.point(wheel.x,wheel.y));
                console.log("zoom")
            }

            onPressed: {
                prevX = mouse.x
                prevY = mouse.y
                canvas.last_point = Qt.point(mouse.x,mouse.y)
                if(!mouse.modifiers){
                    drawing = true
                    canvas.draw_begin();
                }
            }
            onReleased: {
                mousearea.cursorShape = Qt.CrossCursor
                if(drawing == true){
                    canvas.draw_end()
                    drawing = false;
                }
            }


        }
    }






}
