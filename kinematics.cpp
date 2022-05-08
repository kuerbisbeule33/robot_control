#include "kinematics.h"
#include <QtMath>

//Grenzen der von den Servos erreichbaren Winkel
const double Q1_MIN = -30;
const double Q1_MAX = 30;
const double Q2_MIN = 39;
const double Q2_MAX = 120;
      double Q3_MIN = -90;
      double Q3_MAX = 90;
//Längen der einzelnen Gelenke
const double  L1 = 92.0; //mm
const double  L2 = 135.0;
const double  L3 = 147.0;
const double  L4 = 87.0;


void q3CalcLimits(double angleVertical2){
    Q3_MIN = (-0.6755*angleVertical2)-70.768;
    Q3_MAX = (-0.7165*angleVertical2)-13.144;

}

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

bool checkErrorJointLimits(Angles angles){
    q3CalcLimits(angles);
    // testen auf nan und inf bei der Berechnung
    if(angles.q1 == NAN || angles.q1 == INFINITY || angles.q2 == NAN || angles.q2 == INFINITY || angles.q3 == NAN || angles.q3 == INFINITY){
        return true;
    }
    else if(angles.q1 >= Q1_MIN && angles.q1 <= Q1_MAX && angles.q2 >= Q2_MIN && angles.q2 <= Q2_MAX && angles.q3 >= Q3_MIN && angles.q3 <= Q3_MAX){
        return true;
    }
    return false;
}

quint16 q1ToServo(double q1){
    q1 = round(((-2.0479)*q1)+91.726);
    return q1;
}
quint16 q2ToServo(double q2){
    q2  = round(180- q2);
    return q2;
}
quint16 q3ToServo(double q2, double q3){
    q3 = round(q2 + 135 + q3);
    return q3;
}
