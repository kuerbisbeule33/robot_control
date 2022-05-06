#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <QtMath>

extern const double armLengthHorizontal;
extern const double armLengthVertical;
extern const double gripLength;


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

bool checkErrorJointLimits(Angles angles); // Überprüfe ob Winkel innerhalb der Grenzen liegen, sonst erzeuge Fehlermeldung
Point forwardKinematics(Angles); //übergebe Winkel und berechne daraus die Greiferposition
Angles reverseKinematics(Point); //übergebe Punkt der Greiferposition und berechne daraus die Winkel



#endif // KINEMATICS_H
