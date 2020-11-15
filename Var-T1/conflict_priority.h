#ifndef _CONFLICT_PRIORITY_H_
#define _CONFLICT_PRIORITY_H_

#include "public.h"

int PriorityDecideMethods(int param, int robot_i, int robot_k);
float CalPricebyRule(int robotId);
float ProgressFirst(int robotId);
float CalPricebyVirtualCost(int robotId);
float Judge0(int robotId, float price_get);
float Judge1(int robotId, float price_out, float price_get);			
int AdjustWeight(int robotId);
int CalReward(int taskId, int area);
float calLine(int areaId, int robotType, int currentDistance);




#endif // !_CONFLICT_PRIORITY_H_
#pragma once
