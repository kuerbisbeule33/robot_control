#include "kinematics.h"
#include <QtMath>

//Längen der einzelnen Gelenke
const double  L1 = 92.0; //mm
const double  L2 = 135.0;
const double  L3 = 147.0;
const double  L4 = 87.0;

double lawOfCosinus(double a, double b, double c){
    return qAcos((a*a + b*b - c*c) / (2*a*b));
}

Point forwardKinematics(Angles angles){
    Point result;
    angles.q1 = angles.q1 * M_PI/180; // umrechnen in Radianten
    angles.q2 = angles.q2 * M_PI/180;
    angles.q3 = angles.q3 * M_PI/180;

    result.x = (qCos(angles.q1)*(qCos(angles.q2+angles.q3)*L3 + qCos(angles.q2)*L2))+(L4*qCos(angles.q1));
    result.y = (qSin(angles.q1)*(qCos(angles.q2+angles.q3)*L3 + qCos(angles.q2)*L2))+(L4*qSin(angles.q1));
    result.z = (L1+qSin(angles.q2)*L2+qSin(angles.q2+angles.q3)*L3);
    return result;
}

Angles reverseKinematics(Point point){
    Angles result;
    // Erster Winkel berechnen
    result.q1 = qAtan2(point.y,point.x);
    //Werte für Gelenk 4 berechnen
    Point jointFour;
    jointFour.x = point.x - L4*qCos(result.q1);
    jointFour.y = point.y - L4*qSin(result.q1);
    jointFour.z = point.z;

    //Dritter Winkel berechnen
    double dist_1_4;
    dist_1_4 = jointFour.z - L1;
    double xy_4;
    xy_4 = qSqrt(jointFour.x*jointFour.x + jointFour.y*jointFour.y);
    double v_side = qSqrt(dist_1_4*dist_1_4 + xy_4*xy_4);
    result.q3 = -(M_PI - qAcos((L2*L2+L3*L3-v_side*v_side)/(2*L2*L3)));

    //Zweiter Winkel berechnen
    result.q2 = qAtan2(dist_1_4,xy_4) + qAcos((v_side*v_side+L2*L2-L3*L3)/(2*v_side*L2)); // Notiz: für richtige Berechnung muss acos Term manchmal subtrahiert werden, abhängig davon wo der Greifer steht

    // Umrechnen in Gradmaß
    result.q1 = result.q1 *180/M_PI;
    result.q2 = result.q2 *180/M_PI;
    result.q3 = result.q3 *180/M_PI;

    return result;
}
