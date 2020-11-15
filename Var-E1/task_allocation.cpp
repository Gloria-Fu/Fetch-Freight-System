#include "task_allocation.h"
#include "conflict_resolution.h"
#include "calculate.h"
#include "goods_area.h"
#include "path_planning.h"
#include "update_file.h"




int AllocateTask2Area(void)
{
	//��GoodsInformation�Ÿ�����
	for (int id = 0; id < goodsTypeNum; id++) {
		for (unsigned int i = 0; i < GoodsInformation[id].size(); i++) {
			for (unsigned int j = i + 1; j < GoodsInformation[id].size(); j++) {
				if (GoodsInformation[id][i].goods_X < GoodsInformation[id][j].goods_X) {
					swap(GoodsInformation[id][i], GoodsInformation[id][j]);
				}
				else if (GoodsInformation[id][i].goods_X == GoodsInformation[id][j].goods_X) {
					if (GoodsInformation[id][i].goods_Y > GoodsInformation[id][j].goods_Y)
						swap(GoodsInformation[id][i], GoodsInformation[id][j]);
				}
			}
		}
	}

	int taskID = 0;
	int area = 0;

	vector<int> maxTaskNum;
	vector<int> taskRequirement = getTask(goodsTypeNum);		//��ȡÿ���������������

	for (int i = 0; i < goodsTypeNum; i++) {
		maxTaskNum.push_back(0);
		for (int j = 0; j < GoodsInformation[i].size(); j++) {
			maxTaskNum[i] += GoodsInformation[i][j].goods_Num;	//���ÿ�����������
		}
		if (taskRequirement[i] > maxTaskNum[i]) {
			taskRequirement[i] = maxTaskNum[i];					//���������󳬹������������ޣ����������
		}
	}


	int goods = 0;
	for (int i = 0; i < taskRequirement.size(); i++) {
		int goodsTypeID = i;
		goods = 0;
		for (int j = 0; j < taskRequirement[i];) {				//task[i]<=Goods[i]�ܺ�
			if (GoodsInformation[goodsTypeID][goods].goods_Num > 0) {
				j++;
				GoodsInformation[goodsTypeID][goods].goods_Num--;

				taskID++;
				task newTask;

				newTask.goodsX = GoodsInformation[goodsTypeID][goods].goods_X;
				newTask.goodsY = GoodsInformation[goodsTypeID][goods].goods_Y;
				newTask.taskId = taskID;
				newTask.goodsId = GoodsInformation[goodsTypeID][goods].goods_Id;
				newTask.goodsType = GoodsInformation[goodsTypeID][goods].goods_Type;
				newTask.pickingRobotId = 0;
				newTask.freightRobotId = 0;
				newTask.status = 0;
				area = (GoodsInformation[goodsTypeID][goods].goods_Id - 1) / eachGoodsNum;

				taskProcess[area].updateTask0(newTask);

				taskRecord newtaskInfo;

				newtaskInfo.taskInfo = newTask;

				taskPeriodTime.push_back(newtaskInfo);
			}
			else {
				goods++;
			}
		}
	}

	return 0;
}




int AssignPickingRobot(vector<MapNode> Map)
{
	for (int i = 0; i < pickingRobotNum; i++) {
		int pickingRobotId = i + 1 + freightRobotNum;
		int areaId = updateRobotInfo[pickingRobotId - 1].attribute.area;
		if (taskProcess[areaId - 1].unFinishedTask.size() > 0) {

			if (updateRobotInfo[pickingRobotId - 1].attribute.robotStatus == 0) {						//�û����˿���
				int taskIndex = 0;
				while (true) {
					if (taskIndex < taskProcess[areaId - 1].unFinishedTask.size()) {					//�Ƿ񳬳���δ�������ķ�Χ��
						task currentTask = taskProcess[areaId - 1].unFinishedTask[taskIndex];
						int goodsID = currentTask.goodsId;
						int row = currentTask.goodsY;

						bool rowIsNull = true;
						if (GoodsContents[goodsID - 1].goods_status == 1)
							rowIsNull = false;

						bool rowIsNull2 = true; //checkNearby(currentTask.goodsX, currentTask.goodsY, areaId, pickingRobotId - 1);

						bool rowIsNull3 = checkIsInArea(pickingRobotId - 1);


						if (GoodsContents[goodsID - 1].goods_status == 0) {									//֤�����λ����ʱû�����˹�
							if (rowIsNull == true && rowIsNull2 == true && rowIsNull3 == true) {
								taskProcess[areaId - 1].updateTask1(pickingRobotId, currentTask.goodsX, currentTask.goodsY, taskIndex);
								taskPeriodTime[currentTask.taskId - 1].taskInfo.goodsX = currentTask.goodsX;
								taskPeriodTime[currentTask.taskId - 1].taskInfo.goodsY = currentTask.goodsY;

								if (currentTask.taskId == 2968)
									int a = 0;

								//2.����Ҫ����Ļ���״̬��Ϣ
								GoodsContents[goodsID - 1].goods_status = 1;

								//3.�鿴��ǰ����ڵ��λ����Ϣ
								updateRobotInfo[pickingRobotId - 1].robot_map = CreateFreightRobotMap(areaId, Map);

								int aboveNodeType = updateRobotInfo[pickingRobotId - 1].robot_map[currentTask.goodsX + mapWidth * (currentTask.goodsY - 1)].isAccessible;
								int pickingTargetNodeX = 0;
								int pickingTargetNodeY = 0;
								if (aboveNodeType == 0) {													//����ͨ�У����Ŀ�������Ϸ���Ϊ��������������
									pickingTargetNodeX = currentTask.goodsX;
									pickingTargetNodeY = currentTask.goodsY - 1;
								}
								else if (aboveNodeType == 1) {												//������ͨ�У����Ŀ�������·���Ϊ��������������
									pickingTargetNodeX = currentTask.goodsX;
									pickingTargetNodeY = currentTask.goodsY + 1;
								}
								updateRobotInfo[pickingRobotId - 1].attribute.taskX = pickingTargetNodeX;	//���¼�������˵�Ŀ��������
								updateRobotInfo[pickingRobotId - 1].attribute.taskY = pickingTargetNodeY;	//���¼�������˵�Ŀ���������
								updateRobotInfo[pickingRobotId - 1].attribute.robotStatus = 1;
								updateRobotInfo[pickingRobotId - 1].attribute.taskId = currentTask.taskId;
								updateRobotInfo[pickingRobotId - 1].attribute.goodsId = currentTask.goodsId;

								updateRobotInfo[pickingRobotId - 1].robot_path.clear();										//��ȻҪ���¹滮�ˣ��Ͱ�ԭ���ĵ�·���һ��
								updateRobotInfo[pickingRobotId - 1].attribute.onceWorkPreiod = 0;

								if (updateRobotInfo[pickingRobotId - 1].attribute.robotCurrentX == updateRobotInfo[pickingRobotId - 1].attribute.taskX
									&& updateRobotInfo[pickingRobotId - 1].attribute.robotCurrentY == updateRobotInfo[pickingRobotId - 1].attribute.taskY) {
									pushback(pickingRobotId - 1, 1);
									updateRobotInfo[pickingRobotId - 1].attribute.oncePathSize = 5;
									updateRobotInfo[pickingRobotId - 1].attribute.onceWorkPreiod = 0;
								}
								else {
									bool isSuccess = useAStar(pickingRobotId - 1, updateRobotInfo[pickingRobotId - 1].robot_map);	//�滮�������·��
									if (isSuccess == false) {
										if (updateRobotInfo[pickingRobotId - 1].attribute.robotCurrentX == updateRobotInfo[pickingRobotId - 1].attribute.taskX && updateRobotInfo[pickingRobotId - 1].attribute.robotCurrentY == updateRobotInfo[pickingRobotId - 1].attribute.taskY) {
											pushback(pickingRobotId - 1, 1);
										}
										else {
											int a = 0;
										}
									}

									updateRobotInfo[pickingRobotId - 1].attribute.oncePathSize = updateRobotInfo[pickingRobotId - 1].robot_path.size();
									updateRobotInfo[pickingRobotId - 1].attribute.onceWorkPreiod = 0;
								}
								taskPeriodTime[currentTask.taskId - 1].taskInfo.status = 1;
								updateRobotInfo[pickingRobotId - 1].attribute.robotNextX = updateRobotInfo[pickingRobotId - 1].robot_path[updateRobotInfo[pickingRobotId - 1].robot_path.size() - 1].x;
								updateRobotInfo[pickingRobotId - 1].attribute.robotNextY = updateRobotInfo[pickingRobotId - 1].robot_path[updateRobotInfo[pickingRobotId - 1].robot_path.size() - 1].y;

								taskPeriodTime[currentTask.taskId - 1].startPeriod = globalPeriod;
								taskPeriodTime[currentTask.taskId - 1].taskInfo.pickingRobotId = pickingRobotId;
								taskPeriodTime[currentTask.taskId - 1].pickRobotWorkStartTime = globalPeriod;

								break;
							}
							else {
								taskIndex++;
							}
						}
						else {
							taskIndex++;
						}
					}
					else {
						break;
					}
				}
			}
		}
	}

	return 0;
}


int AssignFreightRobot(vector<MapNode> Map)
{
	int areaId = 0;
	task currentTask;

	for (int i = 0; i < areaNum; i++) {
		areaId = i + 1;
		if (taskProcess[i].taskNum > 0) {
			while (taskProcess[i].pickingTask.size() != 0) {
				currentTask = taskProcess[i].pickingTask.front();
				int value = mapWidth * mapHeight;
				int freightRobot = -1;
				for (int j = 0; j < freightRobotNum; j++) {
					if ((updateRobotInfo[j].attribute.area == areaId || updateRobotInfo[j].attribute.area == 0) && updateRobotInfo[j].attribute.robotStatus == 0) {
						int distance = abs(currentTask.goodsX - updateRobotInfo[j].attribute.robotCurrentX) + abs(currentTask.goodsY - updateRobotInfo[j].attribute.robotCurrentY);
						if (distance < value) {
							value = distance;
							freightRobot = j;
						}
					}
				}

				if (freightRobot >= 0) {
					if (updateRobotInfo[freightRobot].attribute.robotType == 2) {
						updateRobotInfo[freightRobot].attribute.area = areaId;
					}
					taskProcess[i].updateTask2(freightRobot + 1);

					if (freightRobot == 167 && globalPeriod > 0)
						int a = 0;

					updateRobotInfo[freightRobot].attribute.relatedRobotId = currentTask.pickingRobotId;
					updateRobotInfo[updateRobotInfo[freightRobot].attribute.relatedRobotId - 1].attribute.relatedRobotId = freightRobot + 1;
					updateRobotInfo[freightRobot].attribute.robotStatus = 1;
					updateRobotInfo[freightRobot].attribute.taskId = currentTask.taskId;
					updateRobotInfo[freightRobot].attribute.goodsId = currentTask.goodsId;
					taskPeriodTime[currentTask.taskId - 1].taskInfo.status = 2;
					taskPeriodTime[currentTask.taskId - 1].taskInfo.freightRobotId = freightRobot + 1;
					taskPeriodTime[currentTask.taskId - 1].freightRobotWorkStartTime = globalPeriod;

					PlanPath1to2(1, Map, freightRobot);
				}
				else {
					break;
				}
			}
		}
	}

	return 0;
}


struct NumIndex {
	int num;
	int index;
};

//�����˷����㷨�ĵ���
int RobotAllocationMethods(int param)
{
	if (param == 1) {				//1.ʹ��Ԥ����������㷨
		preAllocate();
	}
	else if (param == 2) {			//2.ʹ�ö�̬����������㷨part-one
		if (globalPeriod % 1000 == 0)
			ReAssign(globalPeriod);								//��;�����˻�������
	}
	else if (param == 3) {			//3.ʹ�ö�̬����������㷨part-two
		HelpOtherArea();														//����������ɺ���Ԯ��������
	}

	return 0;
}


//1.Ԥ����������㷨
int preAllocate(void)
{
	int sumPickRobotNum = pickingRobotNum;
	int sumFreiRobotNum = freightRobotNum;

	int sumPickRobotNum1 = sumPickRobotNum;
	int sumFreiRobotNum1 = sumFreiRobotNum;

	int pickDistance = 20;
	int extraDistance = 0;

	for (int i = 0; i < areaNum; i++) {
		taskProcess[i].taskNum = taskProcess[i].unFinishedTask.size();
		taskProcess[i].pickDistance = pickDistance;												//����ļ�������˾���

		int area_x = start_x + i / (shelfYnum + 1) * (shelfWidth + road) + (shelfWidth / 2);
		int area_y = start_y + i % (shelfYnum + 1) * (shelfHeight + road) + (shelfHeight / 2);
		int distance = 1000, index = 0;
		for (int node = 0; node < EndNode.size(); node++) {
			if ((abs(area_x - EndNode[node].x) + abs(area_y - EndNode[node].y)) < distance) {
				distance = abs(area_x - EndNode[node].x) + abs(area_y - EndNode[node].y);
			}
		}
		taskProcess[i].freiDistance = 2 * distance;

		if (taskProcess[i].unFinishedTask.size() > 0) {			   //������������Ϊ0������һ�˷���һ��������˻�������
			taskProcess[i].pickRobotNum++;
			sumPickRobotNum--;
			taskProcess[i].freiRobotNum++;
			sumFreiRobotNum--;
		}
	}

	int maxTime = 0, tempTime = 0;
	int areaId = -1;
	vector<NumIndex> areaInfo;
	NumIndex node;
	for (int i = 0; i < areaNum; i++) {
		if (taskProcess[i].taskNum > 0) {
			areaInfo.push_back(node);
		}
	}
	while (true) {
		for (int i = 0, j = 0; i < areaNum; i++) {
			if (taskProcess[i].taskNum > 0) {
				areaInfo[j].index = i;
				areaInfo[j].num = taskProcess[i].calTaskTime(taskProcess[i].taskNum);
				j++;
			}
		}

		for (unsigned int i = 0; i < areaInfo.size(); i++) {
			for (unsigned int j = i + 1; j < areaInfo.size(); j++) {
				if (areaInfo[i].num > areaInfo[j].num) {
					swap(areaInfo[i], areaInfo[j]);
				}
			}
		}

		int area = areaInfo.size() - 1;

		while (true) {
			areaId = areaInfo[area].index;
			if (taskProcess[areaId].pickOrfrei == 0) {			//ȱ���������
				if (sumPickRobotNum > 0 && taskProcess[areaId].pickRobotNum < 10) {
					taskProcess[areaId].pickRobotNum++;
					sumPickRobotNum--;
					break;
				}
			}
			else {												//ȱ�˻�������
				if (sumFreiRobotNum > 0 && taskProcess[areaId].freiRobotNum < 20) {
					taskProcess[areaId].freiRobotNum++;
					sumFreiRobotNum--;
					break;
				}
			}
			area--;
			if (area == -1) {
				break;
			}
		}

		if (sumPickRobotNum == 0 && sumFreiRobotNum == 0)	//�������
			break;

		if ((sumPickRobotNum != 0 && sumFreiRobotNum == 0) || (sumPickRobotNum == 0 && sumFreiRobotNum != 0)) {
			vector<int> maxTime, maxIndex;

			for (int i = 0; i < areaNum; i++) {
				if (taskProcess[i].taskNum > 0) {
					tempTime = taskProcess[i].calTaskTime(taskProcess[i].taskNum);
					maxTime.push_back(tempTime);
					maxIndex.push_back(i);
				}
			}

			for (unsigned int i = 0; i < maxTime.size(); i++) {
				for (unsigned int j = i + 1; j < maxTime.size(); j++) {
					if (maxTime[i] < maxTime[j]) {
						swap(maxTime[i], maxTime[j]);
						swap(maxIndex[i], maxIndex[j]);
					}
				}
			}

			int success = -1;
			if (sumPickRobotNum > 0) {
				for (int i = 0; i < maxTime.size(); i++) {
					if (taskProcess[maxIndex[i]].pickOrfrei == 0 && taskProcess[maxIndex[i]].pickRobotNum < 10) {
						taskProcess[maxIndex[i]].pickRobotNum++;
						sumPickRobotNum--;
						success = 1;
						break;
					}
				}

				if (success != 1) {
					vector<int> empty1;
					swap(empty1, maxTime);
					vector<int> empty2;
					swap(empty2, maxIndex);

					for (int i = 0; i < areaNum; i++) {
						if (taskProcess[i].taskNum > 0) {
							maxTime.push_back(taskProcess[i].taskNum);
							maxIndex.push_back(i);
						}
					}
					for (unsigned int i = 0; i < maxTime.size(); i++) {
						for (unsigned int j = i + 1; j < maxTime.size(); j++) {
							if (taskProcess[i].taskNum < taskProcess[j].taskNum) {
								swap(maxTime[i], maxTime[j]);
								swap(maxIndex[i], maxIndex[j]);
							}
						}
					}

					for (int j = 0; j < maxTime.size(); j++) {
						if (taskProcess[maxIndex[j]].pickRobotNum < 10) {
							taskProcess[maxIndex[j]].pickRobotNum++;
							sumPickRobotNum--;
							if (sumPickRobotNum == 0) {
								break;
							}
						}
					}
				}
			}
			else if (sumFreiRobotNum > 0) {
				for (int i = 0; i < maxTime.size(); i++) {
					if (taskProcess[maxIndex[i]].pickOrfrei == 1 && taskProcess[maxIndex[i]].freiRobotNum < 20) {
						taskProcess[maxIndex[i]].freiRobotNum++;
						sumFreiRobotNum--;
						success = 1;
						break;
					}
				}

				if (success != 1) {
					vector<int> empty1;
					swap(empty1, maxTime);
					vector<int> empty2;
					swap(empty2, maxIndex);

					for (int i = 0; i < areaNum; i++) {
						if (taskProcess[i].taskNum > 0) {
							maxTime.push_back(taskProcess[i].taskNum);
							maxIndex.push_back(i);
						}
					}
					for (unsigned int i = 0; i < maxTime.size(); i++) {
						for (unsigned int j = i + 1; j < maxTime.size(); j++) {
							if (taskProcess[i].taskNum < taskProcess[j].taskNum) {
								swap(maxTime[i], maxTime[j]);
								swap(maxIndex[i], maxIndex[j]);
							}
						}
					}

					for (int j = 0; j < maxTime.size(); j++) {
						if (taskProcess[maxIndex[j]].freiRobotNum < 20) {
							taskProcess[maxIndex[j]].freiRobotNum++;
							sumFreiRobotNum--;
							if (sumFreiRobotNum == 0) {
								break;
							}
						}
					}
				}
			}
		}
	}

	return 0;
}


//2.��̬�����������㷨part-one
int ReAssign(int period)
{
	if (finishAreaNum == 0) {

		int maxPeriod = 0;
		int minPeriod = 100000;
		int value = 0;
		int maxArea = 0, minArea = 0;

		vector<int> currentTime;
		vector<int> sortArea;
		for (int i = 0; i < areaNum; i++) {
			if (taskProcess[i].taskNum > 0 && taskProcess[i].finishedTaskNum > 0) {
				value = (period / taskProcess[i].finishedTaskNum) * (taskProcess[i].taskNum - taskProcess[i].finishedTaskNum);
				sortArea.push_back(i);
				currentTime.push_back(value);
			}
		}

		for (unsigned int i = 0; i < currentTime.size(); i++) {
			for (unsigned int j = i + 1; j < currentTime.size(); j++) {
				if (currentTime[i] > currentTime[j]) {
					swap(currentTime[i], currentTime[j]);
					swap(sortArea[i], sortArea[j]);
				}
			}
		}

		int success = 0, oldTime = 0, newTime = 0;

		vector<int> sortArea1 = sortArea;
		vector<int> currentTime1 = currentTime;
		int size = sortArea.size();
		while (sortArea1.size() > size / 2) {				//����֮һ�Ļ��ᣬ��������֮һ����ҪԮ���Ͳ�����
			maxArea = sortArea1.back();

			oldTime = taskProcess[maxArea].calTaskTime(taskProcess[maxArea].taskNum - taskProcess[maxArea].finishedTaskNum);
			taskProcess[maxArea].freiRobotNum++;
			newTime = taskProcess[maxArea].calTaskTime(taskProcess[maxArea].taskNum - taskProcess[maxArea].finishedTaskNum);

			if (oldTime == newTime) {						//����Ԯ���ʱ��û�б仯��֤���˻������˱��ͣ��Ͳ�������������ˣ��Ա�Ԯ���Ĺ��ƣ�
				taskProcess[maxArea].freiRobotNum--;
				success = -1;
				sortArea1.pop_back();
				currentTime1.pop_back();
			}
			else {
				success = 1;
				break;
			}
		}

		if (success == 1) {												//��ҪȥԮ�����������жϣ�����������ʱ���Ϊ�������ģ��򲻵���
			vector<int> sortArea2 = sortArea;
			vector<int> currentTime2 = currentTime;

			for (int i = 0; i < size; i++) {
				minArea = sortArea2[i];

				if (taskProcess[minArea].freiRobotNum > 1) {
					taskProcess[minArea].freiRobotNum--;
					newTime = taskProcess[minArea].calTaskTime(taskProcess[minArea].taskNum - taskProcess[minArea].finishedTaskNum);

					int maxtime = currentTime2.back();

					if (newTime < maxtime) {
						success = 2;
						for (int i = updateRobotInfo.size() - 1; i >= 0; i--) {
							if (updateRobotInfo[i].attribute.robotType == 1 && updateRobotInfo[i].attribute.area == minArea + 1 && (updateRobotInfo[i].attribute.robotStatus == 0 || updateRobotInfo[i].attribute.robotStatus == 3)) {
								updateRobotInfo[i].attribute.area = maxArea + 1;
								if (taskProcess[maxArea].freiRobotNum > 11) {
									updateRobotInfo[i].attribute.robotDefaultX = (start_x - 1) + (maxArea / shelfYnum) * (shelfWidth + road) - 1;
									updateRobotInfo[i].attribute.robotDefaultY = start_y - 1 + (maxArea % shelfYnum) * (shelfHeight + road) + taskProcess[maxArea].freiRobotNum - 11;
								}
								else {
									updateRobotInfo[i].attribute.robotDefaultX = (start_x - 1) + (maxArea / shelfYnum) * (shelfWidth + road);
									updateRobotInfo[i].attribute.robotDefaultY = start_y - 1 + (maxArea % shelfYnum) * (shelfHeight + road) + taskProcess[maxArea].freiRobotNum;
								}
								success = 1;
								break;
							}
						}
						break;
					}
					else {
						success = -1;
						taskProcess[minArea].freiRobotNum++;
					}
				}
			}
			if (success == -1)
				taskProcess[maxArea].freiRobotNum--;
		}
	}
	return 0;
}

//3.��̬�����������㷨part-two
int HelpOtherArea(void)
{
	//������������robot��Ϊ����robot
	for (int i = 0; i < areaNum; i++) {
		if (taskProcess[i].taskNum > 0) {
			if (taskProcess[i].isAllFinished == 1) {
				for (int j = 0; j < freightRobotNum; j++) {
					if (updateRobotInfo[j].attribute.area == i + 1) {
						updateRobotInfo[j].attribute.area = 0;
						updateRobotInfo[j].attribute.robotType = 2;
						taskProcess[i].isAllFinished = 2;
					}
				}
			}
		}
	}

	return 0;
}