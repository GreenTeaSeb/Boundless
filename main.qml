import QtQuick
import BoardCanvas 1.0
import QtQuick.Layouts
import QtQuick.Controls 6.3
import Qt5Compat.GraphicalEffects
import Qt.labs.platform
import QtQuick3D.Effects 6.0

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Fractal")

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
        text: `${(canvas.zoom + 20) * (100 - 0) / (20  + 20) + 0}% ${canvas.zoom}\n${canvas.offset}`
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
        text: `${mousearea.drawing}\n${canvas.brush_size}`
        anchors.right: parent.right
        anchors.top: parent.top
        font.pixelSize: 12
        anchors.rightMargin: 25
        font.family: "Arial"
        anchors.topMargin: 8;
        z: 2
    }

    ColorDialog{
        id: color_dialog
        currentColor: document.color
        color: palette.text

        onAccepted: canvas.color = color
    }

    ColumnLayout{
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 20
        z: 4
        IconImage {
            source: "qrc:icons/squiggle.svg"
            sourceSize.height: 30
            sourceSize.width: 30
            antialiasing: true
            color: palette.text
        }

        Slider{
            id: size_slider
            value: canvas.brush_size
            snapMode: RangeSlider.SnapAlways
            stepSize: 1
            wheelEnabled: true
            orientation: Qt.Vertical
            height: 200
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            width: 20
            from: 1
            to: 20

            background: IconImage{
                id: slider_bg
                source: "qrc:icons/triangle.svg"
                color: palette.mid

            }

            handle: Rectangle{
                y: size_slider.visualPosition * (size_slider.availableHeight - height)
                height: Math.min(Math.max(size_slider.value, 6), 20) + 4;
                width: Math.min(Math.max(size_slider.value, 6), 20) + 4;
                radius: width/2
                anchors.horizontalCenter: parent.horizontalCenter
                color: palette.button
                border.width: 1
                border.color: palette.dark
            }
            onMoved: {
                canvas.brush_size = value
            }
        }
        IconImage {
            source: "qrc:icons/squiggle_thin.svg"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            sourceSize.height: 20
            sourceSize.width: 20
            antialiasing: true
            color: palette.text
        }
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
                icon.color: palette.text
                icon.height: 20
                icon.width: 20
                checkable: true
                display: AbstractButton.IconOnly
                antialiasing: true
                icon.source: "qrc:icons/eraser.svg"
                Layout.fillHeight: true
                Layout.preferredWidth: height

                background: Rectangle {
                    color: parent.down ? "red" : (parent.hovered ? "blue" : "transparent")
                }
            }

            ToolSeparator{}

            Button{
                antialiasing: true
                Layout.fillHeight: true
                Layout.preferredWidth: height

                background: Rectangle {
                    color: canvas.color
                    border.width: 1
                    border.color: palette.text
                    implicitHeight: 25
                    implicitWidth: 25
                    radius: width/2
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                }

                onClicked: color_dialog.open()
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
                if(mouse.modifiers & Qt.ControlModifier){
                    // pan around
                    canvas.offset = Qt.point(prevX-mouse.x,prevY-mouse.y)
                    mousearea.cursorShape = Qt.BlankCursor

                }
                else if (mouse.modifiers & Qt.ShiftModifier){
                    //change brush size
                    canvas.change_size(Qt.point(mouse.x,mouse.y))
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
                canvas.zoom_to(wheel.angleDelta.y/ 120 , Qt.point(wheel.x,wheel.y));
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
