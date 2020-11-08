#ifndef _PATH_PLANNING_H_
#define _PATH_PLANNING_H_

#include "public.h"





vector<Node2D> PathPlanningOnTopology1(int robotId, vector<MapNode> Map);

int PlanToGoodsArea(int robotId, vector<MapNode> Map);
int LeaveGoodsArea(vector<MapNode> Map, int robotId);
int UnloadGoods(int robotId);
vector<vector<Node2D>> CreatPoints(void);
float CalHeatValueBetweenTwoNode(NodeInfo node1, NodeInfo node2, vector<vector<Node2D>> Points);
vector<Node2D> OnWhichRoad(int x, int y, vector<vector<Node2D>> Points);
vector<Node2D> PathPlanningOnTopology(int robotId, int areaId, vector<vector<Node2D>> Points);
int ReachNewNode(int robotId, vector<MapNode> Map);
NodeInfo popSmallestNode(vector<NodeInfo>& OpenList);
bool Astar(vector<MapNode> Map, int width, int height, int startX, int startY, int endX, int endY, vector<PathNode>& path);
bool Astar2(int width, int height, int startX, int startY, int endX, int endY, vector<Node2D>& path);

vector<MapNode> createBackMap00(vector<MapNode> Map, int area);
vector<MapNode> createBackMap(vector<MapNode> Map, int area);
vector<MapNode> createComeMap(vector<MapNode> Map, int area, int robotX, int robotY, int status, int robotId);

int PlanPath1to2(int param, vector<MapNode> Map, int robotId);
int PlanPath3to4(int param, vector<MapNode> Map, int robotId);
int PlanPath4to5(int param, vector<MapNode> Map, int robotId);




#endif // !_PATH_PLANNING_H_
#pragma once
