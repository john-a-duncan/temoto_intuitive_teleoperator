// Include all temoto messages
#include "temoto/Command.h"
#include "temoto/Status.h"
#include "temoto/ChangeTf.h"
#include "temoto/Goal.h"

#ifndef TEMOTO_COMMON_H
#define TEMOTO_COMMON_H

// Calculate the distance between the first two points in the input vector of points
double calculateDistance (std::vector <geometry_msgs::Point> & twoPointVector);

geometry_msgs::Quaternion combineQuaternions(geometry_msgs::Quaternion q0, geometry_msgs::Quaternion q1);

#endif