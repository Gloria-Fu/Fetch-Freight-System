#include "status_transform.h"
#include "conflict_resolution.h"
#include "calculate.h"
#include "path_planning.h"
#include "conflict_priority.h"
#include "goods_area.h"
#include "update_file.h"


int TriggerInArea(int robotId)
{
	int robotX = updateRobotInfo[robotId].attribute.robotCurrentX;
	int robotY = updateRobotInfo[robotId].attribute.robotCurrentY;
	int area = updateRobotInfo[robotId].attribute.area;

	int x = (area - 1) / shelfYnum;
	int y = (area - 1) % shelfYnum;
	int areaX = start_x + x * (shelfWidth + road);
	int areaY = start_y + y * (shelfHeight + road);

	if (robotX >= areaX && robotX < areaX + shelfWidth && robotY >= areaY && robotY < areaY + shelfHeight) {			//进入了货区
		chooseHandoverNode(updateRobotInfo[robotId].attribute.goodsId, area, robotId);

		updateRobotInfo[robotId].robot_path.clear();
		bool isSuccess = useAStar(robotId, updateRobotInfo[robotId].robot_map);
		if (isSuccess == false) {
			if (updateRobotInfo[robotId].attribute.robotCurrentX == updateRobotInfo[robotId].attribute.taskX && updateRobotInfo[robotId].attribute.robotCurrentY == updateRobotInfo[robotId].attribute.taskY) {
				pushback(robotId, 1);
				updateRobotInfo[robotId].highLevelPath.clear();
			}
			else {
				int a = 0;
			}
		}
		else {
			updateRobotInfo[robotId].highLevelPath.clear();
		}

		updateRobotInfo[robotId].attribute.robotNextX = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].x;
		updateRobotInfo[robotId].attribute.robotNextY = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].y;

		updateRobotInfo[robotId].attribute.infoFlag = 1;
	}

	return 0;
}


int RobotMove(vector<MapNode> Map, int period)
{
	PathNode updateCurrentNode, updateNextNode;

	for (int i = 0; i < robotNum; i++) {
		if (i == 186 && globalPeriod == 125)
			int a = 0;

		if (updateRobotInfo[i].robot_path.size() > 1) {													//判断该机器人是否有下一步和下下一步的路径坐标点
			updateCurrentNode = updateRobotInfo[i].robot_path[updateRobotInfo[i].robot_path.size() - 1];					//获取该机器人路径的下一步坐标
			updateNextNode = updateRobotInfo[i].robot_path[updateRobotInfo[i].robot_path.size() - 2];						//获取该机器人路径的下下一步坐标

			updateRobotInfo[i].attribute.robotCurrentX = updateCurrentNode.x;
			updateRobotInfo[i].attribute.robotCurrentY = updateCurrentNode.y;
			updateRobotInfo[i].attribute.robotNextX = updateNextNode.x;
			updateRobotInfo[i].attribute.robotNextY = updateNextNode.y;
			updateRobotInfo[i].attribute.needUpdate = 1;

			updateRobotInfo[i].robot_path.pop_back();													//把该坐标点从该机器人的路径中删除

			if (updateRobotInfo[i].attribute.robotType != 0 && updateRobotInfo[i].attribute.robotStatus == 1 && updateRobotInfo[i].attribute.infoFlag == 0)
				TriggerInArea(i);
		}
		else if (updateRobotInfo[i].robot_path.size() == 1) {											//判断该机器人是否只剩一个路径坐标点，下下坐标点写NULL，与一开始一致
			updateCurrentNode = updateRobotInfo[i].robot_path[updateRobotInfo[i].robot_path.size() - 1];					//获取该机器人路径的下一步坐标

			updateRobotInfo[i].attribute.robotCurrentX = updateCurrentNode.x;
			updateRobotInfo[i].attribute.robotCurrentY = updateCurrentNode.y;
			updateRobotInfo[i].attribute.robotNextX = updateCurrentNode.x;
			updateRobotInfo[i].attribute.robotNextY = updateCurrentNode.y;
			updateRobotInfo[i].attribute.needUpdate = 1;

			updateRobotInfo[i].robot_path.pop_back();													//把该坐标点从该机器人的路径中删除

			if (updateRobotInfo[i].highLevelPath.size() == 0) {							//机器人high-level路径走完了，证明已经到单程全部路径的终点（取货或者卸货的最后一段路的终点）
				if (updateRobotInfo[i].attribute.robotStatus == -1) {					//最后机器人归位状态
					updateRobotInfo[i].attribute.robotStatus = 0;
					/*updateRobotInfo[i].attribute.taskX = 0;
					updateRobotInfo[i].attribute.taskY = 0;*/
				}
				else if (updateRobotInfo[i].attribute.robotStatus == 0) {				//机器人回到缺省点了，待着就行
					updateRobotInfo[i].attribute.taskX = 0;
					updateRobotInfo[i].attribute.taskY = 0;
				}
				else {																	//普通工作时状态
					if (i < freightRobotNum) {											//证明是运货机器人，运货机器人完成路径才算完成任务
						if (updateRobotInfo[i].attribute.robotStatus == 1) {			//证明现在是运货机器人到取货货物的位置	

							if ((updateRobotInfo[int(updateRobotInfo[i].attribute.relatedRobotId - 1)].attribute.robotStatus == 1) ||
								(updateRobotInfo[int(updateRobotInfo[i].attribute.relatedRobotId - 1)].attribute.robotStatus == 2 &&
									updateRobotInfo[int(updateRobotInfo[i].attribute.relatedRobotId - 1)].robot_path.size() > 0)) {	//运货机器人到目标点了，但拣货机器人没到，在原地等，检测到robot状态为2时就可以工作了

								updateRobotInfo[i].attribute.waitTime++;
								taskPeriodTime[updateRobotInfo[i].attribute.taskId - 1].feiWaitTime++;

								updateRobotInfo[i].calBalance.balance++;
								updateRobotInfo[i].attribute.one_wayStep--;

								pushback(i, 1);
								updateRobotInfo[i].attribute.oncePathSize = updateRobotInfo[i].robot_path.size();
								updateRobotInfo[i].attribute.onceWorkPreiod = 0;
							}
							else if (updateRobotInfo[int(updateRobotInfo[i].attribute.relatedRobotId - 1)].attribute.robotStatus == 2 &&
								updateRobotInfo[int(updateRobotInfo[i].attribute.relatedRobotId - 1)].robot_path.size() == 0) {		//该任务的拣货机器人和运货机器人均到达货物点,开始交接
								pushback(i, 15);
								pushback(updateRobotInfo[i].attribute.relatedRobotId - 1, 15);

								updateRobotInfo[i].attribute.waitTime = 0;
								updateRobotInfo[updateRobotInfo[i].attribute.relatedRobotId - 1].attribute.waitTime = 0;
								updateRobotInfo[i].attribute.oncePathSize = updateRobotInfo[i].robot_path.size();
								updateRobotInfo[i].attribute.onceWorkPreiod = 0;
								updateRobotInfo[int(updateRobotInfo[i].attribute.relatedRobotId - 1)].attribute.oncePathSize = updateRobotInfo[int(updateRobotInfo[i].attribute.relatedRobotId - 1)].robot_path.size();
								updateRobotInfo[int(updateRobotInfo[i].attribute.relatedRobotId - 1)].attribute.onceWorkPreiod = 0;
								updateRobotInfo[i].attribute.one_wayStep = 0;
								updateRobotInfo[int(updateRobotInfo[i].attribute.relatedRobotId - 1)].attribute.robotStatus = 3;
								updateRobotInfo[i].attribute.robotStatus = 2;

								taskPeriodTime[int(updateRobotInfo[i].attribute.taskId - 1)].taskInfo.status = 3;
								taskPeriodTime[int(updateRobotInfo[i].attribute.taskId - 1)].planPeriod += 15;
							}
						}
						else if (updateRobotInfo[i].attribute.robotStatus == 2) {			//当前运货机器人从交接状态2变成返程状态3
							TaskStatus3to4(i, Map);											//任务状态从3到4											
						}
						else if (updateRobotInfo[i].attribute.robotStatus == 3) {
							if (Map[updateRobotInfo[i].attribute.robotCurrentX + mapWidth * updateRobotInfo[i].attribute.robotCurrentY].nodeType == 4) {	//所在位置是传送带，可以卸货
								TaskStatus4to5(i, period, Map);
							}
							else {
								UnloadGoods(i);
							}
						}
						else if (updateRobotInfo[i].attribute.robotStatus == 0) {						//回缺省点状态
							updateRobotInfo[i].robot_map.clear();
							updateRobotInfo[i].attribute.taskX = 0;
							updateRobotInfo[i].attribute.taskY = 0;
							updateRobotInfo[i].attribute.oncePathSize = 0;
							updateRobotInfo[i].attribute.onceWorkPreiod = 0;
						}
					}
					else {
						if (updateRobotInfo[i].attribute.robotStatus == 1) {							//拣货机器人到达目的地
							updateRobotInfo[i].attribute.one_wayStep = 0;
							updateRobotInfo[i].attribute.robotStatus = 2;
							pushback(i, 15);
							updateRobotInfo[i].attribute.oncePathSize = updateRobotInfo[i].robot_path.size();
							updateRobotInfo[i].attribute.onceWorkPreiod = 0;
						}
						else if (updateRobotInfo[i].attribute.robotStatus == 0) {
							updateRobotInfo[i].attribute.taskX = 0;
							updateRobotInfo[i].attribute.taskY = 0;
						}
					}
				}
			}
			else if (updateRobotInfo[i].highLevelPath.size() >= 1) {									//机器人的高层路径还没走完，证明现在并没有到最后一步
				ReachNewNode(i, Map);
			}
		}
		else if (updateRobotInfo[i].robot_path.size() == 0) {											//没有路径的机器人，停在原地
			updateRobotInfo[i].attribute.needUpdate = 0;
			if (i >= freightRobotNum && updateRobotInfo[i].attribute.robotStatus == 2) {
				if (updateRobotInfo[i].attribute.relatedRobotId > 0) {
					updateRobotInfo[i].attribute.waitTime++;
					taskPeriodTime[updateRobotInfo[i].attribute.taskId - 1].pickWaitTime++;
					updateRobotInfo[i].attribute.oncePathSize = updateRobotInfo[i].robot_path.size();
					updateRobotInfo[i].attribute.onceWorkPreiod = 0;
				}
			}
		}

		if (updateRobotInfo[i].attribute.robotStatus == 1 || updateRobotInfo[i].attribute.robotStatus == 2 || updateRobotInfo[i].attribute.robotStatus == 3) {
			updateRobotInfo[i].attribute.onceWorkPreiod++;

			if (updateRobotInfo[i].attribute.robotType == 1 || updateRobotInfo[i].attribute.robotType == 2) {
				taskPeriodTime[updateRobotInfo[i].attribute.taskId - 1].realPreiod++;
			}
			if (updateRobotInfo[i].attribute.robotType == 0 && updateRobotInfo[i].attribute.robotStatus == 1) {
				updateRobotInfo[i].attribute.one_wayStep++;
				updateRobotInfo[i].calBalance.balance -= 1;
			}
			else if (updateRobotInfo[i].attribute.robotType != 0 && (updateRobotInfo[i].attribute.robotStatus == 1 || updateRobotInfo[i].attribute.robotStatus == 3)) {
				updateRobotInfo[i].attribute.one_wayStep++;
				updateRobotInfo[i].calBalance.balance -= 1;
			}
		}
		else if (updateRobotInfo[i].attribute.robotStatus == -1) {
			updateRobotInfo[i].attribute.onceWorkPreiod++;
		}
	}

	writeRobot();

	return 0;
}



int TaskStatus3to4(int freightRobotId, vector<MapNode> Map)
{
	updateRobotInfo[freightRobotId].attribute.goodsType = taskPeriodTime[updateRobotInfo[freightRobotId].attribute.taskId - 1].taskInfo.goodsType;		//1.查询运的货物种类

	taskPeriodTime[updateRobotInfo[freightRobotId].attribute.taskId - 1].taskInfo.status = 4;				//2.把当前任务状态从3修改成4
	taskPeriodTime[updateRobotInfo[freightRobotId].attribute.taskId - 1].pickRobotWorkEndTime = globalPeriod;
	
	//LeaveGoodsArea(Map, freightRobotId);								//规划从货区内到传送带的路径
	PlanPath3to4(1, Map, freightRobotId);

	GoodsContents[updateRobotInfo[updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1].attribute.goodsId - 1].goods_status = 0;

	updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.robotStatus = 0;		//当前运货机器人已到达目标点，拣货机器人交接任务完成，状态变成0:空闲
	updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.taskX = updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.robotDefaultX;
	updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.taskY = updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.robotDefaultY;
	updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.taskId = 0;
	updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.relatedRobotId = 0;		//把拣货机器人的关联机器人编号设为无

	bool isSuccess = useAStar(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1, updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].robot_map);
	if (isSuccess == false) {
		if (updateRobotInfo[freightRobotId].attribute.robotCurrentX == updateRobotInfo[freightRobotId].attribute.taskX && updateRobotInfo[freightRobotId].attribute.robotCurrentY == updateRobotInfo[freightRobotId].attribute.taskY) {
			pushback(freightRobotId, 1);
		}
		else {
			int a = 0;
		}
	}

	pushback(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1, 1);
	updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.robotNextX = updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.robotCurrentX;
	updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.robotNextY = updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.robotCurrentY;
	updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.oncePathSize = updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].robot_path.size();
	updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.onceWorkPreiod = 0;
	
	updateRobotInfo[freightRobotId].attribute.oncePathSize = updateRobotInfo[freightRobotId].robot_path.size();
	updateRobotInfo[freightRobotId].attribute.onceWorkPreiod = 0;
	updateRobotInfo[updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1].attribute.goodsId = 0;
	updateRobotInfo[freightRobotId].attribute.relatedRobotId = 0;														//把运货机器人的关联机器人编号设为无
	updateRobotInfo[freightRobotId].attribute.robotStatus = 3;

	
	return 0;
}



int TaskStatus4to5(int freightRobotID, int currentPeriod, vector<MapNode> Map)
{
	int area = (updateRobotInfo[freightRobotID].attribute.goodsId - 1) / eachGoodsNum;
	taskProcess[area].finishedTaskNum++;

	updateRobotInfo[freightRobotID].attribute.goodsType = 0;

	if (taskProcess[area].taskNum == taskProcess[area].finishedTaskNum) {				//判断该区域是否已完成所有任务
		taskProcess[area].isAllFinished = 1;
		finishAreaNum++;
		taskProcess[area].finishPeriod = currentPeriod;
	}

	taskPeriodTime[updateRobotInfo[freightRobotID].attribute.taskId - 1].taskInfo.status = 5;
	taskPeriodTime[updateRobotInfo[freightRobotID].attribute.taskId - 1].endPeriod = globalPeriod;
	taskPeriodTime[updateRobotInfo[freightRobotID].attribute.taskId - 1].freightRobotWorkEndTime = globalPeriod;

	CalReward(updateRobotInfo[freightRobotID].attribute.taskId - 1, area);
	
	updateRobotInfo[freightRobotID].attribute.one_wayStep = 0;
	updateRobotInfo[freightRobotID].attribute.robotStatus = 0;							//把运货机器人的状态设为0:空闲，可分配给其他新的任务

	if (updateRobotInfo[freightRobotID].attribute.robotType == 2) 						//如果是机动机器人，服务的任务区域需要重新置零
		updateRobotInfo[freightRobotID].attribute.area = 0;
	updateRobotInfo[freightRobotID].attribute.taskId = 0;

	updateRobotInfo[freightRobotID].attribute.taskX = updateRobotInfo[freightRobotID].attribute.robotDefaultX;
	updateRobotInfo[freightRobotID].attribute.taskY = updateRobotInfo[freightRobotID].attribute.robotDefaultY;

	PlanPath4to5(1, Map, freightRobotID);												//规划从传送带回默认位置的路径

	return 0;
}




//当所有任务完成后统一让所有机器人回去缺省坐标点
int AllRobotBack(vector<MapNode> Map)
{
	int robotId, robotStartX, robotStartY, robotEndX, robotEndY;

	for (int i = 0; i < updateRobotInfo.size(); i++) {
		if (updateRobotInfo[i].attribute.robotCurrentX != updateRobotInfo[i].attribute.robotDefaultX || updateRobotInfo[i].attribute.robotCurrentY != updateRobotInfo[i].attribute.robotDefaultY) {
			robotId = updateRobotInfo[i].attribute.robotId;
			robotStartX = updateRobotInfo[i].attribute.robotCurrentX;
			robotStartY = updateRobotInfo[i].attribute.robotCurrentY;
			robotEndX = updateRobotInfo[i].attribute.robotDefaultX;
			robotEndY = updateRobotInfo[i].attribute.robotDefaultY;

			updateRobotInfo[robotId - 1].robot_map = CreateMap(Map);

			updateRobotInfo[robotId - 1].highLevelPath.clear();
			updateRobotInfo[i].robot_path.clear();
			bool isSuccess = Astar(updateRobotInfo[robotId - 1].robot_map, mapWidth, mapHeight, robotStartX, robotStartY, robotEndX, robotEndY, updateRobotInfo[robotId - 1].robot_path);
			if (isSuccess == false) {
				if (robotStartX == robotEndX && robotStartY == robotEndY) {
					pushback(robotId - 1, 1);
				}
				else {
					int a = 0;
				}
			}
			updateRobotInfo[robotId - 1].attribute.robotNextX = updateRobotInfo[robotId - 1].robot_path[updateRobotInfo[robotId - 1].robot_path.size() - 1].x;
			updateRobotInfo[robotId - 1].attribute.robotNextY = updateRobotInfo[robotId - 1].robot_path[updateRobotInfo[robotId - 1].robot_path.size() - 1].y;

			updateRobotInfo[i].attribute.oncePathSize = updateRobotInfo[i].robot_path.size();
			updateRobotInfo[i].attribute.onceWorkPreiod = 0;

			updateRobotInfo[robotId - 1].attribute.robotStatus = -1;
			updateRobotInfo[robotId - 1].attribute.area = 0;
		}
	}

	return 0;
}
