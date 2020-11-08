#include "calculate.h"
#include "conflict_resolution.h"
#include "goods_area.h"
#include "path_planning.h"
#include "update_file.h"


int CreatInitRobots(void)
{
	int gap = 5;
	int gap1 = 5, gap2 = 5;

	RobotInfo robot;
	vector<RobotInfo> robotInit;

	int robotX, robotY, robotId = 0, robotType;

	robotX = 1;
	gap = 4;
	robotY = 1;
	int time = 0;
	for (int i = 0; i < freightRobotNum; i++) {
		robotId = i + 1;
		robotType = 1;

		robot.attribute.robotId = robotId;
		robot.attribute.robotType = robotType;
		robot.attribute.robotCurrentX = robotX;
		robot.attribute.robotCurrentY = robotY;
		robot.attribute.robotDefaultX = robotX;
		robot.attribute.robotDefaultY = robotY;
		robot.attribute.robotNextX = robotX;
		robot.attribute.robotNextY = robotY;
		robot.attribute.area = 0;
		robot.attribute.goodsType = 0;
		robot.attribute.needUpdate = 0;
		robot.attribute.relatedRobotId = 0;
		robot.attribute.robotStatus = 0;
		robot.attribute.taskId = 0;
		robot.attribute.taskX = 0;
		robot.attribute.taskY = 0;

		robotInit.push_back(robot);
		updateRobotInfo.push_back(robot);

		robotY += gap2;


		if (robotY > mapHeight - 1) {
			time++;
			robotY = 1;

			if (robotX + gap > start_x && robotX + gap <= start_x + (shelfWidth + road) * shelfXnum) {
				gap = shelfWidth + road;
			}
			else if (robotX + gap > start_x + (shelfWidth + road) * shelfXnum) {
				gap = 4;
			}

			robotX += gap;
			/*if (time >= shelfXnum + 1) {
				gap2 = gap1;
				robotX += gap1;
			}
			else {
				robotX += shelfWidth + road;
			}*/
		}
	}

	//拣货机器人
	gap = shelfWidth + road;
	robotX = start_x + shelfWidth;
	robotY = 4;

	for (int i = 0; i < pickingRobotNum; i++) {
		robotId = i + 1 + freightRobotNum;
		robotType = 0;


		robot.attribute.robotId = robotId;
		robot.attribute.robotType = robotType;
		robot.attribute.robotCurrentX = robotX;
		robot.attribute.robotCurrentY = robotY;
		robot.attribute.robotDefaultX = robotX;
		robot.attribute.robotDefaultY = robotY;
		robot.attribute.robotNextX = robotX;
		robot.attribute.robotNextY = robotY;
		robot.attribute.area = 0;
		robot.attribute.goodsType = 0;
		robot.attribute.needUpdate = 0;
		robot.attribute.relatedRobotId = 0;
		robot.attribute.robotStatus = 0;
		robot.attribute.taskId = 0;
		robot.attribute.taskX = 0;
		robot.attribute.taskY = 0;

		robotInit.push_back(robot);
		updateRobotInfo.push_back(robot);

		robotY += 4;

		if (robotY >= mapHeight - 3) {
			robotY = 4;
			robotX += gap;
		}
	}

	writeRobot();
	writeRobot2();

	return 0;
}


//让机器人们走去自己的位置开始工作了
int WalkToOwnArea(vector<MapNode> Map)
{
	RobotInfo robot;
	vector<RobotInfo> pickRobotInit;

	int robotID = 0;
	int areaId;
	int robotType;
	int robotNum;
	int robotX = start_x;
	int robotY = start_y;

	//创建运货机器人
	robotType = 1;
	areaId = 1;
	robotNum = 0;
	int time = 0;
	for (int col = 0; col < shelfXnum; col++) {
		for (int row = 0; row < shelfYnum; row++) {
			robotX = start_x - 1 + col * (shelfWidth + road);
			robotY = start_y + row * (shelfHeight + road);
			robotNum = taskProcess[areaId - 1].freiRobotNum;
			time = 0;
			for (int i = 0; i < robotNum; i++) {
				updateRobotInfo[robotID].attribute.robotDefaultX = robotX;
				updateRobotInfo[robotID].attribute.robotDefaultY = robotY;
				updateRobotInfo[robotID].attribute.area = areaId;
				robotID++;

				robotY += 2;
				time += 2;

				if (time >= 11) {
					time = 0;
					robotY = start_y + row * (shelfHeight + road);
					//robotX = start_x - road + col * (shelfWidth + road);
					robotX -= 1;
				}
			}
			areaId++;
		}
	}

	robotType = 0;
	areaId = 1;
	robotNum = 0;
	//创建拣货机器人
	for (int col = 0; col < shelfXnum; col++) {
		robotX = start_x + shelfWidth - 1 + col * (shelfWidth + road);
		for (int row = 0; row < shelfYnum; row++) {
			robotY = start_y + row * (shelfHeight + road);

			robotNum = taskProcess[areaId - 1].pickRobotNum;

			for (int i = 0; i < robotNum; i++) {
				updateRobotInfo[robotID].attribute.robotDefaultX = robotX;
				updateRobotInfo[robotID].attribute.robotDefaultY = robotY;
				updateRobotInfo[robotID].attribute.area = areaId;
				robotID++;

				robotY += 2;
			}
			areaId++;
		}
	}

	for (int i = 0; i < updateRobotInfo.size(); i++) {
		updateRobotInfo[i].attribute.robotStatus = 0;
		updateRobotInfo[i].attribute.taskX = updateRobotInfo[i].attribute.robotDefaultX;
		updateRobotInfo[i].attribute.taskY = updateRobotInfo[i].attribute.robotDefaultY;
		updateRobotInfo[i].robot_map = Map;
		
		bool isSuccess = useAStar(i, updateRobotInfo[i].robot_map);
		if (updateRobotInfo[i].attribute.robotCurrentX == updateRobotInfo[i].attribute.taskX && updateRobotInfo[i].attribute.robotCurrentY == updateRobotInfo[i].attribute.taskY) {
			pushback(i, 1);
		}
		else {
			int a = 0;
		}

		if (updateRobotInfo[i].robot_path.size() > 0) {
			updateRobotInfo[i].attribute.robotNextX = updateRobotInfo[i].robot_path[updateRobotInfo[i].robot_path.size() - 1].x;
			updateRobotInfo[i].attribute.robotNextY = updateRobotInfo[i].robot_path[updateRobotInfo[i].robot_path.size() - 1].y;
		}
	}

	return 0;
}


int calDistance(int x, int y)
{
	int distance = 1000, index = 0;
	for (int node = 0; node < EndNode.size(); node++) {
		if ((abs(x - EndNode[node].x) + abs(y - EndNode[node].y)) < distance) {
			distance = abs(x - EndNode[node].x) + abs(y - EndNode[node].y);
		}
	}

	return distance;
}


int chooseNewEndNode(int x, int y, vector<MapNode> Map)
{
	int distance = 1000, index = -1;
	for (int i = 0; i < EndNode.size(); i++) {
		if (abs(x - EndNode[i].x) + abs(y - EndNode[i].y) < distance && Map[EndNode[i].x + EndNode[i].y * mapWidth].isAccessible == 0) {
			distance = abs(x - EndNode[i].x) + abs(y - EndNode[i].y);
			index = i;
		}
	}

	return index;
}

int chooseHandoverNode(int goodsId, int areaId, int freightRobot)
{
	int goodsCenter = goodsId - (areaId - 1) * eachGoodsNum;				
	int goodsLeft = goodsCenter - 4;
	int goodsRight = goodsCenter + 4;
	int statusLeft = 1, statusRight = 1;

	if (goodsLeft <= 0 || (goodsLeft > 0 && GoodsContents[goodsId - 1 - 4].goods_status == 0))
		statusLeft = 0;

	if (goodsRight > eachGoodsNum || (goodsRight <= eachGoodsNum && GoodsContents[goodsId - 1 + 4].goods_status == 0))
		statusRight = 0;

	if (statusRight == 0) {
		updateRobotInfo[freightRobot].attribute.taskX = updateRobotInfo[updateRobotInfo[freightRobot].attribute.relatedRobotId - 1].attribute.taskX + 1;
		updateRobotInfo[freightRobot].attribute.taskY = updateRobotInfo[updateRobotInfo[freightRobot].attribute.relatedRobotId - 1].attribute.taskY;
	}
	else if (statusRight != 0 && statusLeft == 0) {
		updateRobotInfo[freightRobot].attribute.taskX = updateRobotInfo[updateRobotInfo[freightRobot].attribute.relatedRobotId - 1].attribute.taskX - 1;
		updateRobotInfo[freightRobot].attribute.taskY = updateRobotInfo[updateRobotInfo[freightRobot].attribute.relatedRobotId - 1].attribute.taskY;
	}
	else {
		updateRobotInfo[freightRobot].attribute.taskX = updateRobotInfo[updateRobotInfo[freightRobot].attribute.relatedRobotId - 1].attribute.taskX;
		if (updateRobotInfo[freightRobot].robot_map[updateRobotInfo[updateRobotInfo[freightRobot].attribute.relatedRobotId - 1].attribute.taskX + mapWidth * (updateRobotInfo[updateRobotInfo[freightRobot].attribute.relatedRobotId - 1].attribute.taskY + 1)].isAccessible == 1) {
			updateRobotInfo[freightRobot].attribute.taskY = updateRobotInfo[updateRobotInfo[freightRobot].attribute.relatedRobotId - 1].attribute.taskY - 1;
		}
		else {
			updateRobotInfo[freightRobot].attribute.taskY = updateRobotInfo[updateRobotInfo[freightRobot].attribute.relatedRobotId - 1].attribute.taskY + 1;
		}
	}

	return 0;
}



int ChooseEndPoint(int robotId)
{
	int robotX = updateRobotInfo[robotId].attribute.robotCurrentX;
	int robotY = updateRobotInfo[robotId].attribute.robotCurrentY;

	int value = mapWidth * mapHeight;
	int target = 0;

	for (int i = 0; i < EndNode.size(); i++) {
		if (EndNode[i].nodeType != -1) {
			int distance = abs(EndNode[i].x - robotX) + abs(EndNode[i].y - robotY);
			if (distance < value) {
				value = distance;
				target = i;
			}
		}
	}

	return target;
}



//帮运货机器人规划回程路线
int FreightReturn(int robotId, int target, vector<MapNode> Map)
{
	int endX = EndNode[target].x; 
	int endY = EndNode[target].y;
	int startX = updateRobotInfo[robotId].attribute.robotCurrentX;
	int startY = updateRobotInfo[robotId].attribute.robotCurrentY;

	updateRobotInfo[robotId].attribute.taskX = endX;
	updateRobotInfo[robotId].attribute.taskY = endY;

	PathNode endPoint;
	endPoint.x = endX;
	endPoint.y = endY;
	endPoint.robotID = robotId + 1;
	endPoint.index = endX + endY * mapWidth;

	updateRobotInfo[robotId].robot_path.clear();

	for (int i = 0; i < 5; i++)
		updateRobotInfo[robotId].robot_path.push_back(endPoint);

	updateRobotInfo[robotId - 1].robot_map = CreateFreightRobotMap(updateRobotInfo[robotId - 1].attribute.area, Map);

	bool isSuccess = Astar(updateRobotInfo[robotId].robot_map, mapWidth, mapHeight, startX, startY, endX, endY, updateRobotInfo[robotId].robot_path);

	if (isSuccess == false)
		int a = 0;

	taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].overPeriod += updateRobotInfo[robotId].robot_path.size();
	taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].planPeriod += updateRobotInfo[robotId].robot_path.size();
	
	updateRobotInfo[robotId].attribute.oncePathSize = updateRobotInfo[robotId].robot_path.size();
	updateRobotInfo[robotId].attribute.onceWorkPreiod = 0;

	updateRobotInfo[robotId].attribute.robotNextX = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].x;
	updateRobotInfo[robotId].attribute.robotNextY = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].y;

	return 0;
}




//帮运货机器人规划回程路线
int FreightReturn1(int robotId, int target, vector<MapNode> Map)
{
	int endX = EndNode[target].x;
	int endY = EndNode[target].y;
	int startX = updateRobotInfo[robotId].attribute.robotCurrentX;
	int startY = updateRobotInfo[robotId].attribute.robotCurrentY;

	updateRobotInfo[robotId].attribute.taskX = endX;
	updateRobotInfo[robotId].attribute.taskY = endY;

	PathNode endPoint;
	endPoint.x = endX;
	endPoint.y = endY;
	endPoint.robotID = robotId + 1;
	endPoint.index = endX + endY * mapWidth;

	updateRobotInfo[robotId].robot_path.clear();

	for (int i = 0; i < 5; i++)
		updateRobotInfo[robotId].robot_path.push_back(endPoint);

	updateRobotInfo[robotId].robot_map = createBackMap(updateRobotInfo[robotId].robot_map, updateRobotInfo[robotId].attribute.area);

	bool isSuccess = Astar(updateRobotInfo[robotId].robot_map, mapWidth, mapHeight, startX, startY, endX, endY, updateRobotInfo[robotId].robot_path);

	if (isSuccess == false)
		int a = 0;

	taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].overPeriod += updateRobotInfo[robotId].robot_path.size();
	taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].planPeriod += updateRobotInfo[robotId].robot_path.size();

	updateRobotInfo[robotId].attribute.oncePathSize = updateRobotInfo[robotId].robot_path.size();
	updateRobotInfo[robotId].attribute.onceWorkPreiod = 0;

	updateRobotInfo[robotId].attribute.robotNextX = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].x;
	updateRobotInfo[robotId].attribute.robotNextY = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].y;

	updateRobotInfo[robotId - 1].robot_map = CreateFreightRobotMap(updateRobotInfo[robotId - 1].attribute.area, Map);

	return 0;
}




bool useAStar(int robotId, vector<MapNode> Map)
{
	int startX = updateRobotInfo[robotId].attribute.robotCurrentX;
	int startY = updateRobotInfo[robotId].attribute.robotCurrentY;
	int endX = updateRobotInfo[robotId].attribute.taskX;
	int endY = updateRobotInfo[robotId].attribute.taskY;

	bool isSuccess = Astar(Map, mapWidth, mapHeight, startX, startY, endX, endY, updateRobotInfo[robotId].robot_path);

	if (isSuccess == true) {
		return true;
	}
	else {
		return false;
	}
}





vector<MapNode> CreateFreightRobotMap(int areaId, vector<MapNode> Map)
{
	int startX = start_x, startY = start_y;

	int x = (areaId - 1) / shelfYnum;
	int y = (areaId - 1) % shelfYnum;

	for (int i = startX + x * (shelfWidth + road); i < startX + x * (shelfWidth + road) + shelfWidth; i++) {
		for (int j = startY + y * (shelfHeight + road); j < startY + y * (shelfHeight + road) + shelfHeight; j++) {
			if (Map[i + j * mapWidth].nodeType == 2) {
				Map[i + j * mapWidth].isAccessible = 0;
			}
		}
	}

	return Map;
}


//创建属于拣货机器人的地图（所在货架区域）
//输入为无机器人且只有道路可行的原始静态地图，以及机器人编号
//输出该机器人对应的地图，该机器人所在的货架区域改为可通行，其他全部禁止通行
vector<MapNode> CreatePickingRobotMap(vector<MapNode> Map, int robotId)
{
	//把所有道路设为禁止通行
	for (int j = 0; j < mapHeight; j++) {
		for (int i = 0; i < mapWidth; i++) {
			if (Map[i + j * mapWidth].nodeType == 1) {
				Map[i + j * mapWidth].isAccessible = 1;
			}
		}
	}

	int startX = start_x, startY = start_y;
	int areaId = updateRobotInfo[robotId - 1].attribute.area;
	int x = (areaId - 1) / shelfYnum;
	int y = (areaId - 1) % shelfYnum;

	for (int i = startX + x * (shelfWidth + road); i < startX + x * (shelfWidth + road) + shelfWidth; i++) {
		for (int j = startY + y * (shelfHeight + road); j < startY + y * (shelfHeight + road) + shelfHeight; j++) {
			if (Map[i + j * mapWidth].nodeType == 2) {
				Map[i + j * mapWidth].isAccessible = 0;
			}
		}
	}

	return Map;
}


vector<MapNode> CreateMap(vector<MapNode> Map)
{
	for (int i = 0; i < mapWidth; i++) {
		for (int j = 0; j < mapHeight; j++) {
			if (Map[i + j * mapWidth].nodeType == 1 || Map[i + j * mapWidth].nodeType == 2) {
				Map[i + j * mapWidth].isAccessible = 0;
			}
		}
	}

	return Map;
}


vector<MapNode> CreateNewRobotMap(int robotId, vector<MapNode> Map)
{
	int startX = start_x, startY = start_y;

	int areaId = updateRobotInfo[robotId].attribute.area;
	int x = (areaId - 1) / shelfYnum;
	int y = (areaId - 1) % shelfYnum;

	for (int i = startX + x * (shelfWidth + road); i < startX + x * (shelfWidth + road) + shelfWidth; i++) {
		for (int j = startY + y * (shelfHeight + road); j < startY + y * (shelfHeight + road) + shelfHeight; j++) {
			if (Map[i + j * mapWidth].nodeType == 2) {
				Map[i + j * mapWidth].isAccessible = 0;
			}
		}
	}

	if (Map[updateRobotInfo[robotId].attribute.robotCurrentX + updateRobotInfo[robotId].attribute.robotCurrentY * mapWidth].nodeType == 2) {
		int	x1 = (updateRobotInfo[robotId].attribute.robotCurrentX - start_x + road) / (shelfWidth + road);
		int y1 = (updateRobotInfo[robotId].attribute.robotCurrentY - start_y + road) / (shelfHeight + road);

		for (int i = startX + x1 * (shelfWidth + road); i < startX + x1 * (shelfWidth + road) + shelfWidth; i++) {
			for (int j = startY + y1 * (shelfHeight + road); j < startY + y1 * (shelfHeight + road) + shelfHeight; j++) {
				if (Map[i + j * mapWidth].nodeType == 2) {
					Map[i + j * mapWidth].isAccessible = 0;
				}
			}
		}
	}

	return Map;
}


bool checkNearby(int taskGoodsX, int taskGoodsY, int areaId, int robotId)
{
	bool result = true;
	for (int i = freightRobotNum; i < updateRobotInfo.size(); i++) {
		if (i != robotId) {
			if (updateRobotInfo[i].attribute.area == areaId && updateRobotInfo[i].attribute.robotStatus != 0) {
				int goodsX = GoodsContents[updateRobotInfo[i].attribute.goodsId - 1].goods_X;
				int goodsY = GoodsContents[updateRobotInfo[i].attribute.goodsId - 1].goods_Y;
				if (abs(taskGoodsX - goodsX) + abs(taskGoodsY - goodsY) <= 4)
					result = false;
			}
		}
	}

	return result;
}


bool checkIsInArea(int robotId)
{
	int area = updateRobotInfo[robotId].attribute.area;
	int area_x = (area - 1) / shelfYnum;
	int area_y = (area - 1) % shelfYnum;

	int left_x = area_x * (shelfWidth + road) + start_x;
	int left_y = area_y * (shelfHeight + road) + start_y;

	int robot_x = updateRobotInfo[robotId].attribute.robotCurrentX;
	int robot_y = updateRobotInfo[robotId].attribute.robotCurrentY;

	if (robot_x >= left_x && robot_x < left_x + shelfWidth && robot_y >= left_y && robot_y < left_y + shelfHeight)
		return true;
	else
		return false;
}
