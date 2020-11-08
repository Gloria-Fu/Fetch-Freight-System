#ifndef _GOODS_AREA_H_
#define _GOODS_AREA_H_



#include "public.h"






/*
GoodsAreaInfo类的作用：货区监控信息管理
*/

//struct MapNode;
//struct Node2D;
//struct task;


struct InAreaRobot {
	int robotId = -1;
	int currentX = -1;
	int currentY = -1;
	int robotType = -1;
	int status = -1;
};



class GoodsAreaInfo {
private:
	int leftX = 0;							//货区左上角横坐标
	int leftY = 0;							//货区左上角纵坐标
	int rightX = 0;							//货区右下角横坐标
	int rightY = 0;							//货区右下角纵坐标

	int predictCostTime = 0;

	void clear();

	////////////////////////////////////////货区监控实时信息相关定义
	int currentRobotNum = 0;
	vector<InAreaRobot> InAreaRobotInfo;

	//////////////////////////////////////////////////////////////////

public:
	GoodsAreaInfo();
	~GoodsAreaInfo();

	int areaId = 0;

	int pickRobotNum = 0;
	int freiRobotNum = 0;

	/////////////////////////////////////完成任务订单的相关参数
	int taskNum = 0;
	int finishedTaskNum = 0;			//已完成的任务数量
	int isAllFinished = 0;				//标记是否已完成该区域任务
	int finishPeriod = 0;				//记录完成该区域全部任务的时间

	vector<task> unFinishedTask;		//未开始执行的任务订单
	vector<task> pickingTask;			//已分配给拣货机器人的任务订单
	queue<task> cooperateTask;			//已分配给运货机器人的任务订单

	void updateTask0(task task0);
	void updateTask1(int pickingId, int goods_X, int goods_Y, int index);
	void updateTask2(int freightId);
	//////////////////////////////////////////////////////////////////


	//////////////////////////////////////计算预分配机器人的相关参数
	int pickOrfrei = -1;				//需要拣货机器人or运货机器人
	int pickDistance = 0;				//拣货机器人完成单个任务需要走过的平均距离（手动设置，不准）
	int freiDistance = 0;				//运货机器人完成单个任务需要走过的平均距离（手动设置，不准）

	int calTaskTime(int currentTaskNum);
	
	int pickingWaitPeriod = 0;
	int freightWaitPeriod = 0;
};



extern vector<GoodsInfo> GoodsContents;					//货物信息
extern vector<vector <GoodsInfo>> GoodsInformation;
extern vector<GoodsAreaInfo> taskProcess;



#endif // !_GOODS_AREA_H_


#pragma once
