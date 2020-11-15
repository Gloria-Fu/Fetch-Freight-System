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


struct MapNode {				//��̬��ͼ������Ϣ
	int x;						//��ͼ��ĺ�����
	int y;						//��ͼ���������
	int nodeType;				//��ͼ�������
	int nodeId;					//��ͼ���ID
	int isAccessible;			//��ͼ���Ƿ��ͨ�У�0�ɣ�1����
	int nodeWeight;				//�õ����ڼ���A*ʱ��Ȩ��ֵ
	int distance;
};


struct NodeInfo {
	int x;						//��ĺ�����
	int y;						//���������
	float g;					//��ǰ����븸�ڵ�ľ���
	float h;					//��ʼ�㵽�յ�������پ���
	float f;					//f = g + h
	int index;					//��ǰ���һά����λ��
	int preIndex;				//�丸�ڵ��һά����λ��
};

struct PathNode {
	int x;						//·���ڵ�ĺ�����
	int y;						//·���ڵ��������
	int index;					//·���ڵ���±�
	int robotID;				//·�������Ļ����˱��
};



struct robotAttribute {
	int robotId = -1;				//�����˱��
	int robotDefaultX = 0;			//������ȱʡ������
	int robotDefaultY = 0;			//������ȱʡ������
	int robotType = -1;				//����������
	int robotCurrentX = 0;			//�����˵�ǰ������
	int robotCurrentY = 0;			//�����˵�ǰ������
	int robotNextX = 0;				//��������һ��������
	int robotNextY = 0;				//��������һ��������
	int robotStatus = 0;			//�����˵�ǰ״̬ 0������  1������
	int taskX = 0;					//Ŀ��������
	int taskY = 0;					//Ŀ���������
	int needUpdate = 0;				//�Ƿ���Ҫ��������
	int relatedRobotId = -1;		//Эͬ��������һ��������ID
	int area = 0;					//�û����������������
	int taskId = 0;					//��ǰ����Ļ����˱��
	int goodsType = -1;				//��ǰ���˵Ļ�������
	int goodsId = 0;				//��������С���
	int infoFlag = 0;				//�Ƿ�ɹ����Ե��ͨ��
	int oncePathSize = 0;			//ÿ��·�̼ƻ�����ʱ��
	int onceWorkPreiod = 0;			//ÿ��·��ʵ�ʹ���ʱ��
	int one_wayStep = 0;			//ִ������ʱ����ʵ��ʱ����ȡ��/ж�����������ķֶ�·�̲�ͬ��
	int waitTime = 0;				//����ǰ�ȴ��Է���ʱ��
};



struct Account {
	float balance = 300;				//�����˵�ǰ����ʣ��ֵ
	float last_balance = 300;			//��һ���յ�����ʱ�����
	float yield = 0;					//������
	float param_addWeight = 0.5;		//������Ȩ�����ھ����û������Ǽ��������ºͣ�������ʷ״���ı䣬�����ɸ�
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

//�������
extern int mapWidth;			//1.��ͼ���
extern int mapHeight;			//2.��ͼ�߶�
extern int shelfXnum;			//3.��������
extern int shelfYnum;			//4.��������
extern int areaNum;				//5.��������
extern int goodsTypeNum;		//6.������������
extern int pickingRobotNum;		//7.�������������
extern int freightRobotNum;		//8.�˻�����������
extern int conveyorNum;			//9.���ʹ����������߻�˫�ߣ�
//ȫ�ֱ���
extern int robotNum;			//.����������
extern int globalPeriod;		//.��������
extern int finishAreaNum;       //.������������������
extern int start_x;				//������������һ�����������Ͻ�x����
//ȫ�ֲ�����
const int eachGoodsNum = 48;
const int start_y = 4;
const int shelfWidth = 16, shelfHeight = 11;
const int goodsWidth = 12, goodsHeight = 2;
const int road = 3;





extern vector<RobotInfo> updateRobotInfo;		//ȫ�������˵���Ϣ
extern vector<MapNode> EndNode;
extern vector<MapNode> EndPointRight1;
extern vector<MapNode> EndPointRight2;
extern vector<taskRecord> taskPeriodTime;








#endif // !_PUBLIC_H_


#pragma once
