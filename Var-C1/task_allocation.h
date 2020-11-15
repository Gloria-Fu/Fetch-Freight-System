#ifndef _TASK_ALLOCATION_H_
#define _TASK_ALLOCATION_H_


#include "public.h"


int AllocateTask2Area(void);
int AssignPickingRobot(vector<MapNode> Map);
int AssignFreightRobot(vector<MapNode> Map);
int preAllocate(void);
int ReAssign(int period);
int HelpOtherArea(void);
int RobotAllocationMethods(int param);




#endif // !_TASKALLOCATION_H_

#pragma once
