import QtQuick
import BoardCanvas 1.0
import QtQuick.Controls

Button{

    SystemPalette { id: palette; colorGroup: SystemPalette.Active }

    property string shortcut_key
    property int tool_id
    icon.height: 20
    icon.width: 20
    antialiasing: true
    background: Rectangle {
        color: parent.down ? palette.dark : (parent.hovered ? palette.mid : "transparent")
    }


    onPressed: parent.action.trigger()

    action: Action{
        shortcut: shortcut_key
        onTriggered: {
            canvas.current_tool = tool_id
        }
    }



}
