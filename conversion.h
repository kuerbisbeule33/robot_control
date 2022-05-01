#ifndef CONVERSION_H
#define CONVERSION_H

#include <QtMath>

extern const double armLengthHorizontal;
extern const double armLengthVertical;
extern const double gripLength;

struct Point {
    quint16 x;
    quint16 y;
    quint16 z;
};

double lawOfCosinus(double a, double b, double c);
Point angleToCoordinates(quint16 rotationAngle, quint16 tiltAngle, quint16 horizontalAngle);
void coordinatesToAngle(Point point, quint16& rotationAngle, quint16& tiltAngle, quint16& horizontalAngle);

#endif // CONVERSION_H
