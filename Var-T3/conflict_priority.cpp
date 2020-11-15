#include "conflict_priority.h"
#include "calculate.h"
#include "goods_area.h"


int PriorityDecideMethods(int param, int robot_i, int robot_k)
{
	int rePlanRobot, obstacleRobot;

	if (param == 1) {												//1.����
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
		if (robot_i > robot_k) {											//2.�������ȼ�
			rePlanRobot = robot_k;
			obstacleRobot = robot_i;
		}
		else {
			rePlanRobot = robot_i;
			obstacleRobot = robot_k;
		}
	}	
	else if (param == 3) {													//3.������������ȿ�������
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

	return ((rePlanRobot == robot_i) ? robot_i : robot_k);						//return ��Ҫ���¹滮��robotID
}




//������ͻʱ���������Ը����۶���
//��ͻʱ���ļ�Ǯ�Ƿ�Ӧ�����ڱ��������Ľ����ƽ�⻨�ѽ��������������������ƵĹ�ϵ
float CalPricebyRule(int robotId)
{
	float price = 0;
	int status = updateRobotInfo[robotId].attribute.robotStatus;
	int type = updateRobotInfo[robotId].attribute.robotType;
	int distance = 0;

	float fx_status = 0, fx_type = 0, fx_distance = 0;

	float balance = updateRobotInfo[robotId].calBalance.balance;

	//1.����������Ĳ�ͬȡֵ����ʱĬ�ϼ�������˵����ȼ����˻������˸�,����������1
	if (type == 0)
		fx_type = 0.6;
	else
		fx_type = 0.4;

	//2.�����˲�ͬ״̬�Ĳ�ͬȡֵ��Ҫ�ֿ��������������ۣ�Ĭ��״ֵ̬Խ�����ȼ�Խ��
	if (type != 0) {													//�˻�������
		if (status == 0) {												//��ȱʡ���·�ϣ�û������
			fx_status = 0.1;

			//������˵�ǰλ�õ�Ĭ��λ�õľ��룿
			distance = abs(updateRobotInfo[robotId].attribute.robotCurrentX - updateRobotInfo[robotId].attribute.robotDefaultX) + abs(updateRobotInfo[robotId].attribute.robotCurrentY - updateRobotInfo[robotId].attribute.robotDefaultY);
		}
		else if (status == 1) {											//ȥȡ����·�ϣ�������
			fx_status = 0.35;

			//������˵�ǰλ�õ�Ŀ������λ��
			distance = abs(updateRobotInfo[robotId].attribute.robotCurrentX - taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].taskInfo.goodsX) + abs(updateRobotInfo[robotId].attribute.robotCurrentY - taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].taskInfo.goodsY);
		}
		else if (status == 2) {
			fx_status = 0;
		}										//�ͼ�������˽��ӵĹ����У�������(һ�㲻�ñ��ϰɣ����Ǳ��ϵ��㷨Ӧ�����ж��Ƿ�Ҫ���ϣ��ټ������ȼ�)
		else if (status == 3) {											//�������˵����ʹ���·�ϣ�׼��ж����������
			fx_status = 0.55;
			distance = calDistance(updateRobotInfo[robotId].attribute.robotCurrentX, updateRobotInfo[robotId].attribute.robotCurrentY);
			//distance = abs(updateRobotInfo[robotId].attribute.robotCurrentX - EndNode[0].x);	//��ʱֻ�ܴ��Թ��ƻ����˵����ʹ��ĺ��������
		}
		else {
			int a = 0;
		}
	}
	else if (type == 0) {												//���������
		if (status == 0) {												//û��������ԭ�ش��ţ����߻�ȱʡ��·�ϣ�������ȱʡ��λ�ã�һ��Ӧ�ò�����
			fx_status = 0.3;											//ǰ����·���İɣ����û��·���Ľ������˰ɣ�
			if (updateRobotInfo[robotId].robot_path.size() == 0) {
				int a = 0;
			}

			distance = abs(updateRobotInfo[robotId].attribute.robotCurrentX - updateRobotInfo[robotId].attribute.robotDefaultX) + abs(updateRobotInfo[robotId].attribute.robotCurrentY - updateRobotInfo[robotId].attribute.robotDefaultY);
		}
		else if (status == 1) {											//ȥȡ����·�ϣ�������
			fx_status = 0.7;

			distance = abs(updateRobotInfo[robotId].attribute.robotCurrentX - taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].taskInfo.goodsX) + abs(updateRobotInfo[robotId].attribute.robotCurrentY - taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].taskInfo.goodsY);
		}
		else if (status == 2) {}										//ȡ���Ĺ��̣���ֹ����
		else if (status == 3) {}										//���˻������˽��ӣ���ֹ����
	}

	//3.�����������Ŀ���ľ��룬����Խ�������ȼ�Խ�ߣ�(�ⲿ�����������⣬��Ϊ�����˵�taskλ���Ѿ��������˰�)
	distance = abs(updateRobotInfo[robotId].attribute.robotCurrentX - updateRobotInfo[robotId].attribute.taskX) + abs(updateRobotInfo[robotId].attribute.robotCurrentY - updateRobotInfo[robotId].attribute.taskY);

	//ÿ��������ÿ����Ҫ�����ľ��붼��һ����Ӧ�ò�����ͬһ�ױ�׼�ƶ�

	fx_distance = calLine(updateRobotInfo[robotId].attribute.area, updateRobotInfo[robotId].attribute.robotType, distance);

	float sumPrice = fx_status + fx_distance + fx_type + updateRobotInfo[robotId].calBalance.param_addWeight;

	if (balance <= sumPrice) {											//���������ʣ������ܷ��ߵ��յ㣬��ҪԤ���Ǹ�ʣ����룿
		return 0;
	}
	else {
		return sumPrice;
	}

}


float ProgressFirst(int robotId)
{
	///////////////////////////////////////////////////////////////////////////////////////////////�����������ʱ����뵽����Ȩ��
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


float Judge0(int robotId, float price_get)			//�õ��Ǹ�
{
	int extraPath = 2;

	if (2 * price_get > extraPath) {				//���߶��ˣ�flag-1
		updateRobotInfo[robotId].calBalance.flag -= 1;
	}
	else if (2 * price_get <= extraPath) {			//���ߴ��ˣ�flag+1
		updateRobotInfo[robotId].calBalance.flag += 1;
	}

	return 0;
}


float Judge1(int robotId, float price_out, float price_get)			//���õ��Ǹ�
{
	int extraPath = 2;

	if (price_get + price_out > extraPath) {						//���ߴ��ˣ�flag-1
		updateRobotInfo[robotId].calBalance.flag -= 1;
	}
	else if (price_get + price_out <= extraPath && price_out > 1.5 * price_get) {					//���ߴ��ˣ�flag-1
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



//����Ŀǰ����Ӧ�ð��������λ��+�����˵��������ƶ�
//ÿ������Ľ������ǻ�����������������������Ϊ�ͷ���
int CalReward(int taskId, int area)
{
	//1.�����������˵�����
	float pick_reward = 8;
	int pickId = taskPeriodTime[taskId].taskInfo.pickingRobotId - 1;

	float pickCostTime = taskPeriodTime[taskId].pickRobotWorkEndTime - taskPeriodTime[taskId].pickRobotWorkStartTime - taskPeriodTime[taskId].pickWaitTime - 30;

	updateRobotInfo[pickId].calBalance.balance += pick_reward;

	//2.�����˻������˵�����
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
