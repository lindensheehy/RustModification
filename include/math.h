#pragma once

#include <math.h>

// For logging error cases
#include "log.h"

#ifndef pi
#define pi 3.14159265358979323846
#endif

// This doesnt need to be extremely high becuase its just for trig functions and such
#ifndef inf
#define inf 1e100
#endif

// For degree conversion
const double degToRadFactor = pi / 180;


/*   --------------------------   */
/*   ---   Trig Functions   ---   */
/*   --------------------------   */

// Sine function. Uses radians
double sin(double x);

// Cosine function. Uses radians
double cos(double x);

// Tangent function. Uses radians
double tan(double x);

// Reverse sine function. Uses radians
double arcsin(double x);

// Reverse cosine function. Uses radians
double arccos(double x);

// Reverse tangent function. Uses radians
double arctan(double x);


/*   ---------------------------   */
/*   ---   Other Functions   ---   */
/*   ---------------------------   */

// Returns the distance between 2 sets of 2 dimensional coordinates
double distance2(double x1, double y1, double x2 = 0, double y2 = 0);

// Returns the distance between 2 sets of 3 dimensional coordinates
double distance3(double x1, double y1, double z1, double x2 = 0, double y2 = 0, double z2 = 0);

// Returns the distance between 2 sets of 4 dimensional coordinates
double distance4(double x1, double y1, double z1, double w1, double x2 = 0, double y2 = 0, double z2 = 0, double w2 = 0);

// Returns a value 0-1 for how far between 'from' and 'to' the value is. Returns -1 if its outside the range.
double range(double num, double from, double to);

// Same as above, but allows values outside the range, in which case it would return values outside 0-1
double inRange(double num, double from, double to);

// Returns the angle between two points in DEGREES.
// This angle is from a vertical line on (x1, y1) to (x2, y2)
// For example if (x2, y2) is directly right of (x1, y1) this returns 90. 
// The second position is set to (0, 0) by default.
double getAngle(double x1, double y1, double x2 = 0, double y2 = 0);


/*   ----------------------------   */
/*   ---   Inline Functions   ---   */
/*   ----------------------------   */

// Angle conversion. Degrees -> Radians
inline double toRadians(double degrees) {
    return degrees * degToRadFactor;
}

// Angle conversion. Radians -> Degrees
inline double toDegrees(double radians) {
    return radians / degToRadFactor;
}
