#include "public.h"
#include "update_file.h"
#include "task_allocation.h"
#include "calculate.h"
#include "conflict_resolution.h"
#include "status_transform.h"
#include "goods_area.h"


vector<taskRecord> taskPeriodTime;
vector<GoodsAreaInfo> taskProcess;
vector<RobotInfo> updateRobotInfo;						//��¼ÿ�������˵ĵ�ǰ��Ϣ(�����ݿ����ݱ���һ��)
vector<vector <GoodsInfo>> GoodsInformation;
vector<GoodsInfo> GoodsContents;
vector<MapNode> EndNode;								//��¼�����յ��������Ϣ
vector<MapNode> EndPointRight1;							//�Ҵ��ʹ�1
vector<MapNode> EndPointRight2;							//�Ҵ��ʹ�2


int mapWidth = 0;
int mapHeight = 0;
int shelfXnum = 0;
int shelfYnum = 0;
int areaNum = 0;
int goodsTypeNum = 0;
int freightRobotNum = 0;
int pickingRobotNum = 0;
int robotNum = 0;
int globalPeriod = 0;
int finishAreaNum = 0;
int start_x = 0;
int conveyorNum = 0;

int WorkingProcess(void);
int Clear(void);
int checkTaskList(vector<MapNode> Map);



int main(void)
{
	ifstream fic;
	fic.open("outputfiles.txt");
	char str_1[502][77], str_2[502][77], str_3[502][77], str_4[502][77];
	for (int i = 1; i <= 500; i++) {
		fic >> str_1[i] >> str_2[i] >> str_3[i] >> str_4[i];
	}

	readParameters();

	CreateMap(mapWidth, mapHeight, shelfXnum, shelfYnum, conveyorNum);

	for (int i = 301; i <= 500; i++) {
		ReadGoodsAndTasks(i, str_1[i], str_2[i]);

		int tmp = WorkingProcess();

		if (tmp == -2)
			break;

		OutputResult(tmp, i, str_3[i]);
	}

	cout << "success" << endl;

	return 0;
}


int WorkingProcess(void)
{
	Clear();																	//�������ȫ������

	int isCheck = 0;
	int isWrong = 0;
	int isFinished = 0;

	GoodsAreaInfo newtask;
	for (int i = 0; i < areaNum; i++)
		taskProcess.push_back(newtask);

	GoodsInformation.clear();
	for (int i = 0; i < goodsTypeNum; i++)
		GoodsInformation.push_back(vector<GoodsInfo>());						//��ʼ������洢�б�

	vector<MapNode> Map = getMapNode();											//��ȡ��̬��ͼ��Ϣ

	getGoodsNode2();															//��û�����Ϣ

	getEndNode();																//�õ��յ��λ��

	AllocateTask2Area();														//һ������������
															
	RobotAllocationMethods(1);													//�������˷��䵽��������

	CreatInitRobots();															//���������˳�ʼλ��

	WalkToOwnArea(CreateMap(Map));												//�û�������ȥ��������

	while (true) {
		AssignPickingRobot(Map);												//�������������������

		AssignFreightRobot(Map);												//�����������˻�������

		isWrong = CheckNeighborhood();											//������һ��ǰ����ͻ��������������У�
		if (isWrong == -1)
			break;

		RobotMove(Map, globalPeriod);											//��̬ˢ�»�����λ�ã�״̬3��4��5�ĸı䣩

		globalPeriod++;															//��ɢʱ�䲽����1

		if (globalPeriod == 35)
			int a = 0;

		if (globalPeriod > 15000) 												//����ʱ��������ж�����
			return -1;

		RobotAllocationMethods(2);												//��;�����˻�������

		RobotAllocationMethods(3);												//����������ɺ���Ԯ��������

		isFinished = checkTaskList(Map);										//������������Ƿ����

		if (isFinished == 1) {													//�����ɾ���������ֹ����
			for (int i = 0; i < areaNum; i++)
				cout << "1. " << taskProcess[i].finishPeriod << endl;
			cout << "the whole period is : " << globalPeriod << endl << endl;

			isFinished = 0;
			break;
		}
		else if (isFinished == -1) {
			cout << "The running time is too long!" << endl;
			break;
		}
	}

	if (isWrong == 0) {
		cout << "\n\n  ***  All tasks have been finished!  ***\n\n" << endl;
	}
	else if (isWrong == -1) {
		cout << "\n\n !!! ERROR !!! \n\n" << endl;
		return -2;
	}

	return 0;
}


//ÿ�����ȫ�ֱ���
int Clear(void)
{
	updateRobotInfo.clear();

	for (int i = 0; i < updateRobotInfo.size(); i++)
		updateRobotInfo[i].robot_map.clear();
	updateRobotInfo.clear();

	taskProcess.clear();

	EndNode.clear();

	taskPeriodTime.clear();

	for (int i = 0; i < GoodsInformation.size(); i++)
		GoodsInformation[i].clear();
	GoodsInformation.clear();

	GoodsContents.clear();

	globalPeriod = 0;

	finishAreaNum = 0;

	return 0;
}


//ÿ��time step����Ƿ����������
int checkTaskList(vector<MapNode> Map)
{
	int lastTaskNum = -1;
	for (int i = 0; i < areaNum; i++) {
		if (taskProcess[i].taskNum > 0) {
			if (taskProcess[i].isAllFinished > 0) {
				lastTaskNum = 0;
			}
			else if (taskProcess[i].isAllFinished == 0) {
				lastTaskNum = -1;
				break;
			}
		}
	}

	int isFinished = 0;

	if (lastTaskNum == 0) {							//��ʾ���������Ѿ����
		getRobotDefault();
		AllRobotBack(Map);							//������ȱʡλ�õĻ����˹滮��ȥ��·��

		while (true) {
			globalPeriod++;
			if (globalPeriod > 15000)
				return -1;

			int isWrong = CheckNeighborhood();					//������һ��ǰ����ͻ��������������У�
			if (isWrong == -1)
				break;

			RobotMove(Map, 1);

			for (int i = 0; i < updateRobotInfo.size(); i++) {
				if (updateRobotInfo[i].attribute.robotCurrentX == updateRobotInfo[i].attribute.robotDefaultX && updateRobotInfo[i].attribute.robotCurrentY == updateRobotInfo[i].attribute.robotDefaultY) {
					updateRobotInfo[i].robot_path.clear();
					updateRobotInfo[i].attribute.robotStatus = 0;
					updateRobotInfo[i].attribute.robotNextX = updateRobotInfo[i].attribute.robotCurrentX;
					updateRobotInfo[i].attribute.robotNextY = updateRobotInfo[i].attribute.robotCurrentY;
				}
			}

			for (int i = 0; i < updateRobotInfo.size(); i++) {
				if (updateRobotInfo[i].robot_path.size() > 0) {
					isFinished = 1;
					break;
				}
				else {
					isFinished = 0;
				}
			}
			if (isFinished == 0) {
				break;
			}
		}
		return 1;
	}
	else {
		return 0;
	}
}