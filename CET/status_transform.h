#ifndef _STATUS_TRANSFORM_H_
#define _STATUS_TRANSFORM_H_


#include "public.h"

int TriggerInArea(int robotId);
int RobotMove(vector<MapNode> Map, int period);
int TaskStatus3to4(int freightRobotId, vector<MapNode> Map);
int TaskStatus4to5(int freightRobotID, int currentPeriod, vector<MapNode> Map);
int AllRobotBack(vector<MapNode> Map);








#endif // !_STATUS_TRANSFORM_H_
#pragma once
