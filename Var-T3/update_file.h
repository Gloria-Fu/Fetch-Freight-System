#ifndef _UPDATE_FILE_H_
#define _UPDATE_FILE_H_


#include "public.h"

int CreateMap(int width, int height, int shelfX, int shelfY, int conveyor_num);
vector<MapNode> getMapNode(void);
int readParameters(void);
vector<int> getTask(int goodsTypeNum);
int writeRobot(void);
int writeRobot2(void);
int getRobotDefault(void);
int getGoodsNode2(void);
int getEndNode(void);
int OutputResult(int param, int index, char* s3);
int ReadGoodsAndTasks(int index, char* s1, char* s2);





#endif // !_UPDATEFILE_H_
#pragma once
