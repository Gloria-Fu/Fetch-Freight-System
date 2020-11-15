#include "conflict_priority.h"
#include "calculate.h"
#include "goods_area.h"


int PriorityDecideMethods(int param, int robot_i, int robot_k)
{
	int rePlanRobot, obstacleRobot;

	if (param == 1) {												//1.竞价
		float price_i = CalPricebyVirtualCost(robot_i);
		float price_k = CalPricebyVirtualCost(robot_k);
		float price_get, price_out;

		if (price_i > price_k) {
			rePlanRobot = robot_k;
			obstacleRobot = robot_i;
			updateRobotInfo[robot_i].calBalance.balance -= price_i;
			updateRobotInfo[robot_k].calBalance.balance += price_i;
			price_get = price_i;
			price_out = price_i;
		}
		else {
			rePlanRobot = robot_i;
			obstacleRobot = robot_k;
			updateRobotInfo[robot_k].calBalance.balance -= price_k;
			updateRobotInfo[robot_i].calBalance.balance += price_k;
			price_get = price_k;
			price_out = price_k;
		}

		Judge0(rePlanRobot, price_out);
		Judge1(obstacleRobot, price_i, price_k);
		AdjustWeight(robot_i);
		AdjustWeight(robot_k);
	}
	else if (param == 2) {
		if (robot_i > robot_k) {											//2.给定优先级
			rePlanRobot = robot_k;
			obstacleRobot = robot_i;
		}
		else {
			rePlanRobot = robot_i;
			obstacleRobot = robot_k;
		}
	}	
	else if (param == 3) {													//3.按所属区域进度快慢决定
		float price_i = ProgressFirst(robot_i);
		float price_k = ProgressFirst(robot_k);

		if (price_i > price_k) {
			rePlanRobot = robot_k;
			obstacleRobot = robot_i;
			updateRobotInfo[robot_i].calBalance.balance -= price_i;
			updateRobotInfo[robot_k].calBalance.balance += price_i;
		}
		else {
			rePlanRobot = robot_i;
			obstacleRobot = robot_k;
			updateRobotInfo[robot_k].calBalance.balance -= price_k;
			updateRobotInfo[robot_i].calBalance.balance += price_k;
		}
	}

	return ((rePlanRobot == robot_i) ? robot_i : robot_k);						//return 需要重新规划的robotID
}




//遇到冲突时计算机器人愿意出价多少
//冲突时出的价钱是否应该少于避障所花的金额，如何平衡花费金额与余额与整体收益趋势的关系
float CalPricebyRule(int robotId)
{
	float price = 0;
	int status = updateRobotInfo[robotId].attribute.robotStatus;
	int type = updateRobotInfo[robotId].attribute.robotType;
	int distance = 0;

	float fx_status = 0, fx_type = 0, fx_distance = 0;

	float balance = updateRobotInfo[robotId].calBalance.balance;

	//1.机器人种类的不同取值，暂时默认拣货机器人的优先级比运货机器人高,加起来等于1
	if (type == 0)
		fx_type = 0.6;
	else
		fx_type = 0.4;

	//2.机器人不同状态的不同取值，要分开机器人种类讨论，默认状态值越高优先级越高
	if (type != 0) {													//运货机器人
		if (status == 0) {												//回缺省点的路上，没有任务
			fx_status = 0.1;

			//测机器人当前位置到默认位置的距离？
			distance = abs(updateRobotInfo[robotId].attribute.robotCurrentX - updateRobotInfo[robotId].attribute.robotDefaultX) + abs(updateRobotInfo[robotId].attribute.robotCurrentY - updateRobotInfo[robotId].attribute.robotDefaultY);
		}
		else if (status == 1) {											//去取货的路上，有任务
			fx_status = 0.35;

			//测机器人当前位置到目标货物的位置
			distance = abs(updateRobotInfo[robotId].attribute.robotCurrentX - taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].taskInfo.goodsX) + abs(updateRobotInfo[robotId].attribute.robotCurrentY - taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].taskInfo.goodsY);
		}
		else if (status == 2) {
			fx_status = 0;
		}										//和拣货机器人交接的过程中，有任务(一般不用避障吧？就是避障的算法应该先判断是否要避障，再计算优先级)
		else if (status == 3) {											//将货物运到传送带的路上，准备卸货，有任务
			fx_status = 0.55;
			distance = calDistance(updateRobotInfo[robotId].attribute.robotCurrentX, updateRobotInfo[robotId].attribute.robotCurrentY);
			//distance = abs(updateRobotInfo[robotId].attribute.robotCurrentX - EndNode[0].x);	//暂时只能粗略估计机器人到传送带的横向距离了
		}
		else {
			int a = 0;
		}
	}
	else if (type == 0) {												//拣货机器人
		if (status == 0) {												//没有任务，在原地待着，或者回缺省点路上，或者在缺省点位置（一般应该不动）
			fx_status = 0.3;											//前提有路径的吧？如果没有路径的进不来了吧？
			if (updateRobotInfo[robotId].robot_path.size() == 0) {
				int a = 0;
			}

			distance = abs(updateRobotInfo[robotId].attribute.robotCurrentX - updateRobotInfo[robotId].attribute.robotDefaultX) + abs(updateRobotInfo[robotId].attribute.robotCurrentY - updateRobotInfo[robotId].attribute.robotDefaultY);
		}
		else if (status == 1) {											//去取货的路上，有任务
			fx_status = 0.7;

			distance = abs(updateRobotInfo[robotId].attribute.robotCurrentX - taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].taskInfo.goodsX) + abs(updateRobotInfo[robotId].attribute.robotCurrentY - taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].taskInfo.goodsY);
		}
		else if (status == 2) {}										//取货的过程，静止不动
		else if (status == 3) {}										//和运货机器人交接，静止不动
	}

	//3.计算机器人与目标点的距离，距离越近，优先级越高？(这部分严重有问题，因为机器人的task位置已经不正常了吧)
	distance = abs(updateRobotInfo[robotId].attribute.robotCurrentX - updateRobotInfo[robotId].attribute.taskX) + abs(updateRobotInfo[robotId].attribute.robotCurrentY - updateRobotInfo[robotId].attribute.taskY);

	//每个机器人每次需要经过的距离都不一样，应该不可能同一套标准制定

	fx_distance = calLine(updateRobotInfo[robotId].attribute.area, updateRobotInfo[robotId].attribute.robotType, distance);

	float sumPrice = fx_status + fx_distance + fx_type + updateRobotInfo[robotId].calBalance.param_addWeight;

	if (balance <= sumPrice) {											//计算机器人剩余余额能否走到终点，需要预计那个剩余距离？
		return 0;
	}
	else {
		return sumPrice;
	}

}


float ProgressFirst(int robotId)
{
	///////////////////////////////////////////////////////////////////////////////////////////////将该区域完成时间加入到竞价权重
	int area = updateRobotInfo[robotId].attribute.area - 1;
	float progress = 0;
	if (area >= 0 && taskProcess[area].taskNum > 0 && taskProcess[area].finishedTaskNum > 0)
		progress = float(taskProcess[area].finishedTaskNum) / float(taskProcess[area].taskNum) * 100;
	else if (area < 0)
		progress = 100;
	else if (taskProcess[area].finishedTaskNum == 0)
		progress = 0;

	float fx_Progress = (100 - progress) / 10;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float sumPrice = 0;

	sumPrice = fx_Progress;

	return sumPrice;
}



float CalPricebyVirtualCost(int robotId)
{
	//PathNode tempNewPath = Astar(Map, mapWidth, mapHeight, updateRobotInfo[robotId].attribute.robotCurrentX, updateRobotInfo[robotId].attribute.robotCurrentY, )
	int extraPath = 2;
	float price = updateRobotInfo[robotId].calBalance.param_addWeight * extraPath;

	return price;
}


float Judge0(int robotId, float price_get)			//让的那个
{
	int extraPath = 2;

	if (2 * price_get > extraPath) {				//决策对了，flag-1
		updateRobotInfo[robotId].calBalance.flag -= 1;
	}
	else if (2 * price_get <= extraPath) {			//决策错了，flag+1
		updateRobotInfo[robotId].calBalance.flag += 1;
	}

	return 0;
}


float Judge1(int robotId, float price_out, float price_get)			//不让的那个
{
	int extraPath = 2;

	if (price_get + price_out > extraPath) {						//决策错了，flag-1
		updateRobotInfo[robotId].calBalance.flag -= 1;
	}
	else if (price_get + price_out <= extraPath && price_out > 1.5 * price_get) {					//决策错了，flag-1
		updateRobotInfo[robotId].calBalance.flag -= 1;
	}

	return 0;
}


int AdjustWeight(int robotId)
{
	if (updateRobotInfo[robotId].calBalance.flag >= 5) {
		updateRobotInfo[robotId].calBalance.param_addWeight *= 1.1;
		updateRobotInfo[robotId].calBalance.flag = 0;
	}
	else if (updateRobotInfo[robotId].calBalance.flag <= -5) {
		updateRobotInfo[robotId].calBalance.param_addWeight *= 0.9;
		updateRobotInfo[robotId].calBalance.flag = 0;
	}

	return 0;
}



//奖励目前来看应该按照任务的位置+机器人的完成情况制定
//每个货物的奖励算是基本奖励，机器人完成情况作为惩罚项
int CalReward(int taskId, int area)
{
	//1.计算拣货机器人的收益
	float pick_reward = 8;
	int pickId = taskPeriodTime[taskId].taskInfo.pickingRobotId - 1;

	float pickCostTime = taskPeriodTime[taskId].pickRobotWorkEndTime - taskPeriodTime[taskId].pickRobotWorkStartTime - taskPeriodTime[taskId].pickWaitTime - 30;

	updateRobotInfo[pickId].calBalance.balance += pick_reward;

	//2.计算运货机器人的收益
	int freightId = taskPeriodTime[taskId].taskInfo.freightRobotId - 1;
	
	float frei_reward = 0;
	int goodsDistance = calDistance(taskPeriodTime[taskId].taskInfo.goodsX, taskPeriodTime[taskId].taskInfo.goodsY);

	float freiCostTime = taskPeriodTime[taskId].freightRobotWorkEndTime - taskPeriodTime[taskId].freightRobotWorkStartTime - taskPeriodTime[taskId].feiWaitTime - 20;

	frei_reward +=  2.5 * goodsDistance;			

	updateRobotInfo[freightId].calBalance.balance += frei_reward;

	return 0;
}




float calLine(int areaId, int robotType, int currentDistance)
{
	int maxDistance;

	if (areaId > 0) {
		if (robotType == 0) {
			maxDistance = taskProcess[areaId - 1].pickDistance;
		}
		else {
			maxDistance = taskProcess[areaId - 1].freiDistance;
		}
	}
	else if (areaId == 0) {
		return 0;
	}

	if (currentDistance > maxDistance)
		currentDistance = maxDistance;

	float x1 = 0, x2 = float(maxDistance), y1 = 1, y2 = 0;

	float a = (y2 - y1) / (x2 - x1);
	float b = y1 - a * x1;

	float currentY = a * float(currentDistance) + b;

	return currentY;
}
