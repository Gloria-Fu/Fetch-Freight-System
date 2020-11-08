#include "goods_area.h"





GoodsAreaInfo::GoodsAreaInfo() = default;
GoodsAreaInfo::~GoodsAreaInfo() = default;

void GoodsAreaInfo::updateTask0(task task0) {
	unFinishedTask.push_back(task0);
}



void GoodsAreaInfo::updateTask1(int pickingId, int goods_X, int goods_Y, int index) {
	task task1;
	task1 = unFinishedTask[index];
	task1.goodsX = goods_X;
	task1.goodsY = goods_Y;
	task1.status = 1;
	task1.pickingRobotId = pickingId;
	pickingTask.push_back(task1);
	unFinishedTask.erase(unFinishedTask.begin() + index);
}



void GoodsAreaInfo::updateTask2(int freightId) {
	task task2;
	task2 = pickingTask.front();
	task2.status = 2;
	task2.freightRobotId = freightId;
	cooperateTask.push(task2);
	pickingTask.erase(pickingTask.begin());
}



void GoodsAreaInfo::clear() {
	vector<task> empty1;
	swap(empty1, unFinishedTask);
	vector<task> empty2;
	swap(empty2, pickingTask);
	queue<task> empty3;
	swap(empty3, cooperateTask);

	finishedTaskNum = 0;
	isAllFinished = 0;
}



int GoodsAreaInfo::calTaskTime(int currentTaskNum) {

	int pickTime = pickDistance / pickRobotNum;
	int freiTime = freiDistance / freiRobotNum;

	int taskTime = pickTime > freiTime ? pickTime : freiTime;

	predictCostTime = currentTaskNum * taskTime;

	pickOrfrei = pickTime > freiTime ? 0 : 1;

	return predictCostTime;
}



