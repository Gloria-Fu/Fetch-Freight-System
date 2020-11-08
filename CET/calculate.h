#ifndef _CALCULATE_H_
#define _CALCULATE_H_

#include "public.h"

int CreatInitRobots(void);
int WalkToOwnArea(vector<MapNode> Map);
int calDistance(int x, int y);
int chooseNewEndNode(int x, int y, vector<MapNode> Map);
int ChooseEndPoint(int robotId);
int FreightReturn(int freightRobotId, int target, vector<MapNode> Map);
int FreightReturn1(int robotId, int target, vector<MapNode> Map);
bool useAStar(int robotId, vector<MapNode> Map);
vector<MapNode> CreateFreightRobotMap(int areaId, vector<MapNode> Map);
vector<MapNode> CreatePickingRobotMap(vector<MapNode> Map, int robotId);
vector<MapNode> CreateMap(vector<MapNode> Map);
vector<MapNode> CreateNewRobotMap(int robotId, vector<MapNode> Map);
int chooseHandoverNode(int goodsId, int areaId, int freightRobot);
bool checkNearby(int taskGoodsX, int taskGoodsY, int areaId, int robotId);
bool checkIsInArea(int robotId);




#endif // !_CALCULATE_H_
#pragma once
