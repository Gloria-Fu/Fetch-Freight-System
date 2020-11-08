#ifndef _PUBLIC_H_
#define _PUBLIC_H_


#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <queue>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>

//#include "goods_area.h"


using namespace std;


struct Node2D {
	int x;
	int y;
};


struct task {
	int taskId;
	int goodsType;
	int goodsId;
	int pickingRobotId;
	int freightRobotId;
	int goodsX;
	int goodsY;
	int status;
};




struct GoodsInfo {
	int goods_Id = 0;
	int goods_X = 0;
	int goods_Y = 0;
	int goods_Num = 0;
	int goods_Type = 0;
	int goods_status = 0;

};


struct MapNode {				//静态地图坐标信息
	int x;						//地图点的横坐标
	int y;						//地图点的纵坐标
	int nodeType;				//地图点的类型
	int nodeId;					//地图点的ID
	int isAccessible;			//地图点是否可通行，0可，1不可
	int nodeWeight;				//该点用于计算A*时的权重值
	int distance;
};


struct NodeInfo {
	int x;						//点的横坐标
	int y;						//点的纵坐标
	float g;					//当前点距离父节点的距离
	float h;					//起始点到终点的曼哈顿距离
	float f;					//f = g + h
	int index;					//当前点的一维坐标位置
	int preIndex;				//其父节点的一维坐标位置
};

struct PathNode {
	int x;						//路径节点的横坐标
	int y;						//路径节点的纵坐标
	int index;					//路径节点的下标
	int robotID;				//路径所属的机器人编号
};



struct robotAttribute {
	int robotId = -1;				//机器人编号
	int robotDefaultX = 0;			//机器人缺省横坐标
	int robotDefaultY = 0;			//机器人缺省纵坐标
	int robotType = -1;				//机器人类型
	int robotCurrentX = 0;			//机器人当前横坐标
	int robotCurrentY = 0;			//机器人当前纵坐标
	int robotNextX = 0;				//机器人下一个横坐标
	int robotNextY = 0;				//机器人下一个纵坐标
	int robotStatus = 0;			//机器人当前状态 0：空闲  1：工作
	int taskX = 0;					//目标点横坐标
	int taskY = 0;					//目标点纵坐标
	int needUpdate = 0;				//是否需要更新坐标
	int relatedRobotId = -1;		//协同工作的另一个机器人ID
	int area = 0;					//该机器人所负责的区域
	int taskId = 0;					//当前负责的机器人编号
	int goodsType = -1;				//当前在运的货物种类
	int goodsId = 0;				//单个货物小编号
	int infoFlag = 0;				//是否成功与边缘端通信
	int oncePathSize = 0;			//每段路程计划工作时长
	int onceWorkPreiod = 0;			//每段路程实际工作时长
	int one_wayStep = 0;			//执行任务时单程实际时长（取货/卸货，与上述的分段路程不同）
	int waitTime = 0;				//交接前等待对方的时间
};



struct Account {
	float balance = 300;				//机器人当前奖励剩余值
	float last_balance = 300;			//上一次收到奖励时的余额
	float yield = 0;					//收益率
	float param_addWeight = 0.5;		//参数加权：用于决定该机器人是激进还是温和，根据历史状况改变，可正可负
	int flag = 0;
};



class RobotInfo {
public:
	robotAttribute attribute;
	Account calBalance;
	vector<MapNode> robot_map;
	vector<PathNode> robot_path;
	vector<Node2D> highLevelPath;
};




class taskRecord {
public:
	int planPeriod;
	int realPreiod;
	int overPeriod;
	int planTime;
	int startPeriod;
	int endPeriod;
	int pickRobotWorkStartTime;
	int pickRobotWorkEndTime;
	int pickWaitTime;
	int freightRobotWorkStartTime;
	int freightRobotWorkEndTime;
	int feiWaitTime = 0;

	task taskInfo;

	taskRecord(void) {
		planPeriod = 0;
		realPreiod = 0;
		overPeriod = 0;
		planTime = 0;
		startPeriod = 0;
		endPeriod = 0;
		pickRobotWorkStartTime = 0;
		pickRobotWorkEndTime = 0;
		pickWaitTime = 0;
		freightRobotWorkStartTime = 0;
		freightRobotWorkEndTime = 0;
		feiWaitTime = 0;

		taskInfo.taskId = 0;
		taskInfo.goodsType = 0;
		taskInfo.freightRobotId = 0;
		taskInfo.goodsX = 0;
		taskInfo.goodsY = 0;
		taskInfo.pickingRobotId = 0;
		taskInfo.status = 0;
	}
};


class GoodsAreaInfo;

//输入参数
extern int mapWidth;			//1.地图宽度
extern int mapHeight;			//2.地图高度
extern int shelfXnum;			//3.货区行数
extern int shelfYnum;			//4.货区列数
extern int areaNum;				//5.货区数量
extern int goodsTypeNum;		//6.货物种类数量
extern int pickingRobotNum;		//7.拣货机器人数量
extern int freightRobotNum;		//8.运货机器人数量
extern int conveyorNum;			//9.传送带数量（单边或双边）
//全局变量
extern int robotNum;			//.机器人总数
extern int globalPeriod;		//.运行周期
extern int finishAreaNum;       //.已完成任务的区域数量
extern int start_x;				//从左往右数第一个货区的左上角x坐标
//全局不变量
const int eachGoodsNum = 48;
const int start_y = 4;
const int shelfWidth = 16, shelfHeight = 11;
const int goodsWidth = 12, goodsHeight = 2;
const int road = 3;





extern vector<RobotInfo> updateRobotInfo;		//全部机器人的信息
extern vector<MapNode> EndNode;
extern vector<MapNode> EndPointRight1;
extern vector<MapNode> EndPointRight2;
extern vector<taskRecord> taskPeriodTime;








#endif // !_PUBLIC_H_


#pragma once
