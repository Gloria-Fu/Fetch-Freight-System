#ifndef _GOODS_AREA_H_
#define _GOODS_AREA_H_



#include "public.h"






/*
GoodsAreaInfo������ã����������Ϣ����
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
	int leftX = 0;							//�������ϽǺ�����
	int leftY = 0;							//�������Ͻ�������
	int rightX = 0;							//�������½Ǻ�����
	int rightY = 0;							//�������½�������

	int predictCostTime = 0;

	void clear();

	////////////////////////////////////////�������ʵʱ��Ϣ��ض���
	int currentRobotNum = 0;
	vector<InAreaRobot> InAreaRobotInfo;

	//////////////////////////////////////////////////////////////////

public:
	GoodsAreaInfo();
	~GoodsAreaInfo();

	int areaId = 0;

	int pickRobotNum = 0;
	int freiRobotNum = 0;

	/////////////////////////////////////������񶩵�����ز���
	int taskNum = 0;
	int finishedTaskNum = 0;			//����ɵ���������
	int isAllFinished = 0;				//����Ƿ�����ɸ���������
	int finishPeriod = 0;				//��¼��ɸ�����ȫ�������ʱ��

	vector<task> unFinishedTask;		//δ��ʼִ�е����񶩵�
	vector<task> pickingTask;			//�ѷ������������˵����񶩵�
	queue<task> cooperateTask;			//�ѷ�����˻������˵����񶩵�

	void updateTask0(task task0);
	void updateTask1(int pickingId, int goods_X, int goods_Y, int index);
	void updateTask2(int freightId);
	//////////////////////////////////////////////////////////////////


	//////////////////////////////////////����Ԥ��������˵���ز���
	int pickOrfrei = -1;				//��Ҫ���������or�˻�������
	int pickDistance = 0;				//�����������ɵ���������Ҫ�߹���ƽ�����루�ֶ����ã���׼��
	int freiDistance = 0;				//�˻���������ɵ���������Ҫ�߹���ƽ�����루�ֶ����ã���׼��

	int calTaskTime(int currentTaskNum);
	
	int pickingWaitPeriod = 0;
	int freightWaitPeriod = 0;
};



extern vector<GoodsInfo> GoodsContents;					//������Ϣ
extern vector<vector <GoodsInfo>> GoodsInformation;
extern vector<GoodsAreaInfo> taskProcess;



#endif // !_GOODS_AREA_H_


#pragma once
