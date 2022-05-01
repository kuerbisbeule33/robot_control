#include "conversion.h"
#include <QtMath>

const double armLengthHorizontal = 1.0;
const double armLengthVertical = 1.0;
const double gripLength = 1.0;

double lawOfCosinus(double a, double b, double c){
    return qAcos((a*a + b*b - c*c) / (2*a*b));
}

Point angleToCoordinates(quint16 rotationAngle, quint16 tiltAngle, quint16 horizontalAngle){
    //x nach rechts
    //y nach oben
    //z nach vorne
    Point point;
    double rotation = static_cast<double>(rotationAngle);
    double tilt = static_cast<double>(tiltAngle);
    double horizontal = static_cast<double>(horizontalAngle);
    double ditanzXZ = qCos(tilt) * armLengthVertical + qCos(180-tilt-horizontal) * armLengthHorizontal + gripLength;
    point.x = qCos(rotation) * ditanzXZ;
    point.y = qSin(tilt) * armLengthVertical - qSin(180-tilt-horizontal) * armLengthHorizontal;
    point.z = -qSin(rotation) * ditanzXZ;
    return point;
}

void CoordinatesToAngle(Point point, quint16& rotationAngle, quint16& tiltAngle, quint16& horizontalAngle){
    //2 teile der Koordinaten von https://appliedgo.net/roboticarm/
    //x nach rechts
    //y nach oben
    //z nach vorne
    rotationAngle = static_cast<quint16>(qAtan2(-point.z, point.x));
    double distOnGround = qSqrt(point.x*point.x + point.z*point.z) - gripLength;
    double dist2Dimensions = qSqrt(point.y*point.y + distOnGround*distOnGround);
    quint16 helpAngle1 = qAtan2(point.y, distOnGround);
    quint16 helpAngle2 = static_cast<quint16>(lawOfCosinus(dist2Dimensions, armLengthVertical, armLengthHorizontal));
    tiltAngle = helpAngle1 + helpAngle2;
    horizontalAngle = static_cast<quint16>(lawOfCosinus(armLengthVertical, armLengthHorizontal, dist2Dimensions));
}
