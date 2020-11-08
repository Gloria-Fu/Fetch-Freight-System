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

	if (robotX >= areaX && robotX < areaX + shelfWidth && robotY >= areaY && robotY < areaY + shelfHeight) {			//�����˻���
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

		if (updateRobotInfo[i].robot_path.size() > 1) {													//�жϸû������Ƿ�����һ��������һ����·�������
			updateCurrentNode = updateRobotInfo[i].robot_path[updateRobotInfo[i].robot_path.size() - 1];					//��ȡ�û�����·������һ������
			updateNextNode = updateRobotInfo[i].robot_path[updateRobotInfo[i].robot_path.size() - 2];						//��ȡ�û�����·��������һ������

			updateRobotInfo[i].attribute.robotCurrentX = updateCurrentNode.x;
			updateRobotInfo[i].attribute.robotCurrentY = updateCurrentNode.y;
			updateRobotInfo[i].attribute.robotNextX = updateNextNode.x;
			updateRobotInfo[i].attribute.robotNextY = updateNextNode.y;
			updateRobotInfo[i].attribute.needUpdate = 1;

			updateRobotInfo[i].robot_path.pop_back();													//�Ѹ������Ӹû����˵�·����ɾ��

			if (updateRobotInfo[i].attribute.robotType != 0 && updateRobotInfo[i].attribute.robotStatus == 1 && updateRobotInfo[i].attribute.infoFlag == 0)
				TriggerInArea(i);
		}
		else if (updateRobotInfo[i].robot_path.size() == 1) {											//�жϸû������Ƿ�ֻʣһ��·������㣬���������дNULL����һ��ʼһ��
			updateCurrentNode = updateRobotInfo[i].robot_path[updateRobotInfo[i].robot_path.size() - 1];					//��ȡ�û�����·������һ������

			updateRobotInfo[i].attribute.robotCurrentX = updateCurrentNode.x;
			updateRobotInfo[i].attribute.robotCurrentY = updateCurrentNode.y;
			updateRobotInfo[i].attribute.robotNextX = updateCurrentNode.x;
			updateRobotInfo[i].attribute.robotNextY = updateCurrentNode.y;
			updateRobotInfo[i].attribute.needUpdate = 1;

			updateRobotInfo[i].robot_path.pop_back();													//�Ѹ������Ӹû����˵�·����ɾ��

			if (updateRobotInfo[i].highLevelPath.size() == 0) {							//������high-level·�������ˣ�֤���Ѿ�������ȫ��·�����յ㣨ȡ������ж�������һ��·���յ㣩
				if (updateRobotInfo[i].attribute.robotStatus == -1) {					//�������˹�λ״̬
					updateRobotInfo[i].attribute.robotStatus = 0;
					/*updateRobotInfo[i].attribute.taskX = 0;
					updateRobotInfo[i].attribute.taskY = 0;*/
				}
				else if (updateRobotInfo[i].attribute.robotStatus == 0) {				//�����˻ص�ȱʡ���ˣ����ž���
					updateRobotInfo[i].attribute.taskX = 0;
					updateRobotInfo[i].attribute.taskY = 0;
				}
				else {																	//��ͨ����ʱ״̬
					if (i < freightRobotNum) {											//֤�����˻������ˣ��˻����������·�������������
						if (updateRobotInfo[i].attribute.robotStatus == 1) {			//֤���������˻������˵�ȡ�������λ��	

							if ((updateRobotInfo[int(updateRobotInfo[i].attribute.relatedRobotId - 1)].attribute.robotStatus == 1) ||
								(updateRobotInfo[int(updateRobotInfo[i].attribute.relatedRobotId - 1)].attribute.robotStatus == 2 &&
									updateRobotInfo[int(updateRobotInfo[i].attribute.relatedRobotId - 1)].robot_path.size() > 0)) {	//�˻������˵�Ŀ����ˣ������������û������ԭ�صȣ���⵽robot״̬Ϊ2ʱ�Ϳ��Թ�����

								updateRobotInfo[i].attribute.waitTime++;
								taskPeriodTime[updateRobotInfo[i].attribute.taskId - 1].feiWaitTime++;

								updateRobotInfo[i].calBalance.balance++;
								updateRobotInfo[i].attribute.one_wayStep--;

								pushback(i, 1);
								updateRobotInfo[i].attribute.oncePathSize = updateRobotInfo[i].robot_path.size();
								updateRobotInfo[i].attribute.onceWorkPreiod = 0;
							}
							else if (updateRobotInfo[int(updateRobotInfo[i].attribute.relatedRobotId - 1)].attribute.robotStatus == 2 &&
								updateRobotInfo[int(updateRobotInfo[i].attribute.relatedRobotId - 1)].robot_path.size() == 0) {		//������ļ�������˺��˻������˾���������,��ʼ����
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
						else if (updateRobotInfo[i].attribute.robotStatus == 2) {			//��ǰ�˻������˴ӽ���״̬2��ɷ���״̬3
							TaskStatus3to4(i, Map);											//����״̬��3��4											
						}
						else if (updateRobotInfo[i].attribute.robotStatus == 3) {
							if (Map[updateRobotInfo[i].attribute.robotCurrentX + mapWidth * updateRobotInfo[i].attribute.robotCurrentY].nodeType == 4) {	//����λ���Ǵ��ʹ�������ж��
								TaskStatus4to5(i, period, Map);
							}
							else {
								UnloadGoods(i);
							}
						}
						else if (updateRobotInfo[i].attribute.robotStatus == 0) {						//��ȱʡ��״̬
							updateRobotInfo[i].robot_map.clear();
							updateRobotInfo[i].attribute.taskX = 0;
							updateRobotInfo[i].attribute.taskY = 0;
							updateRobotInfo[i].attribute.oncePathSize = 0;
							updateRobotInfo[i].attribute.onceWorkPreiod = 0;
						}
					}
					else {
						if (updateRobotInfo[i].attribute.robotStatus == 1) {							//��������˵���Ŀ�ĵ�
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
			else if (updateRobotInfo[i].highLevelPath.size() >= 1) {									//�����˵ĸ߲�·����û���֤꣬�����ڲ�û�е����һ��
				ReachNewNode(i, Map);
			}
		}
		else if (updateRobotInfo[i].robot_path.size() == 0) {											//û��·���Ļ����ˣ�ͣ��ԭ��
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
	updateRobotInfo[freightRobotId].attribute.goodsType = taskPeriodTime[updateRobotInfo[freightRobotId].attribute.taskId - 1].taskInfo.goodsType;		//1.��ѯ�˵Ļ�������

	taskPeriodTime[updateRobotInfo[freightRobotId].attribute.taskId - 1].taskInfo.status = 4;				//2.�ѵ�ǰ����״̬��3�޸ĳ�4
	taskPeriodTime[updateRobotInfo[freightRobotId].attribute.taskId - 1].pickRobotWorkEndTime = globalPeriod;
	
	//LeaveGoodsArea(Map, freightRobotId);								//�滮�ӻ����ڵ����ʹ���·��
	PlanPath3to4(1, Map, freightRobotId);

	GoodsContents[updateRobotInfo[updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1].attribute.goodsId - 1].goods_status = 0;

	updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.robotStatus = 0;		//��ǰ�˻��������ѵ���Ŀ��㣬��������˽���������ɣ�״̬���0:����
	updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.taskX = updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.robotDefaultX;
	updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.taskY = updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.robotDefaultY;
	updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.taskId = 0;
	updateRobotInfo[int(updateRobotInfo[freightRobotId].attribute.relatedRobotId - 1)].attribute.relatedRobotId = 0;		//�Ѽ�������˵Ĺ��������˱����Ϊ��

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
	updateRobotInfo[freightRobotId].attribute.relatedRobotId = 0;														//���˻������˵Ĺ��������˱����Ϊ��
	updateRobotInfo[freightRobotId].attribute.robotStatus = 3;

	
	return 0;
}



int TaskStatus4to5(int freightRobotID, int currentPeriod, vector<MapNode> Map)
{
	int area = (updateRobotInfo[freightRobotID].attribute.goodsId - 1) / eachGoodsNum;
	taskProcess[area].finishedTaskNum++;

	updateRobotInfo[freightRobotID].attribute.goodsType = 0;

	if (taskProcess[area].taskNum == taskProcess[area].finishedTaskNum) {				//�жϸ������Ƿ��������������
		taskProcess[area].isAllFinished = 1;
		finishAreaNum++;
		taskProcess[area].finishPeriod = currentPeriod;
	}

	taskPeriodTime[updateRobotInfo[freightRobotID].attribute.taskId - 1].taskInfo.status = 5;
	taskPeriodTime[updateRobotInfo[freightRobotID].attribute.taskId - 1].endPeriod = globalPeriod;
	taskPeriodTime[updateRobotInfo[freightRobotID].attribute.taskId - 1].freightRobotWorkEndTime = globalPeriod;

	CalReward(updateRobotInfo[freightRobotID].attribute.taskId - 1, area);
	
	updateRobotInfo[freightRobotID].attribute.one_wayStep = 0;
	updateRobotInfo[freightRobotID].attribute.robotStatus = 0;							//���˻������˵�״̬��Ϊ0:���У��ɷ���������µ�����

	if (updateRobotInfo[freightRobotID].attribute.robotType == 2) 						//����ǻ��������ˣ����������������Ҫ��������
		updateRobotInfo[freightRobotID].attribute.area = 0;
	updateRobotInfo[freightRobotID].attribute.taskId = 0;

	updateRobotInfo[freightRobotID].attribute.taskX = updateRobotInfo[freightRobotID].attribute.robotDefaultX;
	updateRobotInfo[freightRobotID].attribute.taskY = updateRobotInfo[freightRobotID].attribute.robotDefaultY;

	PlanPath4to5(1, Map, freightRobotID);												//�滮�Ӵ��ʹ���Ĭ��λ�õ�·��

	return 0;
}




//������������ɺ�ͳһ�����л����˻�ȥȱʡ�����
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
