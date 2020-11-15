#include "conflict_resolution.h"
#include "path_planning.h"
#include "conflict_priority.h"
#include "goods_area.h"
#include "calculate.h"
#include "update_file.h"



vector<RobotInfo> update_robot_info_backup;


int CheckNeighborhood()
{
	Node2D fourNeighbors[4];
	int directions[4][2] = { {0,-1},{-1,0},{1,0},{0,1} };			//������

	int robotA = -1, robotB = -1;
	int rePlanRobotId, obstacleRobotId;
	int isInFour = 0;
	int isReplan = 0;
	int isExchange = 0;

	int newStartX, newStartY, newEndX, newEndY;
	int status = 1;
	vector<PathNode> tempPath;							//������ʱ����·��

	update_robot_info_backup = updateRobotInfo;

	//�ӱ�Ÿߵ��ͽ��б��������Խ�ߣ����ȼ�Խ��
	for (int i = updateRobotInfo.size() - 1; i >= 0; i--) {

		if (globalPeriod == 125 && i == 186)
			int a = 0;

		RobotAdjustment(i);

		for (int j = 0; j < 4; j++) {					//��¼��ÿ�������˵�ǰ����������������Ļ����˱��
			fourNeighbors[j].x = updateRobotInfo[i].attribute.robotCurrentX + directions[j][0];
			fourNeighbors[j].y = updateRobotInfo[i].attribute.robotCurrentY + directions[j][1];
		}

		for (int k = updateRobotInfo.size() - 1; k >= 0; k--) {
			if (k != i) {								//������ͬ�Ļ�����

				if (globalPeriod == 35 && i == 320)
					int a = 0;

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////֤�����������˴�ʱ�ص���
				if (updateRobotInfo[i].attribute.robotCurrentX == updateRobotInfo[k].attribute.robotCurrentX && updateRobotInfo[i].attribute.robotCurrentY == updateRobotInfo[k].attribute.robotCurrentY) {
					cout << "\n\n   !!WARNING!!  \n\n" << endl;
					return -1;							//�������󾯸�
				}
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//1.���������
				for (int p = 0; p < 4; p++) {

					if (updateRobotInfo[k].attribute.robotCurrentX == fourNeighbors[p].x && updateRobotInfo[k].attribute.robotCurrentY == fourNeighbors[p].y) {	//k��i����������

						if (updateRobotInfo[k].attribute.relatedRobotId == i + 1) {		//��ʾk��i�Ǻ�����ϵ
							checkIsHandover(i, k, update_robot_info_backup[k].robot_map);
						}
						isInFour = 1;
						//���k+1�Ļ������ڱ��i+1�Ļ�������������,�һ������һ�����ص� 
						if (updateRobotInfo[i].attribute.robotCurrentX == updateRobotInfo[k].attribute.robotNextX && updateRobotInfo[i].attribute.robotCurrentY == updateRobotInfo[k].attribute.robotNextY &&
							updateRobotInfo[i].attribute.robotNextX == updateRobotInfo[k].attribute.robotCurrentX && updateRobotInfo[i].attribute.robotNextY == updateRobotInfo[k].attribute.robotCurrentY) {

							rePlanRobotId = PriorityDecideMethods(2, i, k);
							obstacleRobotId = ((rePlanRobotId == i) ? k : i);

							isReplan = 1;											//�����л�������Ҫ�����¹滮

							update_robot_info_backup[rePlanRobotId].robot_map[updateRobotInfo[obstacleRobotId].attribute.robotCurrentX + updateRobotInfo[obstacleRobotId].attribute.robotCurrentY * mapWidth].isAccessible = 1;	//���Լ��ĵ�ͼ�ϱ�־�Է�λ��Ϊ�ϰ�
							update_robot_info_backup[obstacleRobotId].robot_map[updateRobotInfo[rePlanRobotId].attribute.robotCurrentX + updateRobotInfo[rePlanRobotId].attribute.robotCurrentY * mapWidth].isAccessible = 1;
						}
						//���k+1�Ļ������ڱ��i+1�Ļ�������������,��������A����һ���ǻ�����B�ĵ�ǰ���꣬�û�����B���ߣ�A��
						else {
							if (updateRobotInfo[i].attribute.robotCurrentX == updateRobotInfo[k].attribute.robotNextX && updateRobotInfo[i].attribute.robotCurrentY == updateRobotInfo[k].attribute.robotNextY) {
								if ((updateRobotInfo[i].attribute.robotCurrentX == updateRobotInfo[i].attribute.robotNextX && updateRobotInfo[i].attribute.robotCurrentY == updateRobotInfo[i].attribute.robotNextY) ||
									(updateRobotInfo[i].attribute.robotNextX == 0 && updateRobotInfo[i].attribute.robotNextY == 0) ||
									(i < freightRobotNum && updateRobotInfo[i].attribute.robotStatus == 3 && updateRobotInfo[i].robot_path.size() <= 5) ||					//���˻����������յ㴦ж��ʱ����ֹ���������䵱���ϰ���
									(i >= freightRobotNum && (updateRobotInfo[i].attribute.robotStatus == 2 || updateRobotInfo[i].attribute.robotStatus == 3))) {	//��������˾�ֹ�ڻ���ǰ״̬
																					//���������ʾ������i��ԭ�ز����ģ��൱���ϰ���,����Ҫ�滮·�������Բ���Ҫ�ڵ�ͼ�ϱ��
									update_robot_info_backup[k].robot_map[updateRobotInfo[i].attribute.robotCurrentX + updateRobotInfo[i].attribute.robotCurrentY * mapWidth].isAccessible = 1;	//������һ��������������Ϊ�ϰ�
									rePlanRobotId = k;								//������kҪ���¹滮·����i���ֲ���
									obstacleRobotId = i;
									isReplan = 3;									//������Ҫ���¹滮
								}
								else {												//����������kҪ�ߵ�������i�ĵ�ǰλ�ã���i����һ����k�ȴ�һ��
									if (updateRobotInfo[i].robot_path.size() > 0) {					//ȷ��������i����·���ģ��ܹ��߿���·��k��
										update_robot_info_backup[i].robot_map[updateRobotInfo[k].attribute.robotCurrentX + updateRobotInfo[k].attribute.robotCurrentY * mapWidth].isAccessible = 1;	//������һ��������������Ϊ�ϰ�
										update_robot_info_backup[k].robot_map[updateRobotInfo[i].attribute.robotCurrentX + updateRobotInfo[i].attribute.robotCurrentY * mapWidth].isAccessible = 1;	//������һ��������������Ϊ�ϰ�

										robotA = i;		//��
										robotB = k;		//��

										pushback(robotB, 1);
										updateRobotInfo[robotB].attribute.robotNextX = updateRobotInfo[robotB].robot_path[updateRobotInfo[robotB].robot_path.size() - 1].x;		//�ѻ����˵���һ���������Ϊ��ǰ�����
										updateRobotInfo[robotB].attribute.robotNextY = updateRobotInfo[robotB].robot_path[updateRobotInfo[robotB].robot_path.size() - 1].y;
									}
								}
							}
							else if (updateRobotInfo[i].attribute.robotNextX == updateRobotInfo[k].attribute.robotCurrentX && updateRobotInfo[i].attribute.robotNextY == updateRobotInfo[k].attribute.robotCurrentY) {
								if (updateRobotInfo[k].attribute.robotCurrentX == updateRobotInfo[k].attribute.robotNextX && updateRobotInfo[k].attribute.robotCurrentY == updateRobotInfo[k].attribute.robotNextY ||
									(updateRobotInfo[k].attribute.robotNextX == 0 && updateRobotInfo[k].attribute.robotNextY == 0) ||
									(k < freightRobotNum && updateRobotInfo[k].attribute.robotStatus == 3 && updateRobotInfo[k].robot_path.size() <= 5) ||					//���˻����������յ㴦ж��ʱ����ֹ���������䵱���ϰ���
									(k >= freightRobotNum && (updateRobotInfo[k].attribute.robotStatus == 2 || updateRobotInfo[k].attribute.robotStatus == 3))) {	//��������˾�ֹ�ڻ���ǰ״̬

									//���������ʾ������k��ԭ�ز����ģ��൱���ϰ���
									update_robot_info_backup[i].robot_map[updateRobotInfo[k].attribute.robotCurrentX + updateRobotInfo[k].attribute.robotCurrentY * mapWidth].isAccessible = 1;	//������һ��������������Ϊ�ϰ�
									rePlanRobotId = i;								//������iҪ���¹滮·����i���ֲ���
									obstacleRobotId = k;
									isReplan = 3;									//������Ҫ���¹滮
								}
								else {												//����������BҪ�ߵ�������A�ĵ�ǰλ�ã���A����һ����B�ȴ�һ��
									if (updateRobotInfo[k].robot_path.size() > 0) {
										update_robot_info_backup[i].robot_map[updateRobotInfo[k].attribute.robotCurrentX + updateRobotInfo[k].attribute.robotCurrentY * mapWidth].isAccessible = 1;	//������һ��������������Ϊ�ϰ�
										update_robot_info_backup[k].robot_map[updateRobotInfo[i].attribute.robotCurrentX + updateRobotInfo[i].attribute.robotCurrentY * mapWidth].isAccessible = 1;	//������һ��������������Ϊ�ϰ�

										robotA = k;		//��
										robotB = i;		//��

										pushback(robotB, 1);
										updateRobotInfo[robotB].attribute.robotNextX = updateRobotInfo[robotB].robot_path[updateRobotInfo[robotB].robot_path.size() - 1].x;		//�ѻ����˵���һ���������Ϊ��ǰ�����
										updateRobotInfo[robotB].attribute.robotNextY = updateRobotInfo[robotB].robot_path[updateRobotInfo[robotB].robot_path.size() - 1].y;
									}
								}
							}
							else {													//����Ӱ�죬����ҲҪ���
								if (update_robot_info_backup[i].robot_map.size() == mapWidth * mapHeight) {
									update_robot_info_backup[i].robot_map[updateRobotInfo[k].attribute.robotCurrentX + updateRobotInfo[k].attribute.robotCurrentY * mapWidth].isAccessible = 1;	//������һ��������������Ϊ�ϰ�
								}
								if (update_robot_info_backup[k].robot_map.size() == mapWidth * mapHeight) {
									update_robot_info_backup[k].robot_map[updateRobotInfo[i].attribute.robotCurrentX + updateRobotInfo[i].attribute.robotCurrentY * mapWidth].isAccessible = 1;	//������һ��������������Ϊ�ϰ�
								}
							}
						}
					}
				}

				//2.���k+1�Ļ����˲��ڱ��i+1�Ļ������������ڣ�����һ���������˽��ᵽ��ͬһ�����
				if (isInFour == 0) {
					if (updateRobotInfo[i].attribute.robotNextX == updateRobotInfo[k].attribute.robotNextX && updateRobotInfo[i].attribute.robotNextY == updateRobotInfo[k].attribute.robotNextY) {

						rePlanRobotId = PriorityDecideMethods(2, i, k);
						obstacleRobotId = ((rePlanRobotId == i) ? k : i);
						
						isReplan = 2;
						update_robot_info_backup[rePlanRobotId].robot_map[updateRobotInfo[obstacleRobotId].attribute.robotNextX + updateRobotInfo[obstacleRobotId].attribute.robotNextY * mapWidth].isAccessible = 1;//�ѳ�ͻ�����˵���һ��������Ϊ�ϰ�
					}
				}

				if (isReplan > 0) {													//���г�ͻ�����������·���滮
					ReplanPath(rePlanRobotId, obstacleRobotId, isReplan, isExchange);
				}

				if (k != i) {
					if (updateRobotInfo[i].attribute.robotNextX == updateRobotInfo[k].attribute.robotNextX && updateRobotInfo[i].attribute.robotNextY == updateRobotInfo[k].attribute.robotNextY) {
						cout << "\n\n   !!WARNING!!  \n\n" << endl;
						return -1;							//�������󾯸�
					}
				}
				isReplan = 0;
				isInFour = 0;
				isExchange = 0;
			}
		}

		for (int k = 0; k < updateRobotInfo.size(); k++) {
			if (i != k) {
				Node2D tempfourNeighbors1[4], tempfourNeighbors2[4];
				for (int j = 0; j < 4; j++) {
					tempfourNeighbors2[j].x = updateRobotInfo[k].attribute.robotCurrentX + directions[j][0];
					tempfourNeighbors2[j].y = updateRobotInfo[k].attribute.robotCurrentY + directions[j][1];
					if (tempfourNeighbors2[j].x == updateRobotInfo[i].attribute.robotNextX && tempfourNeighbors2[j].y == updateRobotInfo[i].attribute.robotNextY) {
						update_robot_info_backup[k].robot_map[updateRobotInfo[i].attribute.robotNextX + updateRobotInfo[i].attribute.robotNextY * mapWidth].isAccessible = 1;
					}
				}
			}
		}
	}

	for (int i = updateRobotInfo.size() - 1; i >= 0; i--) {
		for (int k = updateRobotInfo.size() - 1; k >= 0; k--) {
			if (k != i) {								//������ͬ�Ļ�����
				///////////////////////////////////////֤��������������һ��Ҫ�ص�
				if (updateRobotInfo[i].attribute.robotNextX == updateRobotInfo[k].attribute.robotNextX && updateRobotInfo[i].attribute.robotNextY == updateRobotInfo[k].attribute.robotNextY) {
					cout << "\n\n   !!WARNING!!  \n\n" << endl;
					return -1;							//�������󾯸�
				}
			}
		}
	}

	return 0;
}


int ReplanPath(int rePlanRobotId, int obstacleRobotId, int isReplan, int isExchange)
{
	int newStartX, newStartY, newEndX, newEndY;
	int directions[4][2] = { {0,-1},{-1,0},{1,0},{0,1} };			//������
	int status = 1;
	vector<PathNode> tempPath;							//������ʱ����·��

	if (updateRobotInfo[rePlanRobotId].robot_path.size() == 1 && updateRobotInfo[rePlanRobotId].highLevelPath.size() == 0) {						//��һ�����յ㣬û�����¸�����Թ滮
		status = 0;
		if (updateRobotInfo[rePlanRobotId].attribute.robotStatus == 0) {		//�˻������˲�һ������ȱʡ���յ㣬����Ҹ��ط�ȥ��
			if (rePlanRobotId < freightRobotNum) {
				for (int q = 0; q < 4; q++) {							//������������û���ϰ��ĵط�����Ϊ��һ����
					int x = updateRobotInfo[rePlanRobotId].attribute.robotCurrentX + directions[q][0];
					int y = updateRobotInfo[rePlanRobotId].attribute.robotCurrentY + directions[q][1];
					if (update_robot_info_backup[rePlanRobotId].robot_map[x + y * mapWidth].isAccessible == 0) {
						newEndX = x;
						newEndY = y;
						break;
					}
					else {												//�������򶼶����ˣ���ͣ��ԭ��
						newEndX = updateRobotInfo[rePlanRobotId].attribute.robotCurrentX;
						newEndY = updateRobotInfo[rePlanRobotId].attribute.robotCurrentY;
					}
				}
			}
			else {
				newEndX = updateRobotInfo[rePlanRobotId].attribute.robotCurrentX;
				newEndY = updateRobotInfo[rePlanRobotId].attribute.robotCurrentY;
			}
		}
		else if (updateRobotInfo[rePlanRobotId].attribute.robotStatus == 1) {
			if (rePlanRobotId < freightRobotNum) {					//�˻������˲�һ����������ʱ�����ϰ�����Ŀ��㻻�ɻ�������һ��
				status = 2;
				int tempEndX = 0, tempEndY = 0;

				if (updateRobotInfo[rePlanRobotId].attribute.taskX == updateRobotInfo[updateRobotInfo[rePlanRobotId].attribute.relatedRobotId - 1].attribute.taskX + 1 &&
					updateRobotInfo[rePlanRobotId].attribute.taskY == updateRobotInfo[updateRobotInfo[rePlanRobotId].attribute.relatedRobotId - 1].attribute.taskY) {
					tempEndX = updateRobotInfo[updateRobotInfo[rePlanRobotId].attribute.relatedRobotId - 1].attribute.taskX - 1;
					tempEndY = updateRobotInfo[updateRobotInfo[rePlanRobotId].attribute.relatedRobotId - 1].attribute.taskY;
				}
				else
				{
					tempEndX = updateRobotInfo[updateRobotInfo[rePlanRobotId].attribute.relatedRobotId - 1].attribute.taskX + 1;
					tempEndY = updateRobotInfo[updateRobotInfo[rePlanRobotId].attribute.relatedRobotId - 1].attribute.taskY;
				}

				if (update_robot_info_backup[rePlanRobotId].robot_map[tempEndX + tempEndY * mapWidth].isAccessible == 0) {
					newEndX = tempEndX;
					newEndY = tempEndY;
				}
				else {	//�ϻ���
					tempEndX = updateRobotInfo[updateRobotInfo[rePlanRobotId].attribute.relatedRobotId - 1].attribute.taskX;
					tempEndY = updateRobotInfo[updateRobotInfo[rePlanRobotId].attribute.relatedRobotId - 1].attribute.taskY;

					if (update_robot_info_backup[rePlanRobotId].robot_map[tempEndX + (tempEndY - 1) * mapWidth].isAccessible == 1) {
						newEndX = updateRobotInfo[updateRobotInfo[rePlanRobotId].attribute.relatedRobotId - 1].attribute.taskX;
						newEndY = updateRobotInfo[updateRobotInfo[rePlanRobotId].attribute.relatedRobotId - 1].attribute.taskY + 1;
					}
					else if (update_robot_info_backup[rePlanRobotId].robot_map[tempEndX + (tempEndY + 1) * mapWidth].isAccessible == 1) {
						newEndX = updateRobotInfo[updateRobotInfo[rePlanRobotId].attribute.relatedRobotId - 1].attribute.taskX;
						newEndY = updateRobotInfo[updateRobotInfo[rePlanRobotId].attribute.relatedRobotId - 1].attribute.taskY - 1;
					}
					else {
						newEndX = updateRobotInfo[rePlanRobotId].attribute.robotCurrentX;
						newEndY = updateRobotInfo[rePlanRobotId].attribute.robotCurrentY;
					}
				}

				updateRobotInfo[rePlanRobotId].attribute.taskX = newEndX;		//�ı�Ŀ�������
				updateRobotInfo[rePlanRobotId].attribute.taskY = newEndY;
			}
			else {													//��������˲�һ���������㣬���ڲ�������������������ͬʱ����ͬһ�������˵ȣ���һ����
				if (updateRobotInfo[obstacleRobotId].robot_path.size() > 1 && obstacleRobotId >= freightRobotNum && updateRobotInfo[obstacleRobotId].attribute.robotStatus == 1){
					int tempRobotId = rePlanRobotId;
					rePlanRobotId = obstacleRobotId;
					obstacleRobotId = tempRobotId;
					newEndX = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 2].x;	//-1�ǳ�ͻ���Ǹ���-2��·���滮��Ŀ���
					newEndY = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 2].y;

					if (updateRobotInfo[obstacleRobotId].attribute.robotNextX != updateRobotInfo[rePlanRobotId].attribute.robotCurrentX ||
						updateRobotInfo[obstacleRobotId].attribute.robotNextY != updateRobotInfo[rePlanRobotId].attribute.robotCurrentY) {
						update_robot_info_backup[rePlanRobotId].robot_map[updateRobotInfo[obstacleRobotId].attribute.robotNextX + updateRobotInfo[obstacleRobotId].attribute.robotNextY * mapWidth].isAccessible = 1;
					}
					update_robot_info_backup[rePlanRobotId].robot_map[updateRobotInfo[obstacleRobotId].attribute.robotCurrentX + mapWidth * updateRobotInfo[obstacleRobotId].attribute.robotCurrentY].isAccessible = 1;

					pushback(obstacleRobotId, 1);
					updateRobotInfo[obstacleRobotId].attribute.robotNextX = updateRobotInfo[obstacleRobotId].attribute.robotCurrentX;
					updateRobotInfo[obstacleRobotId].attribute.robotNextY = updateRobotInfo[obstacleRobotId].attribute.robotCurrentY;
				}
				else if (updateRobotInfo[obstacleRobotId].robot_path.size() == 1 && obstacleRobotId >= freightRobotNum) {
					int flag = 0;
					for (int f = 0; f < 4; f++) {
						int tempX = updateRobotInfo[rePlanRobotId].attribute.robotCurrentX + directions[f][0];
						int tempY = updateRobotInfo[rePlanRobotId].attribute.robotCurrentY + directions[f][1];
						if (update_robot_info_backup[rePlanRobotId].robot_map[tempX + tempY * mapWidth].isAccessible == 0 && 
							tempX != updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].x && 
							tempY != updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].y) 
						{
							updateRobotInfo[rePlanRobotId].robot_path.clear();
							PathNode Node;
							Node.x = tempX;
							Node.y = tempY;
							Node.robotID = updateRobotInfo[rePlanRobotId].attribute.robotId;
							Node.index = Node.x + Node.y * mapWidth;
							pushback(rePlanRobotId, 1);
							updateRobotInfo[rePlanRobotId].robot_path.push_back(Node);
							flag = 1;
							break;
						}
					}
					if (flag == 0)
						pushback(rePlanRobotId, 1);

					updateRobotInfo[rePlanRobotId].attribute.robotNextX = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].x;
					updateRobotInfo[rePlanRobotId].attribute.robotNextY = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].y;

					newEndX = updateRobotInfo[rePlanRobotId].attribute.robotCurrentX;
					newEndY = updateRobotInfo[rePlanRobotId].attribute.robotCurrentY;

					isExchange = 1;
				}
				else {
					newEndX = updateRobotInfo[rePlanRobotId].attribute.robotCurrentX;
					newEndY = updateRobotInfo[rePlanRobotId].attribute.robotCurrentY;
				}
			}
		}
		else if (updateRobotInfo[rePlanRobotId].attribute.robotStatus == 3) {
			UnloadGoods(rePlanRobotId);

			newEndX = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 2].x;	//-1�ǳ�ͻ���Ǹ���-2��·���滮��Ŀ���
			newEndY = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 2].y;
		}
		else if (updateRobotInfo[rePlanRobotId].attribute.robotStatus == -1 &&
			updateRobotInfo[obstacleRobotId].attribute.robotCurrentX == updateRobotInfo[rePlanRobotId].attribute.taskX &&
			updateRobotInfo[obstacleRobotId].attribute.robotCurrentY == updateRobotInfo[rePlanRobotId].attribute.taskY) {

			status = 4;
			updateRobotInfo[rePlanRobotId].attribute.taskX = updateRobotInfo[obstacleRobotId].attribute.taskX;
			updateRobotInfo[rePlanRobotId].attribute.taskY = updateRobotInfo[obstacleRobotId].attribute.taskY;

			newEndX = updateRobotInfo[obstacleRobotId].attribute.taskX;
			newEndY = updateRobotInfo[obstacleRobotId].attribute.taskY;

			updateRobotInfo[obstacleRobotId].robot_path.clear();
			updateRobotInfo[obstacleRobotId].attribute.taskX = updateRobotInfo[obstacleRobotId].attribute.robotCurrentX;
			updateRobotInfo[obstacleRobotId].attribute.taskY = updateRobotInfo[obstacleRobotId].attribute.robotCurrentY;
		}
		else {
			newEndX = updateRobotInfo[rePlanRobotId].attribute.robotCurrentX;
			newEndY = updateRobotInfo[rePlanRobotId].attribute.robotCurrentY;
		}
	}
	else if (updateRobotInfo[rePlanRobotId].attribute.robotStatus == 3 && updateRobotInfo[rePlanRobotId].robot_path.size() == 6 && updateRobotInfo[rePlanRobotId].highLevelPath.size() == 0
		&& update_robot_info_backup[rePlanRobotId].robot_map[updateRobotInfo[rePlanRobotId].attribute.taskX + updateRobotInfo[rePlanRobotId].attribute.taskY * mapWidth].nodeType == 4 &&
		update_robot_info_backup[rePlanRobotId].robot_map[updateRobotInfo[rePlanRobotId].attribute.robotCurrentX + mapWidth * updateRobotInfo[rePlanRobotId].attribute.robotCurrentY].nodeType != 4)
	{
		status = 2;
		int index = chooseNewEndNode(updateRobotInfo[rePlanRobotId].attribute.robotCurrentX, updateRobotInfo[rePlanRobotId].attribute.robotCurrentY, update_robot_info_backup[rePlanRobotId].robot_map);

		updateRobotInfo[rePlanRobotId].attribute.taskX = newEndX = EndNode[index].x;
		updateRobotInfo[rePlanRobotId].attribute.taskY = newEndY = EndNode[index].y;
	}
	else if (updateRobotInfo[rePlanRobotId].robot_path.size() == 1 && updateRobotInfo[rePlanRobotId].highLevelPath.size() > 0) {
		
		ReachNewNode(rePlanRobotId, getMapNode());
		
		if (updateRobotInfo[rePlanRobotId].robot_path.size() >= 2) {
			newEndX = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 2].x;	//-1�ǳ�ͻ���Ǹ���-2��·���滮��Ŀ���
			newEndY = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 2].y;
		}
		else if (updateRobotInfo[rePlanRobotId].robot_path.size() == 1) {
			newEndX = updateRobotInfo[rePlanRobotId].attribute.robotCurrentX;
			newEndY = updateRobotInfo[rePlanRobotId].attribute.robotCurrentY;
		}
	}
	else {
		if (updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 2].x == updateRobotInfo[rePlanRobotId].attribute.robotCurrentX &&
			updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 2].y == updateRobotInfo[rePlanRobotId].attribute.robotCurrentY &&
			(updateRobotInfo[rePlanRobotId].attribute.robotCurrentX != updateRobotInfo[rePlanRobotId].attribute.taskX ||
				updateRobotInfo[rePlanRobotId].attribute.robotCurrentY != updateRobotInfo[rePlanRobotId].attribute.taskY)) 
		{
			updateRobotInfo[rePlanRobotId].robot_path.pop_back();
			updateRobotInfo[rePlanRobotId].robot_path.pop_back();

			if (updateRobotInfo[rePlanRobotId].robot_path.size() > 1) {
				newEndX = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 2].x;	//-1�ǳ�ͻ���Ǹ���-2��·���滮��Ŀ���
				newEndY = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 2].y;
			}
			else {
				newEndX = updateRobotInfo[rePlanRobotId].attribute.robotCurrentX;
				newEndY = updateRobotInfo[rePlanRobotId].attribute.robotCurrentY;
			}
		}
		else {
			newEndX = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 2].x;	//-1�ǳ�ͻ���Ǹ���-2��·���滮��Ŀ���
			newEndY = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 2].y;
		}
	}

	newStartX = updateRobotInfo[rePlanRobotId].attribute.robotCurrentX;		//�����˵�ǰλ��
	newStartY = updateRobotInfo[rePlanRobotId].attribute.robotCurrentY;

	if (newStartX == newEndX && newStartY == newEndY) {
		if (isExchange == 0) {
			pushback(rePlanRobotId, 1);
			updateRobotInfo[rePlanRobotId].attribute.robotNextX = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].x;
			updateRobotInfo[rePlanRobotId].attribute.robotNextY = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].y;
		}
	}
	else {
		tempPath.clear();

		bool isSuccess = Astar(update_robot_info_backup[rePlanRobotId].robot_map, mapWidth, mapHeight, newStartX, newStartY, newEndX, newEndY, tempPath);	//�����ϻ�õ�һ���µĶ�·��

		if (isSuccess == true) {										//·���滮�ɹ�
			int isRun = 1;
			vector<int> tempRobotStatus;
			if (tempPath.size() >= 20 && status != 4) {
				Node2D tempFourNeighbors[4];
				for (int j = 0; j < 4; j++) {							//��¼��ÿ�������˵�ǰ����������������Ļ����˱��
					tempFourNeighbors[j].x = updateRobotInfo[rePlanRobotId].attribute.robotCurrentX + directions[j][0];
					tempFourNeighbors[j].y = updateRobotInfo[rePlanRobotId].attribute.robotCurrentY + directions[j][1];
				}

				for (int kk = updateRobotInfo.size() - 1; kk >= 0; kk--) {
					if (kk != rePlanRobotId) {
						for (int p = 0; p < 4; p++) {
							if (updateRobotInfo[kk].attribute.robotCurrentX == tempFourNeighbors[p].x && updateRobotInfo[kk].attribute.robotCurrentY == tempFourNeighbors[p].y) {
								if (updateRobotInfo[kk].robot_path.size() == 0 || (updateRobotInfo[kk].attribute.robotCurrentX == updateRobotInfo[kk].attribute.robotNextX && 
									updateRobotInfo[kk].attribute.robotCurrentY == updateRobotInfo[kk].attribute.robotNextY)) 
								{
									tempRobotStatus.push_back(0);
								}
								else {
									tempRobotStatus.push_back(1);
								}
							}
						}
					}
				}

				for (int s = 0; s < tempRobotStatus.size(); s++) {
					if (tempRobotStatus[s] == 0) {
						isRun = 0;
					}
					else {
						isRun = 1;
						break;
					}
				}

				if (isRun == 1) {
					pushback(rePlanRobotId, 1);
					updateRobotInfo[rePlanRobotId].attribute.robotNextX = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].x;
					updateRobotInfo[rePlanRobotId].attribute.robotNextY = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].y;

					//						cout << "\n\n**   The new Route is tooooooooo long!!!!! ** \n\n" << endl;
				}
				else {
					status = 3;
					newEndX = updateRobotInfo[rePlanRobotId].attribute.taskX;
					newEndY = updateRobotInfo[rePlanRobotId].attribute.taskY;

					tempPath.clear();
					bool isSuccess = Astar(update_robot_info_backup[rePlanRobotId].robot_map, mapWidth, mapHeight, newStartX, newStartY, newEndX, newEndY, tempPath);	//�����ϻ�õ�һ���µĶ�·��
					if (isSuccess == false) {
						int a = 0;
						pushback(rePlanRobotId, 1);
						tempPath = updateRobotInfo[rePlanRobotId].robot_path;
					}
					else {
						updateRobotInfo[rePlanRobotId].attribute.robotNextX = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].x;
						updateRobotInfo[rePlanRobotId].attribute.robotNextY = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].y;
					}
				}
			}
			else if (tempPath.size() < 20 || status == 4) {								//�¹滮��·�����Ȳ�����20�������ͣ��ԭ����
				isRun = 0;
			}

			if (isRun == 0) {
				if (status == 2) {
					int length = updateRobotInfo[rePlanRobotId].robot_path.size();
					for (int h = 0; h < length; h++) {
						updateRobotInfo[rePlanRobotId].robot_path.pop_back();
					}
					for (int h = 0; h < length - 1; h++) {
						PathNode tempEnd;
						tempEnd.x = newEndX;
						tempEnd.y = newEndY;
						tempEnd.index = newEndX + newEndY * mapWidth;
						tempEnd.robotID = rePlanRobotId + 1;
						updateRobotInfo[rePlanRobotId].robot_path.push_back(tempEnd);
					}
				}
				else if (status == 3) {
					int endNum = 0;
					if (updateRobotInfo[rePlanRobotId].robot_path.size() > 0) {
						for (int w = 0; w < updateRobotInfo[rePlanRobotId].robot_path.size() - 1; w++) {
							if (updateRobotInfo[rePlanRobotId].robot_path[w].x == updateRobotInfo[rePlanRobotId].robot_path[w + 1].x && 
								updateRobotInfo[rePlanRobotId].robot_path[w].y == updateRobotInfo[rePlanRobotId].robot_path[w + 1].y) 
							{
								endNum += 1;
							}
							else {
								break;
							}
						}
					}
					updateRobotInfo[rePlanRobotId].robot_path.clear();

					PathNode tempEnd;
					tempEnd.x = newEndX;
					tempEnd.y = newEndY;
					tempEnd.index = newEndX + newEndY * mapWidth;
					tempEnd.robotID = rePlanRobotId + 1;
					for (int w = 0; w < endNum; w++) {
						updateRobotInfo[rePlanRobotId].robot_path.push_back(tempEnd);
					}
				}
				else if (status == 4) {
					updateRobotInfo[rePlanRobotId].robot_path.clear();
				}
				else {
					updateRobotInfo[rePlanRobotId].robot_path.pop_back();					//���¸���(����ͻ��)Ҳɾ��
					if (status != 0) {
						updateRobotInfo[rePlanRobotId].robot_path.pop_back();
					}
				}

				if (tempPath.size() > 0) {
					for (int u = 0; u < tempPath.size(); u++) {				//���µ���㣨��ǰ�㣩����һ���㵽�յ�(ԭ��������)��·���ӵ�ԭ����·����ȥ
						updateRobotInfo[rePlanRobotId].robot_path.push_back(tempPath[u]);
					}

					if (status == 4) {
						updateRobotInfo[rePlanRobotId].attribute.oncePathSize = updateRobotInfo[rePlanRobotId].robot_path.size();
						updateRobotInfo[rePlanRobotId].attribute.onceWorkPreiod = 0;
					}
				}

				updateRobotInfo[rePlanRobotId].attribute.robotNextX = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].x;	//�ѻ�������һ�����������¹滮����һ����
				updateRobotInfo[rePlanRobotId].attribute.robotNextY = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].y;
			}
		}
		else {						//·���滮���ɹ�,ͣ��ԭ�ز������ȴ����Ƹı�
			if (isExchange == 0) {
				pushback(rePlanRobotId, 1);
				updateRobotInfo[rePlanRobotId].attribute.robotNextX = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].x;
				updateRobotInfo[rePlanRobotId].attribute.robotNextY = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].y;
			}
			//				cout << "\n\n**   The new Route is impossible!!!! ** \n\n" << endl;
		}

	}
	if (isReplan == 1) {	//����������������г�ͻ��������ı�������һ�������˵�·������һ���������򱣳ֲ����������˫����һ��Ŀ���ĳ�ͻ���ı�һ�������˵�·��������һ����ԭ·����
		pushback(obstacleRobotId, 1);
		updateRobotInfo[obstacleRobotId].attribute.robotNextX = updateRobotInfo[obstacleRobotId].robot_path[updateRobotInfo[obstacleRobotId].robot_path.size() - 1].x;
		updateRobotInfo[obstacleRobotId].attribute.robotNextY = updateRobotInfo[obstacleRobotId].robot_path[updateRobotInfo[obstacleRobotId].robot_path.size() - 1].y;
	}

	if (updateRobotInfo[rePlanRobotId].robot_path.size() > 0) {
		updateRobotInfo[rePlanRobotId].attribute.robotNextX = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].x;
		updateRobotInfo[rePlanRobotId].attribute.robotNextY = updateRobotInfo[rePlanRobotId].robot_path[updateRobotInfo[rePlanRobotId].robot_path.size() - 1].y;
	}
	if (updateRobotInfo[obstacleRobotId].robot_path.size() > 0) {
		updateRobotInfo[obstacleRobotId].attribute.robotNextX = updateRobotInfo[obstacleRobotId].robot_path[updateRobotInfo[obstacleRobotId].robot_path.size() - 1].x;
		updateRobotInfo[obstacleRobotId].attribute.robotNextY = updateRobotInfo[obstacleRobotId].robot_path[updateRobotInfo[obstacleRobotId].robot_path.size() - 1].y;
	}

	for (int ik = 0; ik < updateRobotInfo.size(); ik++) {
		if (rePlanRobotId != ik) {
			Node2D tempfourNeighbors1[4], tempfourNeighbors2[4];
			for (int j = 0; j < 4; j++) {
				tempfourNeighbors2[j].x = updateRobotInfo[ik].attribute.robotCurrentX + directions[j][0];
				tempfourNeighbors2[j].y = updateRobotInfo[ik].attribute.robotCurrentY + directions[j][1];
				if (tempfourNeighbors2[j].x == updateRobotInfo[rePlanRobotId].attribute.robotNextX && tempfourNeighbors2[j].y == updateRobotInfo[rePlanRobotId].attribute.robotNextY) {
					update_robot_info_backup[ik].robot_map[updateRobotInfo[rePlanRobotId].attribute.robotNextX + updateRobotInfo[rePlanRobotId].attribute.robotNextY * mapWidth].isAccessible = 1;
				}
			}
		}
	}

	return 0;
}


int RobotAdjustment(int i)
{
	int directions[4][2] = { {0,-1},{-1,0},{1,0},{0,1} };			//������
	Node2D fourNeighbors[4];

	for (int j = 0; j < 4; j++) {					//��¼��ÿ�������˵�ǰ����������������Ļ����˱��
		fourNeighbors[j].x = updateRobotInfo[i].attribute.robotCurrentX + directions[j][0];
		fourNeighbors[j].y = updateRobotInfo[i].attribute.robotCurrentY + directions[j][1];
	}

	for (int k = 0; k < updateRobotInfo.size(); k++) {
		if (k != i) {
			for (int p = 0; p < 4; p++) {
				if (updateRobotInfo[k].attribute.robotCurrentX == fourNeighbors[p].x && updateRobotInfo[k].attribute.robotCurrentY == fourNeighbors[p].y) {
					update_robot_info_backup[i].robot_map[updateRobotInfo[k].attribute.robotCurrentX + mapWidth * updateRobotInfo[k].attribute.robotCurrentY].isAccessible = 1;
				}
			}
		}
	}

	//1.���������ҵ���----��������˻���������************************************************************************************************************************************
	if (i >= freightRobotNum && updateRobotInfo[i].attribute.area > 0) {				//���������
		if (updateRobotInfo[i].attribute.waitTime > taskProcess[updateRobotInfo[i].attribute.area - 1].freiDistance) {
			///////////////////////////////////////////////////////////////////////////////////////////�������ʱ���˻������˱���������ص��Լ���ȱʡλ��
			int peerRobotId = 0;
			peerRobotId = updateRobotInfo[i].attribute.relatedRobotId - 1;
			updateRobotInfo[peerRobotId].attribute.goodsId = 0;
			updateRobotInfo[peerRobotId].attribute.goodsType = 0;
			updateRobotInfo[peerRobotId].attribute.relatedRobotId = 0;
			updateRobotInfo[peerRobotId].attribute.robotStatus = 0;
			updateRobotInfo[peerRobotId].attribute.taskId = 0;
			updateRobotInfo[peerRobotId].attribute.waitTime = 0;
			updateRobotInfo[peerRobotId].attribute.taskX = updateRobotInfo[peerRobotId].attribute.robotDefaultX;
			updateRobotInfo[peerRobotId].attribute.taskY = updateRobotInfo[peerRobotId].attribute.robotDefaultY;
			if (updateRobotInfo[peerRobotId].attribute.robotType == 2) {
				updateRobotInfo[peerRobotId].attribute.area = 0;
			}

			vector<PathNode> tempPath = updateRobotInfo[peerRobotId].robot_path;
			updateRobotInfo[peerRobotId].robot_path.clear();

			bool isSuccess = useAStar(peerRobotId, update_robot_info_backup[peerRobotId].robot_map);
			if (isSuccess == false) {
				if (updateRobotInfo[peerRobotId].attribute.robotCurrentX == updateRobotInfo[peerRobotId].attribute.taskX && updateRobotInfo[peerRobotId].attribute.robotCurrentY == updateRobotInfo[peerRobotId].attribute.taskY) {
					pushback(peerRobotId, 1);
					updateRobotInfo[peerRobotId].highLevelPath.clear();
				}
				else {
					int a = 0;
					updateRobotInfo[peerRobotId].robot_path = tempPath;
				}
			}
			else {
				updateRobotInfo[peerRobotId].highLevelPath.clear();
			}
			updateRobotInfo[peerRobotId].attribute.oncePathSize = updateRobotInfo[peerRobotId].robot_path.size();
			updateRobotInfo[peerRobotId].attribute.onceWorkPreiod = 0;
			updateRobotInfo[peerRobotId].attribute.robotNextX = updateRobotInfo[peerRobotId].robot_path[updateRobotInfo[peerRobotId].robot_path.size() - 1].x;
			updateRobotInfo[peerRobotId].attribute.robotNextY = updateRobotInfo[peerRobotId].robot_path[updateRobotInfo[peerRobotId].robot_path.size() - 1].y;

			///////////////////////////////////////////////////////////////////////////////�õȴ��˺þõļ�������˱��û�к�����飬����Ѱ�Һ������
			updateRobotInfo[i].attribute.relatedRobotId = 0;
			updateRobotInfo[i].attribute.waitTime = 0;

			task task1;
			task1.goodsId = updateRobotInfo[i].attribute.goodsId;
			task1.goodsX = GoodsContents[updateRobotInfo[i].attribute.goodsId - 1].goods_X;
			task1.goodsY = GoodsContents[updateRobotInfo[i].attribute.goodsId - 1].goods_Y;
			task1.status = 1;
			task1.pickingRobotId = i + 1;
			task1.goodsType = GoodsContents[updateRobotInfo[i].attribute.goodsId - 1].goods_Type;
			task1.taskId = updateRobotInfo[i].attribute.taskId;
			taskProcess[updateRobotInfo[i].attribute.area - 1].pickingTask.push_back(task1);
		}
	}//2.���������ҵ���----�˻������˻�����λ��*******************************************************************************************************************
	else if (i < freightRobotNum && updateRobotInfo[i].attribute.area > 0) {
		if (updateRobotInfo[i].attribute.relatedRobotId > 0) {
			if (updateRobotInfo[updateRobotInfo[i].attribute.relatedRobotId - 1].attribute.robotStatus == 1) {
				if (updateRobotInfo[i].attribute.waitTime > taskProcess[updateRobotInfo[i].attribute.area - 1].pickDistance) {
					if (updateRobotInfo[i].attribute.taskX == updateRobotInfo[updateRobotInfo[i].attribute.relatedRobotId - 1].attribute.taskX + 1) {
						updateRobotInfo[i].attribute.taskX = updateRobotInfo[updateRobotInfo[i].attribute.relatedRobotId - 1].attribute.taskX - 1;
					}
					else if (updateRobotInfo[i].attribute.taskX == updateRobotInfo[updateRobotInfo[i].attribute.relatedRobotId - 1].attribute.taskX - 1) {
						updateRobotInfo[i].attribute.taskX = updateRobotInfo[updateRobotInfo[i].attribute.relatedRobotId - 1].attribute.taskX + 1;
					}
					vector<PathNode> tempPath = updateRobotInfo[i].robot_path;
					updateRobotInfo[i].robot_path.clear();

					bool isSuccess = useAStar(i, update_robot_info_backup[i].robot_map);
					if (isSuccess == false) {
						updateRobotInfo[i].robot_path = tempPath;
					}
					updateRobotInfo[i].attribute.robotNextX = updateRobotInfo[i].robot_path[updateRobotInfo[i].robot_path.size() - 1].x;
					updateRobotInfo[i].attribute.robotNextY = updateRobotInfo[i].robot_path[updateRobotInfo[i].robot_path.size() - 1].y;
					updateRobotInfo[i].attribute.waitTime = 0;
				}
			}
		}
	}
	//*************************************************************************************************************************************************************


	//3.���������ҵ���----�˻������˻س�ʱ�ѵ��ﴫ�ʹ�ǰ��,�ҵ�ǰ����������Ŀ��㣬�ѵ�ǰ�����ĳ�ж����********************************************************************
	if (i < freightRobotNum && updateRobotInfo[i].attribute.robotStatus == 3 && updateRobotInfo[i].attribute.robotCurrentX == updateRobotInfo[i].attribute.taskX &&	//�˻������˵��ﴫ�ʹ�ǰ��
		updateRobotInfo[i].attribute.robotCurrentY != updateRobotInfo[i].attribute.taskY &&
		update_robot_info_backup[i].robot_map[updateRobotInfo[i].attribute.robotCurrentX + mapWidth * updateRobotInfo[i].attribute.robotCurrentY].nodeType == 4)
	{
		updateRobotInfo[i].robot_path.clear();
		pushback(i, 5);
		updateRobotInfo[i].attribute.oncePathSize = updateRobotInfo[i].robot_path.size();
		updateRobotInfo[i].attribute.onceWorkPreiod = 0;
		updateRobotInfo[i].attribute.robotNextY = updateRobotInfo[i].attribute.robotCurrentY;
		updateRobotInfo[i].attribute.taskY = updateRobotInfo[i].attribute.robotCurrentY;
	}
	//****************************************************************************************************************************************************************************

	//4.���������ҵ���----��鵱ǰ�����˵�ʵ��·���Ƿ�ȹ滮·�������ǵĻ����¹滮*********************************************************************************************
	int dreamSize = updateRobotInfo[i].attribute.oncePathSize * 140 / 100;
	if (updateRobotInfo[i].robot_path.size() > dreamSize && (updateRobotInfo[i].robot_path.size() - dreamSize) > 4) {
		vector<PathNode> tempPath;
		tempPath = updateRobotInfo[i].robot_path;
		int flag = 0;
		if (i < freightRobotNum && updateRobotInfo[i].attribute.robotStatus == 3 && updateRobotInfo[i].robot_path.size() >= 6 && updateRobotInfo[i].highLevelPath.size() == 0 &&
			update_robot_info_backup[i].robot_map[updateRobotInfo[i].attribute.taskX + mapWidth * updateRobotInfo[i].attribute.taskY].nodeType == 4) {
			updateRobotInfo[i].robot_path.clear();
			PathNode node;
			node.x = updateRobotInfo[i].attribute.taskX;
			node.y = updateRobotInfo[i].attribute.taskY;
			node.robotID = i + 1;
			node.index = node.x + node.y * mapWidth;
			for (int q = 0; q < 5; q++) {
				updateRobotInfo[i].robot_path.push_back(node);
			}
		}
		else if (i < freightRobotNum && updateRobotInfo[i].attribute.robotStatus == 3 && updateRobotInfo[i].robot_path.size() <= 6 && updateRobotInfo[i].highLevelPath.size() == 0 &&
			update_robot_info_backup[i].robot_map[updateRobotInfo[i].attribute.robotCurrentX + mapWidth * updateRobotInfo[i].attribute.robotCurrentY].nodeType == 4) {
			flag = 1;
		}
		else {
			updateRobotInfo[i].robot_path.clear();
		}

		if (flag == 0) {
			bool isSuccess = useAStar(i, update_robot_info_backup[i].robot_map);
			if (isSuccess == false) {
				if (updateRobotInfo[i].attribute.robotCurrentX == updateRobotInfo[i].attribute.taskX && updateRobotInfo[i].attribute.robotCurrentY == updateRobotInfo[i].attribute.taskY) {
					pushback(i, 1);
					//updateRobotInfo[i].highLevelPath.clear();
				}
				else {
					updateRobotInfo[i].robot_path = tempPath;
				}
			}
			else {
				if (updateRobotInfo[i].robot_path.size() > tempPath.size()) {
					updateRobotInfo[i].robot_path = tempPath;
				}
				else {
					//updateRobotInfo[i].highLevelPath.clear();
					updateRobotInfo[i].attribute.oncePathSize = updateRobotInfo[i].robot_path.size();
					updateRobotInfo[i].attribute.onceWorkPreiod = 0;
				}
			}
		}
		updateRobotInfo[i].attribute.robotNextX = updateRobotInfo[i].robot_path[updateRobotInfo[i].robot_path.size() - 1].x;
		updateRobotInfo[i].attribute.robotNextY = updateRobotInfo[i].robot_path[updateRobotInfo[i].robot_path.size() - 1].y;
	}
	//******************************************************************************************************************************************************************//

	//5.���������ҵ���----һ��·��̫��ʱ���ˣ������Ǳ���ס��,���¹滮
	if (updateRobotInfo[i].attribute.onceWorkPreiod > updateRobotInfo[i].attribute.oncePathSize * 2 && updateRobotInfo[i].robot_path.size() > 0) {
		vector<PathNode> tempPath = updateRobotInfo[i].robot_path;
		int flag = 0;
		if (i < freightRobotNum && updateRobotInfo[i].attribute.robotStatus == 3 && updateRobotInfo[i].robot_path.size() >= 6 && updateRobotInfo[i].highLevelPath.size() == 0 &&
			update_robot_info_backup[i].robot_map[updateRobotInfo[i].attribute.taskX + mapWidth * updateRobotInfo[i].attribute.taskY].nodeType == 4) {

			updateRobotInfo[i].robot_path.clear();
			PathNode endPoint;
			endPoint.x = updateRobotInfo[i].attribute.taskX;
			endPoint.y = updateRobotInfo[i].attribute.taskY;
			endPoint.robotID = i + 1;
			endPoint.index = endPoint.x + endPoint.y * mapWidth;
			for (int iii = 0; iii < 5; iii++)
				updateRobotInfo[i].robot_path.push_back(endPoint);
		}
		else if (i < freightRobotNum && updateRobotInfo[i].attribute.robotStatus == 3 && updateRobotInfo[i].robot_path.size() <= 6 && updateRobotInfo[i].highLevelPath.size() == 0 &&
			update_robot_info_backup[i].robot_map[updateRobotInfo[i].attribute.robotCurrentX + mapWidth * updateRobotInfo[i].attribute.robotCurrentY].nodeType == 4) {
			flag = 1;
		}
		else {
			updateRobotInfo[i].robot_path.clear();
		}

		if (flag == 0) {
			bool isSuccess = useAStar(i, update_robot_info_backup[i].robot_map);
			if (isSuccess == false) {
				if (updateRobotInfo[i].attribute.robotCurrentX == updateRobotInfo[i].attribute.taskX && updateRobotInfo[i].attribute.robotCurrentY == updateRobotInfo[i].attribute.taskY) {
					pushback(i, 1);
					//updateRobotInfo[i].highLevelPath.clear();
				}
				else {
					updateRobotInfo[i].robot_path = tempPath;
				}
			}
			else {
				//updateRobotInfo[i].highLevelPath.clear();
				updateRobotInfo[i].attribute.oncePathSize = updateRobotInfo[i].robot_path.size();
				updateRobotInfo[i].attribute.onceWorkPreiod = 0;
			}
		}

		updateRobotInfo[i].attribute.robotNextX = updateRobotInfo[i].robot_path[updateRobotInfo[i].robot_path.size() - 1].x;
		updateRobotInfo[i].attribute.robotNextY = updateRobotInfo[i].robot_path[updateRobotInfo[i].robot_path.size() - 1].y;
	}
	//**************************************************************************************************************************************************************************

	return 0;
}



//����������˺��˻��������Ƿ�ɽ���
int checkIsHandover(int i, int k, vector<MapNode> map)
{
	if (i >= freightRobotNum) {							//��������i�Ǽ��������
		if (updateRobotInfo[k].attribute.robotStatus == 1 && updateRobotInfo[k].attribute.robotCurrentX == updateRobotInfo[k].attribute.taskX && updateRobotInfo[k].attribute.robotCurrentY == updateRobotInfo[k].attribute.taskY) {
			if (updateRobotInfo[k].robot_path.size() > 0) {
				if (updateRobotInfo[updateRobotInfo[k].attribute.relatedRobotId - 1].attribute.robotStatus == 2) {
					updateRobotInfo[k].robot_path.clear();
					updateRobotInfo[k].highLevelPath.clear();
					pushback(k, 1);
					updateRobotInfo[k].attribute.robotNextX = updateRobotInfo[k].attribute.taskX = updateRobotInfo[k].attribute.robotCurrentX;
					updateRobotInfo[k].attribute.robotNextY = updateRobotInfo[k].attribute.taskY = updateRobotInfo[k].attribute.robotCurrentY;
				}
			}
		}
		else if ((updateRobotInfo[i].attribute.robotCurrentX == updateRobotInfo[i].attribute.taskX && updateRobotInfo[i].attribute.robotCurrentY == updateRobotInfo[i].attribute.taskY) && updateRobotInfo[k].attribute.robotStatus == 1
			&& ((updateRobotInfo[k].attribute.robotCurrentX == updateRobotInfo[i].attribute.taskX + 1 && updateRobotInfo[k].attribute.robotCurrentY == updateRobotInfo[i].attribute.taskY) ||
				(updateRobotInfo[k].attribute.robotCurrentX == updateRobotInfo[i].attribute.taskX - 1 && updateRobotInfo[k].attribute.robotCurrentY == updateRobotInfo[i].attribute.taskY) ||
				(updateRobotInfo[k].attribute.robotCurrentX == updateRobotInfo[i].attribute.taskX && (updateRobotInfo[k].attribute.robotCurrentY == updateRobotInfo[i].attribute.taskY + 1 || updateRobotInfo[k].attribute.taskY == updateRobotInfo[i].attribute.taskY - 1)))
			&& (updateRobotInfo[k].attribute.robotCurrentX != updateRobotInfo[k].attribute.taskX || updateRobotInfo[k].attribute.robotCurrentY != updateRobotInfo[k].attribute.taskY))
		{
			updateRobotInfo[k].robot_path.clear();
			pushback(k, 1);
			updateRobotInfo[k].highLevelPath.clear();
			updateRobotInfo[k].attribute.robotNextX = updateRobotInfo[k].attribute.taskX = updateRobotInfo[k].attribute.robotCurrentX;
			updateRobotInfo[k].attribute.robotNextY = updateRobotInfo[k].attribute.taskY = updateRobotInfo[k].attribute.robotCurrentY;
		}
		else if (updateRobotInfo[i].attribute.robotCurrentX == updateRobotInfo[k].attribute.taskX && updateRobotInfo[i].attribute.robotCurrentY == updateRobotInfo[k].attribute.taskY
			&& updateRobotInfo[k].attribute.robotCurrentX == updateRobotInfo[i].attribute.taskX && updateRobotInfo[k].attribute.robotCurrentY == updateRobotInfo[i].attribute.taskY) {
			int tempTaskX;
			if (updateRobotInfo[k].attribute.robotCurrentX < updateRobotInfo[i].attribute.robotCurrentX) {
				tempTaskX = updateRobotInfo[i].attribute.taskX - 1;
				updateRobotInfo[k].attribute.taskX = updateRobotInfo[i].attribute.taskX - 1;
			}
			else {
				tempTaskX = updateRobotInfo[i].attribute.taskX + 1;
				updateRobotInfo[k].attribute.taskX = updateRobotInfo[i].attribute.taskX + 1;
			}
			vector<PathNode> tempPath = updateRobotInfo[k].robot_path;
			updateRobotInfo[k].robot_path.clear();
			bool isSuccess = Astar(map, mapWidth, mapHeight, updateRobotInfo[k].attribute.robotCurrentX, updateRobotInfo[k].attribute.robotCurrentY, tempTaskX, updateRobotInfo[k].attribute.taskY, updateRobotInfo[k].robot_path);

			if (isSuccess == false) {
				if (updateRobotInfo[k].attribute.robotCurrentX == tempTaskX && updateRobotInfo[k].attribute.robotCurrentY == updateRobotInfo[k].attribute.taskY) {
					pushback(k, 1);
					updateRobotInfo[k].attribute.taskX = tempTaskX;
					updateRobotInfo[k].highLevelPath.clear();
				}
				else {
					int a = 0;
					updateRobotInfo[k].robot_path = tempPath;
				}
			}
			else {
				updateRobotInfo[k].attribute.taskX = tempTaskX;
				updateRobotInfo[k].highLevelPath.clear();
			}
			updateRobotInfo[k].attribute.robotNextX = updateRobotInfo[k].robot_path[updateRobotInfo[k].robot_path.size() - 1].x;
			updateRobotInfo[k].attribute.robotNextY = updateRobotInfo[k].robot_path[updateRobotInfo[k].robot_path.size() - 1].y;
		}
		else {
			int t = 0;
		}
	}

	return 0;
}



int pushback(int robotId, int times)
{
	PathNode Node;
	Node.x = updateRobotInfo[robotId].attribute.robotCurrentX;
	Node.y = updateRobotInfo[robotId].attribute.robotCurrentY;
	Node.robotID = updateRobotInfo[robotId].attribute.robotId;
	Node.index = Node.x + Node.y * mapWidth;

	for (int i = 0; i < times; i++)
		updateRobotInfo[robotId].robot_path.push_back(Node);

	return 0;
}