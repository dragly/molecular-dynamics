import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    RadialGradient {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: Qt.rgba(0.4, 0.4, 0.4, 0.4) }
            GradientStop { position: 0.2; color: Qt.rgba(0.4, 0.4, 0.4, 0.5) }
            GradientStop { position: 0.35; color: Qt.rgba(0.4, 0.4, 0.4, 0.9) }
            GradientStop { position: 0.4; color: Qt.rgba(0.4, 0.4, 0.4, 1.0) }
            GradientStop { position: 0.45; color: Qt.rgba(0.4, 0.4, 0.4, 0.7) }
            GradientStop { position: 0.48; color: Qt.rgba(0.4, 0.4, 0.4, 0.9) }
            GradientStop { position: 0.5; color: Qt.rgba(0.4, 0.4, 0.4, 0.0) }
        }
    }

    MouseArea {
        id: navigatorArea
        property point startPosition
        property real forwardSpeed: 0.0
        property real rightSpeed: 0.0
        property real sensitivity: 0.0005
        anchors.fill: parent

        onPressed: {
            startPosition.x = mouse.x
            startPosition.y = mouse.y
        }

        onPositionChanged: {
            var xDiff = mouse.x - startPosition.x
            var yDiff = mouse.y - startPosition.y
            forwardSpeed = yDiff * sensitivity;
            rightSpeed = xDiff * sensitivity;
        }

        onReleased: {
            forwardSpeed = 0.0
            rightSpeed = 0.0
        }

        Timer {
            id: moveTimer
            repeat: true
            running: true
            interval: 16
            onTriggered: {
                var forwardVector = myCamera.center.minus(myCamera.eye);
                var upVector = myCamera.upVector;
                var a = forwardVector;
                var b = upVector;
                var rightVector = Qt.vector3d(a.y * b.z - a.z * b.y,
                                              a.z * b.x - a.x * b.z,
                                              a.x * b.y - a.y * b.x); // cross product, a x b

                var translation = Qt.vector3d(0,0,0)
                translation = translation.plus(forwardVector.times(-navigatorArea.forwardSpeed))
                translation = translation.plus(rightVector.times(navigatorArea.rightSpeed))
                myCamera.center = myCamera.center.plus(translation);
                myCamera.eye = myCamera.eye.plus(translation);
            }
        }
    }
}
