#include "public.h"
#include "update_file.h"
#include "task_allocation.h"
#include "calculate.h"
#include "conflict_resolution.h"
#include "status_transform.h"
#include "goods_area.h"


vector<taskRecord> taskPeriodTime;
vector<GoodsAreaInfo> taskProcess;
vector<RobotInfo> updateRobotInfo;						//记录每个机器人的当前信息(与数据库内容保持一致)
vector<vector <GoodsInfo>> GoodsInformation;
vector<GoodsInfo> GoodsContents;
vector<MapNode> EndNode;								//记录所有终点的坐标信息
vector<MapNode> EndPointRight1;							//右传送带1
vector<MapNode> EndPointRight2;							//右传送带2


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
	Clear();																	//清空所有全局数据

	int isCheck = 0;
	int isWrong = 0;
	int isFinished = 0;

	GoodsAreaInfo newtask;
	for (int i = 0; i < areaNum; i++)
		taskProcess.push_back(newtask);

	GoodsInformation.clear();
	for (int i = 0; i < goodsTypeNum; i++)
		GoodsInformation.push_back(vector<GoodsInfo>());						//初始化货物存储列表

	vector<MapNode> Map = getMapNode();											//读取静态地图信息

	getGoodsNode2();															//获得货物信息

	getEndNode();																//得到终点的位置

	AllocateTask2Area();														//一次性生成任务
															
	RobotAllocationMethods(1);													//将机器人分配到各个区域

	CreatInitRobots();															//创建机器人初始位置

	WalkToOwnArea(CreateMap(Map));												//让机器人走去工作区域

	while (true) {
		AssignPickingRobot(Map);												//将任务分配给拣货机器人

		AssignFreightRobot(Map);												//将任务分配给运货机器人

		isWrong = CheckNeighborhood();											//在走下一步前检查冲突并解决（持续进行）
		if (isWrong == -1)
			break;

		RobotMove(Map, globalPeriod);											//动态刷新机器人位置（状态3和4和5的改变）

		globalPeriod++;															//离散时间步长加1

		if (globalPeriod == 35)
			int a = 0;

		if (globalPeriod > 15000) 												//工作时间过长，中断跳出
			return -1;

		RobotAllocationMethods(2);												//中途调度运货机器人

		RobotAllocationMethods(3);												//所有区域完成后增援其他区域

		isFinished = checkTaskList(Map);										//检查所有任务是否完成

		if (isFinished == 1) {													//如果完成就跳出，终止程序
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


//每轮清空全局变量
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


//每个time step检查是否已完成任务
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

	if (lastTaskNum == 0) {							//表示所有任务已经完成
		getRobotDefault();
		AllRobotBack(Map);							//给不在缺省位置的机器人规划回去的路径

		while (true) {
			globalPeriod++;
			if (globalPeriod > 15000)
				return -1;

			int isWrong = CheckNeighborhood();					//在走下一步前检查冲突并解决（持续进行）
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