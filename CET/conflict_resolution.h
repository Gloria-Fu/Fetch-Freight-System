#ifndef _CONFLICT_RESOLUTION_H_
#define _CONFLICT_RESOLUTION_H_


#include "public.h"



int checkIsHandover(int i, int k, vector<MapNode> map);
int pushback(int robotId, int times);
int CheckNeighborhood();
int RobotAdjustment(int i);
int ReplanPath(int rePlanRobotId, int obstacleRobotId, int isReplan, int isExchange);



#endif

#pragma once
