#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <QtMath>
//Grenzen der von den Servos erreichbaren Winkel
extern const double Q1_MIN;
extern const double Q1_MAX;
extern const double Q2_MIN;
extern const double Q2_MAX;
extern double Q3_MIN;
extern double Q3_MAX;
//Längen der einzelnen Gelenke
extern const double  L1; //mm
extern const double  L2;
extern const double  L3;
extern const double  L4;



struct Point {
    quint16 x;
    quint16 y;
    quint16 z;
};

struct Angles {
    // Winkel der drei Bewegungsachsen ohne Greifer
    /*
     * Die Winekl sind so definiert: Basis: q1 = 0° Basis zeigt nach vorne
     *                               Hauptarm: q2
     *                               Horizontalarm: q3
     *
     */
    double q1;
    double q2;
    double q3;
};

void q3CalcLimits(Angles angles);
bool checkErrorJointLimits(Angles angles); // Überprüfe ob Winkel innerhalb der Grenzen liegen, sonst erzeuge Fehlermeldung
Point forwardKinematics(Angles); //übergebe Winkel und berechne daraus die Greiferposition
Angles reverseKinematics(Point); //übergebe Punkt der Greiferposition und berechne daraus die Winkel
Angles mapKinematicsToServoAngles(Angles angles);


#endif // KINEMATICS_H
