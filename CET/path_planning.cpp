#include "path_planning.h"
#include "conflict_resolution.h"
#include "calculate.h"


//1to2:任务从状态1变成状态2，即任务从已分配到拣货机器人到已分配到运货机器人，需要为运货机器人规划去取货的路径
int PlanPath1to2(int param, vector<MapNode> Map, int robotId)
{
	if (param == 1) {
		PlanToGoodsArea(robotId, Map);							//1.两级路径规划
	}
	else if (param == 2) {										//2.A*
		updateRobotInfo[robotId].attribute.taskX = updateRobotInfo[updateRobotInfo[robotId].attribute.relatedRobotId - 1].attribute.taskX + 1;
		updateRobotInfo[robotId].attribute.taskY = updateRobotInfo[updateRobotInfo[robotId].attribute.relatedRobotId - 1].attribute.taskY;

		updateRobotInfo[robotId].robot_map = CreateNewRobotMap(robotId, Map);

		updateRobotInfo[robotId].robot_path.clear();										

		bool isSuccess = useAStar(robotId, updateRobotInfo[robotId].robot_map);		
		if (isSuccess == false) {
			if (updateRobotInfo[robotId].attribute.robotCurrentX == updateRobotInfo[robotId].attribute.taskX && updateRobotInfo[robotId].attribute.robotCurrentY == updateRobotInfo[robotId].attribute.taskY) {
				pushback(robotId, 1);
			}
			else {
				int a = 0;
			}
		}
	}
	else if (param == 3) {										//3.单行道
		updateRobotInfo[robotId].attribute.taskX = updateRobotInfo[updateRobotInfo[robotId].attribute.relatedRobotId - 1].attribute.taskX + 1;
		updateRobotInfo[robotId].attribute.taskY = updateRobotInfo[updateRobotInfo[robotId].attribute.relatedRobotId - 1].attribute.taskY;

		updateRobotInfo[robotId].robot_map = createComeMap(updateRobotInfo[robotId].robot_map, updateRobotInfo[robotId].attribute.area, updateRobotInfo[robotId].attribute.robotCurrentX, updateRobotInfo[robotId].attribute.robotCurrentY, 0, robotId);

		updateRobotInfo[robotId].robot_path.clear();

		bool isSuccess = useAStar(robotId, updateRobotInfo[robotId].robot_map);
		if (isSuccess == false) {
			if (updateRobotInfo[robotId].attribute.robotCurrentX == updateRobotInfo[robotId].attribute.taskX && updateRobotInfo[robotId].attribute.robotCurrentY == updateRobotInfo[robotId].attribute.taskY) {
				pushback(robotId, 1);
			}
			else {
				int a = 0;
			}
		}
		updateRobotInfo[robotId].robot_map = CreateNewRobotMap(robotId, Map);
	}

	updateRobotInfo[robotId].attribute.robotNextX = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].x;
	updateRobotInfo[robotId].attribute.robotNextY = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].y;
	updateRobotInfo[robotId].attribute.oncePathSize = updateRobotInfo[robotId].robot_path.size();
	updateRobotInfo[robotId].attribute.onceWorkPreiod = 0;

	return 0;
}

//1to2:任务从状态3变成状态4，即任务已经被两个机器人交接完了，需要为运货机器人规划去卸货的路径
int PlanPath3to4(int param, vector<MapNode> Map, int robotId)
{
	if (param == 1) {
		LeaveGoodsArea(Map, robotId);							//1.两级路径规划
	}
	else if (param == 2) {										//2.A*
		int target = ChooseEndPoint(robotId);					//获取最近的终点
		FreightReturn(robotId, target, Map);					//规划返程路径
	}
	else if (param == 3) {										//3.单行道
		int target = ChooseEndPoint(robotId);					//获取最近的终点
		FreightReturn1(robotId, target, Map);					//规划返程路径
	}

	return 0;
}

//4to5:任务从状态4变成状态5，即任务已经被完成卸货了，为运货机器人规划回默认位置的路径
int PlanPath4to5(int param, vector<MapNode> Map, int robotId)
{
	if (param == 1) {
		PlanToGoodsArea(robotId, Map);							//1.两级路径规划
	}
	else if (param == 2) {										//2.A*
		updateRobotInfo[robotId].robot_map = Map;
		updateRobotInfo[robotId].robot_map[updateRobotInfo[robotId].attribute.robotDefaultX + updateRobotInfo[robotId].attribute.robotDefaultY * mapWidth].isAccessible = 0;

		bool isSuccess = useAStar(robotId, updateRobotInfo[robotId].robot_map);
		if (isSuccess == false) {
			if (updateRobotInfo[robotId].attribute.robotCurrentX == updateRobotInfo[robotId].attribute.taskX && updateRobotInfo[robotId].attribute.robotCurrentY == updateRobotInfo[robotId].attribute.taskY) {
				pushback(robotId, 1);
			}
			else {
				int a = 0;
			}
		}
	}
	else if (param == 3) {										//3.单行道
		updateRobotInfo[robotId].robot_map = createComeMap(Map, updateRobotInfo[robotId].attribute.area, updateRobotInfo[robotId].attribute.robotCurrentX, updateRobotInfo[robotId].attribute.robotCurrentY, 1, robotId);
		updateRobotInfo[robotId].robot_map[updateRobotInfo[robotId].attribute.robotDefaultX + updateRobotInfo[robotId].attribute.robotDefaultY * mapWidth].isAccessible = 0;

		bool isSuccess = useAStar(robotId, updateRobotInfo[robotId].robot_map);
		if (isSuccess == false) {
			if (updateRobotInfo[robotId].attribute.robotCurrentX == updateRobotInfo[robotId].attribute.taskX && updateRobotInfo[robotId].attribute.robotCurrentY == updateRobotInfo[robotId].attribute.taskY) {
				pushback(robotId, 1);
			}
			else {
				int a = 0;
			}
		}

		updateRobotInfo[robotId].robot_map = Map;
	}

	updateRobotInfo[robotId].attribute.robotNextX = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].x;
	updateRobotInfo[robotId].attribute.robotNextY = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].y;

	updateRobotInfo[robotId].attribute.oncePathSize = updateRobotInfo[robotId].robot_path.size();
	updateRobotInfo[robotId].attribute.onceWorkPreiod = 0;

	return 0;
}



int PlanToGoodsArea(int robotId, vector<MapNode> Map)
{
	updateRobotInfo[robotId].highLevelPath = PathPlanningOnTopology1(robotId, Map);

	if (updateRobotInfo[robotId].highLevelPath.size() > 1) {
		Node2D firstNode;
		firstNode.x = updateRobotInfo[robotId].highLevelPath[updateRobotInfo[robotId].highLevelPath.size() - 1].x * (shelfWidth + road) + start_x - 2;
		firstNode.y = updateRobotInfo[robotId].highLevelPath[updateRobotInfo[robotId].highLevelPath.size() - 1].y * (shelfHeight + road) + start_y - 2;

		if (updateRobotInfo[robotId].attribute.robotStatus == 1) {
			updateRobotInfo[robotId].robot_map = CreateFreightRobotMap(updateRobotInfo[robotId].attribute.area, Map);							//创建该机器人地图
		}
		else if (updateRobotInfo[robotId].attribute.robotStatus == 3) {
			updateRobotInfo[robotId].robot_map = Map;
		}

		updateRobotInfo[robotId].robot_path.clear();

		bool isSuccess = Astar(updateRobotInfo[robotId].robot_map, mapWidth, mapHeight, updateRobotInfo[robotId].attribute.robotCurrentX,	//规划机器人从当前位置到第一个节点的路径
			updateRobotInfo[robotId].attribute.robotCurrentY, firstNode.x, firstNode.y, updateRobotInfo[robotId].robot_path);
		
		if (isSuccess == false) {
			if (updateRobotInfo[robotId].attribute.robotCurrentX == firstNode.x && updateRobotInfo[robotId].attribute.robotCurrentY == firstNode.y) {
				pushback(robotId, 1);
			}
			else {
				int a = 0;
			}
		}
		updateRobotInfo[robotId].attribute.taskX = firstNode.x;
		updateRobotInfo[robotId].attribute.taskY = firstNode.y;
	}
	else if (updateRobotInfo[robotId].highLevelPath.size() <= 1) {

		if (updateRobotInfo[robotId].attribute.robotStatus == 3) {
			updateRobotInfo[robotId].robot_map = Map;
			updateRobotInfo[robotId].attribute.taskX = updateRobotInfo[robotId].attribute.robotDefaultX;
			updateRobotInfo[robotId].attribute.taskY = updateRobotInfo[robotId].attribute.robotDefaultY;
		}
		else if (updateRobotInfo[robotId].attribute.robotStatus == 1) {
			updateRobotInfo[robotId].robot_map = CreateFreightRobotMap(updateRobotInfo[robotId].attribute.area, Map);							//创建该机器人地图

			updateRobotInfo[robotId].attribute.taskX = updateRobotInfo[updateRobotInfo[robotId].attribute.relatedRobotId - 1].attribute.taskX + 1;
			updateRobotInfo[robotId].attribute.taskY = updateRobotInfo[updateRobotInfo[robotId].attribute.relatedRobotId - 1].attribute.taskY;

			if (updateRobotInfo[robotId].highLevelPath.size() == 0) {
				updateRobotInfo[robotId].robot_map = CreateNewRobotMap(robotId, updateRobotInfo[robotId].robot_map);
			}
		}
		updateRobotInfo[robotId].robot_path.clear();
		
		bool isSuccess = Astar(updateRobotInfo[robotId].robot_map, mapWidth, mapHeight, updateRobotInfo[robotId].attribute.robotCurrentX, updateRobotInfo[robotId].attribute.robotCurrentY,
			updateRobotInfo[robotId].attribute.taskX, updateRobotInfo[robotId].attribute.taskY, updateRobotInfo[robotId].robot_path);
		
		if (isSuccess == false) {
			if (updateRobotInfo[robotId].attribute.robotCurrentX == updateRobotInfo[robotId].attribute.taskX &&
				updateRobotInfo[robotId].attribute.robotCurrentY == updateRobotInfo[robotId].attribute.taskY) {
				pushback(robotId, 1);
			}
			else {
				int a = 0;
			}
		}
		
		updateRobotInfo[robotId].highLevelPath.clear();
	}
	else {
		int a = 0;
	}

	return 0;
}




//运货机器人规划如何前往货区,先计算出起点在哪（某条路或者卸货区），然后选择终点，终点是目标货区的四个角
//由于是接到任务后的规划，因此可以锁定目标区域，与平常回默认位置不一样，这是有工作状态的情况
vector<Node2D> PathPlanningOnTopology1(int robotId, vector<MapNode> Map)
{
	vector<Node2D> temptopologyPath;

	int areaId = updateRobotInfo[robotId].attribute.area;
	int robotX = updateRobotInfo[robotId].attribute.robotCurrentX;
	int robotY = updateRobotInfo[robotId].attribute.robotCurrentY;

	vector<Node2D> startPoints = OnWhichRoad(robotX, robotY, CreatPoints());

	if (startPoints.size() == 0) {				//机器人在区域内，返回0
		return temptopologyPath;
	}

	vector<Node2D> endPoints;
	Node2D node;

	//现在暂时是不管机器人在路上还是在卸货区，都选离该机器人最近的两个十字路口点作为起始点，目标货区的四个角作为终点进行上层路径规划
	int area_x, area_y;

	if (areaId > 0) {
		area_x = (areaId - 1) / shelfYnum;
		area_y = (areaId - 1) % shelfYnum;
	}

	if (updateRobotInfo[robotId].attribute.robotStatus == 1 && areaId != 0) {					//1.假如机器人收到了任务，要从当前位置重新规划，终点为货区四个角都可
		node.x = area_x;
		node.y = area_y;
		endPoints.push_back(node);
		node.x += 1;
		endPoints.push_back(node);
		node.y += 1;
		endPoints.push_back(node);
		node.x -= 1;
		endPoints.push_back(node);
	}
	else if (updateRobotInfo[robotId].attribute.robotStatus == 0 && areaId != 0) {				//2.机器人卸完货要回去默认位置，终点为货区左侧的两个点，到了之后再具体规划
		node.x = area_x;
		node.y = area_y;
		endPoints.push_back(node);
		node.y += 1;
		endPoints.push_back(node);
	}
	else if (updateRobotInfo[robotId].attribute.robotStatus == 0 && areaId == 0) {				//3.机动机器人没有区域的说法，要通过其默认位置判断要去的位置
		int taskX = updateRobotInfo[robotId].attribute.robotDefaultX;
		int taskY = updateRobotInfo[robotId].attribute.robotDefaultY;

		endPoints = OnWhichRoad(taskX, taskY, CreatPoints());
	}
	else {
		int a = 0;
	}

	vector<vector<Node2D>> topologyPath;

	for (int i = 0; i < startPoints.size() * endPoints.size(); i++)
		topologyPath.push_back(vector<Node2D>());



	int k = 0;
	for (int j = 0; j < startPoints.size(); j++) {										//计算每个起点和每个终点的路径
		for (int i = 0; i < endPoints.size(); i++) {
			bool isSuccess = Astar2(shelfXnum + 1, shelfYnum + 1, startPoints[j].x, startPoints[j].y, endPoints[i].x, endPoints[i].y, topologyPath[k]);	//计算出所有的可能路径
			k++;
			if (isSuccess == false)
				int a = 0;
		}
	}



	int minPathSize = 1000;
	vector<int> minPathIndex;
	vector<Node2D> minStartNodes, minEndNodes;
	for (int i = 0; i < topologyPath.size(); i++) {										//选出最短的路径们
		if (topologyPath[i].size() <= minPathSize) {
			minPathSize = topologyPath[i].size();
		}
	}
	for (int i = 0; i < topologyPath.size(); i++) {
		if (topologyPath[i].size() == minPathSize) {
			minPathIndex.push_back(i);
			minStartNodes.push_back(topologyPath[i][topologyPath[i].size() - 1]);		//选出最短的路径里面的起点
			minEndNodes.push_back(topologyPath[i][0]);									//选出最短的路径里面的终点
		}
	}

	int minDistance = 1000;
	int nodeIndex = -1;
	int endX = 0, enxY = 0;
	int sDistance = 0, eDistance = 0;
	for (int i = 0; i < minPathIndex.size(); i++) {
		sDistance = abs(robotX - (minStartNodes[i].x * (shelfWidth + road) + start_x - 2)) + abs(robotY - (minStartNodes[i].y * (shelfHeight + road) + start_y - 2));

		if (updateRobotInfo[robotId].attribute.robotStatus == 1) {
			endX = taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].taskInfo.goodsX;
			enxY = taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].taskInfo.goodsY;
		}
		else if (updateRobotInfo[robotId].attribute.robotStatus == 0) {
			endX = updateRobotInfo[robotId].attribute.robotDefaultX;
			enxY = updateRobotInfo[robotId].attribute.robotDefaultY;
		}
		eDistance = abs(endX - (minEndNodes[i].x * (shelfWidth + road) + start_x - 2)) + abs(enxY - (minEndNodes[i].y * (shelfHeight + road) + start_y - 2));

		int diff = sDistance + eDistance;
		if (minDistance > diff) {
			minDistance = diff;
			nodeIndex = i;
		}
	}

	temptopologyPath = topologyPath[minPathIndex[nodeIndex]];			//暂时使用这条路径

	return temptopologyPath;
}



//运货机器人离开货区的操作 1：根据当前交通情况计算要经过的卸货区的路段	2：选择距离第一个要经过的道路节点相近且合理的点离开区域 3：路径规划
int LeaveGoodsArea(vector<MapNode> Map, int robotId)
{
	updateRobotInfo[robotId].highLevelPath = PathPlanningOnTopology(robotId, updateRobotInfo[robotId].attribute.area, CreatPoints());

	//tempPath是拓扑图节点的路径，如何控制机器人走这些路段呢？

	if (updateRobotInfo[robotId].highLevelPath.size() > 1) {
		Node2D firstNode;												//是用热力值地图规划好上层路径后要经过的第一个节点的坐标位置（实际上并不是一定要到这个点）
		firstNode.x = updateRobotInfo[robotId].highLevelPath[updateRobotInfo[robotId].highLevelPath.size() - 1].x;
		firstNode.y = updateRobotInfo[robotId].highLevelPath[updateRobotInfo[robotId].highLevelPath.size() - 1].y;

		//接下来选择要离开的点（这个点是在货架区域靠近边缘的一个点）
		//1.首先判断自己在哪里，如果直接A*会不会出现很多相似的位置然后撞车
		int firstX = firstNode.x * (shelfWidth + road) + start_x - 2;
		int firstY = firstNode.y * (shelfHeight + road) + start_y - 2;

		int robotX = updateRobotInfo[robotId].attribute.robotCurrentX;
		int robotY = updateRobotInfo[robotId].attribute.robotCurrentY;

		if (robotY > firstY) {
			firstY++;
		}
		else if (robotY < firstY) {
			firstY--;
		}

		updateRobotInfo[robotId].attribute.taskX = firstX;
		updateRobotInfo[robotId].attribute.taskY = firstY;

		//updateRobotInfo[robotId].robot_map = CreateFreightRobotMap(updateRobotInfo[robotId].attribute.area, Map);
		updateRobotInfo[robotId].robot_map = createBackMap00(Map, updateRobotInfo[robotId].attribute.area);
		updateRobotInfo[robotId].robot_path.clear();

		bool isSuccess = Astar(updateRobotInfo[robotId].robot_map, mapWidth, mapHeight, robotX, robotY, firstX, firstY, updateRobotInfo[robotId].robot_path);
		if (isSuccess == false) {
			if (robotX == firstX && robotY == firstY) {
				pushback(robotId, 1);
			}
			else {
				int a = 0;
			}
		}
		updateRobotInfo[robotId].attribute.robotNextX = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].x;
		updateRobotInfo[robotId].attribute.robotNextY = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].y;
		updateRobotInfo[robotId].attribute.oncePathSize = updateRobotInfo[robotId].robot_path.size();
		updateRobotInfo[robotId].attribute.onceWorkPreiod = 0;

		taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].planPeriod += updateRobotInfo[robotId].robot_path.size();
	}
	//干脆跟道路的一样，直接打横走到卸货区再说
	else if (updateRobotInfo[robotId].highLevelPath.size() == 1) {		//应该只有靠近传送带那一列的货区的high-level长度为0吧，直接选不好吧，毕竟还没进去那个区域，不懂行情啊
		//假如真的能判断是因为靠近传送带而不需要走到节点
		int direction = 0, enterX, startX;
		if (updateRobotInfo[robotId].attribute.area < shelfXnum * shelfYnum / 2) {
			enterX = start_x - 2;
			direction = 1;
		}
		else {
			enterX = shelfXnum * (road + shelfWidth) + start_x - 2;
			direction = -1;
		}
		startX = updateRobotInfo[robotId].attribute.robotCurrentX;

		PathNode pathnode;
		pathnode.x = enterX;
		pathnode.y = updateRobotInfo[robotId].attribute.robotCurrentY;
		for (int step = 0; step < abs(enterX - startX); step++) {
			pathnode.robotID = robotId + 1;
			pathnode.index = pathnode.x + pathnode.y * mapWidth;
			updateRobotInfo[robotId].robot_path.push_back(pathnode);
			pathnode.x += direction;
		}

		updateRobotInfo[robotId].highLevelPath.pop_back();

		updateRobotInfo[robotId].attribute.taskX = updateRobotInfo[robotId].robot_path[0].x;
		updateRobotInfo[robotId].attribute.taskY = updateRobotInfo[robotId].robot_path[0].y;
		updateRobotInfo[robotId].attribute.robotNextX = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].x;
		updateRobotInfo[robotId].attribute.robotNextY = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].y;
		updateRobotInfo[robotId].attribute.oncePathSize = updateRobotInfo[robotId].robot_path.size();
		updateRobotInfo[robotId].attribute.onceWorkPreiod = 0;
		taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].planPeriod += abs(enterX - startX);
	}
	else {
		int a = 0;
	}

	return 0;
}


////////////////卸货区的操作	主要是选点的问题
int UnloadGoods(int robotId)
{
	int robotX = updateRobotInfo[robotId].attribute.robotCurrentX;
	int robotY = updateRobotInfo[robotId].attribute.robotCurrentY;

	int minDistance = 1000;
	int minIndex = -1;
	for (int i = 0; i < EndNode.size(); i++) {
		int distance = abs(updateRobotInfo[robotId].attribute.robotCurrentX - EndNode[i].x) + abs(updateRobotInfo[robotId].attribute.robotCurrentY - EndNode[i].y);
		if (distance < minDistance) {
			minDistance = distance;
			minIndex = i;
		}
	}


	updateRobotInfo[robotId].attribute.taskX = EndNode[minIndex].x;
	updateRobotInfo[robotId].attribute.taskY = EndNode[minIndex].y;

	PathNode endPoint;
	endPoint.x = updateRobotInfo[robotId].attribute.taskX;
	endPoint.y = updateRobotInfo[robotId].attribute.taskY;
	endPoint.robotID = robotId + 1;
	endPoint.index = endPoint.x + endPoint.y * mapWidth;
	updateRobotInfo[robotId].robot_path.clear();
	for (int i = 0; i < 5; i++)
		updateRobotInfo[robotId].robot_path.push_back(endPoint);

	bool isSuccess = useAStar(robotId, updateRobotInfo[robotId].robot_map);
	if (isSuccess == false) {
		if (updateRobotInfo[robotId].attribute.robotCurrentX == updateRobotInfo[robotId].attribute.taskX && updateRobotInfo[robotId].attribute.robotCurrentY == updateRobotInfo[robotId].attribute.taskY) {
			pushback(robotId, 1);
		}
		else {
			int a = 0;
		}
	}

	updateRobotInfo[robotId].attribute.oncePathSize = updateRobotInfo[robotId].robot_path.size();
	updateRobotInfo[robotId].attribute.onceWorkPreiod = 0;

	updateRobotInfo[robotId].attribute.robotNextX = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].x;
	updateRobotInfo[robotId].attribute.robotNextY = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].y;

	taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].planPeriod += updateRobotInfo[robotId].robot_path.size();

	return 0;
}


//生成拓扑图的连接节点(坐标表示拓扑图中的坐标，.x.y表示在真实地图中的坐标)
vector<vector<Node2D>> CreatPoints(void)
{
	vector<vector<Node2D>> Points;					//二维数组表示包围各个货区区域的地图节点

	for (int i = 0; i < shelfXnum + 1; i++) {
		Points.push_back(vector<Node2D>());
	}

	int startX = start_x - road;
	int startY = start_y - road;

	Node2D pp;

	for (int row = 0; row < shelfYnum + 1; row++) {					//从左到右，从上到下
		for (int col = 0; col < shelfXnum + 1; col++) {
			//	Points[col][row].x = startX + ((shelfWidth + road) * col + 1);
			//	Points[col][row].y = startY + ((shelfHeight + road) * row + 1);
			pp.x = startX + ((shelfWidth + road) * col + 1);
			pp.y = startY + ((shelfHeight + road) * row + 1);
			Points[col].push_back(pp);
		}
	}

	return Points;
}


//计算两个拓扑图节点之间的道路区域机器人数量（传入的是两个拓扑图坐标，拓扑图点，要求的是拓扑图点对应的真实坐标范围内的机器人数量）
float CalHeatValueBetweenTwoNode(NodeInfo node1, NodeInfo node2, vector<vector<Node2D>> Points)
{
	float heatValue = 0;

	int leftStartX, leftStartY, rightEndX, rightEndY;				//定义每次计算的矩形范围路段的左上角坐标和右下角坐标
	int robotNums = 0;

	if ((node1.x == node2.x && node1.y < node2.y) || (node1.y == node2.y && node1.x < node2.x)) {
		leftStartX = Points[node1.x][node1.y].x - 1;
		leftStartY = Points[node1.x][node1.y].y - 1;
		rightEndX = Points[node2.x][node2.y].x + 1;
		rightEndY = Points[node2.x][node2.y].y + 1;
	}
	else if ((node1.x == node2.x && node1.y > node2.y) || (node1.y == node2.y && node1.x > node2.x)) {
		leftStartX = Points[node2.x][node2.y].x - 1;
		leftStartY = Points[node2.x][node2.y].y - 1;
		rightEndX = Points[node1.x][node1.y].x + 1;
		rightEndY = Points[node1.x][node1.y].y + 1;
	}

	for (int i = 0; i < updateRobotInfo.size(); i++) {				//遍历每个机器人的位置？
		if (updateRobotInfo[i].attribute.robotCurrentX >= leftStartX && updateRobotInfo[i].attribute.robotCurrentX <= rightEndX				//表示这个机器人在这个道路区域内
			&& updateRobotInfo[i].attribute.robotCurrentY >= leftStartY && updateRobotInfo[i].attribute.robotCurrentY <= rightEndY) {
			robotNums++;
		}
	}

	//该热力值应该不能超过1，不能太大，不然在规划的时候，一个节点到另一个节点距离只有1，要是超过1，岂不是意味着要走两段这么长的路？
	//假设正面遇到一个机器人，避障需要多走两步，而本来需要走的距离是road+shelfWidth，那就robotNum*2/(shelfWidth)？？？？可以吗
	heatValue = float(robotNums) / float(road + shelfWidth);			//热力值的计算方式

	return heatValue;
}


//计算传进来的机器人的坐标目前在哪条边上（糟糕，要是在卸货区咋办咧）
//输入的是机器人的真实坐标，以及拓扑图点，要取拓扑图点对应的真实坐标进行计算该机器人的位置
vector<Node2D> OnWhichRoad(int x, int y, vector<vector<Node2D>> Points)
{
	vector<Node2D> nearNodes;
	Node2D node;

	int a = 0;

	static int b = 0;
	b++;
	if (b == 10)
		int c = 0;

	if (x > (shelfWidth + road) * shelfXnum + start_x - road) {						//如果机器人的位置在卸货区域的话，两种方案：（1）卸货区域旁所有点都可以用 
		for (int i = 0; i < shelfYnum; i++) {										//（2）暂时是用最靠近机器人的两个点
			if (y >= Points[shelfXnum][i].y && y < Points[shelfXnum][i + 1].y) {
				node.x = shelfXnum;
				node.y = i;
				nearNodes.push_back(node);
				node.y = i + 1;
				nearNodes.push_back(node);
				a = 1;
				break;
			}
		}

		if (a == 0) {
			if (y < Points[shelfXnum][0].y) {
				node.x = shelfXnum;
				node.y = 0;
				nearNodes.push_back(node);
			}
			else if (y >= Points[shelfXnum][shelfYnum].y) {
				node.x = shelfXnum;
				node.y = shelfYnum;
				nearNodes.push_back(node);
			}
			else {
				int b = 0;
			}
		}


		if (nearNodes.size() == 0) {
			int a = 0;
		}
	}
	else if (x < start_x) {						//如果机器人的位置在卸货区域的话，两种方案：（1）卸货区域旁所有点都可以用 
		for (int i = 0; i < shelfYnum; i++) {										//（2）暂时是用最靠近机器人的两个点
			if (y >= Points[0][i].y && y < Points[0][i + 1].y) {
				node.x = 0;
				node.y = i;
				nearNodes.push_back(node);
				node.y = i + 1;
				nearNodes.push_back(node);
				a = 1;
				break;
			}
		}

		if (a == 0) {
			if (y < Points[0][0].y) {
				node.x = 0;
				node.y = 0;
				nearNodes.push_back(node);
			}
			else if (y >= Points[0][shelfYnum].y) {
				node.x = 0;
				node.y = shelfYnum;
				nearNodes.push_back(node);
			}
			else {
				int b = 0;
			}
		}


		if (nearNodes.size() == 0) {
			int a = 0;
		}
	}
	else {
		for (int i = 0; i < (shelfXnum + 1) * (shelfYnum + 1); i++) {
			Node2D node1, node2;
			node1.x = Points[i / (shelfYnum + 1)][i % (shelfYnum + 1)].x;
			node1.y = Points[i / (shelfYnum + 1)][i % (shelfYnum + 1)].y;

			if (x >= node1.x - 1 && x <= node1.x + 1 && y >= node1.y - 1 && y <= node1.y + 1) {				//在节点附近，就只搞一个起点
				node.x = i / (shelfYnum + 1);
				node.y = i % (shelfYnum + 1);
				nearNodes.push_back(node);
				break;
			}
			else {																							//在两节点中间，搞两个起点
				if (i % (shelfYnum + 1) > 0) {								//上
					node2.x = node1.x;
					node2.y = node1.y - (shelfHeight + road);

					if (x >= node2.x - 1 && x <= node1.x + 1 && y >= node2.y - 1 && y <= node1.y + 1) {
						node.x = i / (shelfYnum + 1);
						node.y = i % (shelfYnum + 1) - 1;
						nearNodes.push_back(node);
						node.y = i % (shelfYnum + 1);
						nearNodes.push_back(node);
						break;
					}
				}

				if (i % (shelfYnum + 1) < shelfYnum) {						//下
					node2.x = node1.x;
					node2.y = node1.y + (shelfHeight + road);

					if (x >= node1.x - 1 && x <= node2.x + 1 && y >= node1.y - 1 && y <= node2.y + 1) {
						node.x = i / (shelfYnum + 1);
						node.y = i % (shelfYnum + 1);
						nearNodes.push_back(node);
						node.x = i / (shelfYnum + 1);
						node.y = i % (shelfYnum + 1) + 1;
						nearNodes.push_back(node);
						break;
					}
				}

				if (i / (shelfYnum + 1) > 0) {								//左
					node2.x = node1.x - (shelfWidth + road);
					node2.y = node1.y;

					if (x >= node2.x - 1 && x <= node1.x + 1 && y >= node2.y - 1 && y <= node1.y + 1) {
						node.x = i / (shelfYnum + 1) - 1;
						node.y = i % (shelfYnum + 1);
						nearNodes.push_back(node);
						node.x = i / (shelfYnum + 1);
						node.y = i % (shelfYnum + 1);
						nearNodes.push_back(node);
						break;
					}
				}

				if (i / (shelfYnum + 1) < shelfXnum) {						//右
					node2.x = node1.x + (shelfWidth + road);
					node2.y = node1.y;

					if (x >= node1.x - 1 && x <= node2.x + 1 && y >= node1.y - 1 && y <= node2.y + 1) {
						node.x = i / (shelfYnum + 1);
						node.y = i % (shelfYnum + 1);
						nearNodes.push_back(node);
						node.x = i / (shelfYnum + 1) + 1;
						node.y = i % (shelfYnum + 1);
						nearNodes.push_back(node);
						break;
					}
				}
			}
		}
	}

	if (nearNodes.size() == 0) {
		int a = 0;
	}

	return nearNodes;
}


//用于给正在货区完成交接后，根据交通热力图计划回程时计算出粗略的拓扑图路径，但是很有可能在走的过程中需要更新这个过程（未确定）
//输入的是区域编号，以及拓扑图点，拓扑图坐标其实和区域编号可以转化（都是个抽象值），需要对拓扑图中的点进行加权A*规划（上层路径规划，得到的是计划要经过的路段）
vector<Node2D> PathPlanningOnTopology(int robotId, int areaId, vector<vector<Node2D>> Points)
{
	int area_x = (areaId - 1) / shelfYnum;
	int area_y = (areaId - 1) % shelfYnum;
	int robotX = updateRobotInfo[robotId].attribute.robotCurrentX;
	int robotY = updateRobotInfo[robotId].attribute.robotCurrentY;

	vector<Node2D> startPoints, endPoints;

	Node2D node;

	node.x = area_x;
	node.y = area_y;
	startPoints.push_back(node);										//运货机器人所在区域的四个方向节点都有机会当做起点节点
	node.y += 1;
	startPoints.push_back(node);
	node.x += 1;
	startPoints.push_back(node);
	node.y -= 1;
	startPoints.push_back(node);

	if (start_x != 4) {
		for (int i = 0; i < shelfYnum + 1; i++) {
			node.x = 0;												//靠近传送带的一整列节点都有机会当做终点节点
			node.y = i;
			endPoints.push_back(node);
		}
	}
	for (int i = 0; i < shelfYnum + 1; i++) {
		node.x = shelfXnum;												//靠近传送带的一整列节点都有机会当做终点节点
		node.y = i;
		endPoints.push_back(node);
	}



	vector<vector<Node2D>> topologyPath;

	for (int i = 0; i < startPoints.size() * endPoints.size(); i++)
		topologyPath.push_back(vector<Node2D>());

	int k = 0;
	for (int j = 0; j < startPoints.size(); j++) {
		for (int i = 0; i < endPoints.size(); i++) {
			bool isSuccess = Astar2(shelfXnum + 1, shelfYnum + 1, startPoints[j].x, startPoints[j].y, endPoints[i].x, endPoints[i].y, topologyPath[k]);	//计算出所有的可能路径
			if (isSuccess == false) {
				int a = 0;
			}
			k++;
		}
	}

	int minPathSize = 1000;
	vector<int> minPathIndex;
	vector<Node2D> minStartNodes;
	for (int i = 0; i < topologyPath.size(); i++) {									//选出最短的路径们
		if (topologyPath[i].size() <= minPathSize) {
			minPathSize = topologyPath[i].size();
		}
	}

	for (int i = 0; i < topologyPath.size(); i++) {
		if (topologyPath[i].size() == minPathSize) {
			minPathIndex.push_back(i);
			minStartNodes.push_back(topologyPath[i][topologyPath[i].size() - 1]);
		}
	}

	//1.计算出要经过的最短的路段们分别的交通热力值
	float minHeatValue = 1000;
	int minIndex = -1;
	float heatValue = 0;
	NodeInfo node1, node2;
	for (int i = 0; i < minPathIndex.size(); i++) {
		heatValue = 0;
		for (int j = 0; j < topologyPath[minPathIndex[i]].size() - 1; j++) {
			node1.x = topologyPath[minPathIndex[i]][j].x;
			node1.y = topologyPath[minPathIndex[i]][j].y;
			node2.x = topologyPath[minPathIndex[i]][j + 1].x;
			node2.y = topologyPath[minPathIndex[i]][j + 1].y;
			heatValue += CalHeatValueBetweenTwoNode(node1, node2, CreatPoints());
		}
		if (heatValue < minHeatValue) {
			minHeatValue = heatValue;
			minIndex = i;
		}
	}

	//2.计算机器人离哪个起点更近
	int minDistance = 1000;
	int nodeIndex = -1;
	for (int i = 0; i < minStartNodes.size(); i++) {								//在最短的路径们中选出起点和机器人最近的那条路
		int distance = abs(robotX - (minStartNodes[i].x * (shelfWidth + road) + start_x - 2)) + abs(robotY - (minStartNodes[i].y * (shelfHeight + road) + start_y - 2));
		if (distance < minDistance) {
			minDistance = distance;
			nodeIndex = i;
		}
	}

	vector<Node2D> temptopologyPath;
	if (nodeIndex == minIndex) {
		temptopologyPath = topologyPath[minPathIndex[nodeIndex]];
	}
	else {
		temptopologyPath = (abs(robotY - (minStartNodes[minIndex].y * (shelfHeight + road) + start_y - 2)) > 9) ? topologyPath[minPathIndex[nodeIndex]] : topologyPath[minPathIndex[minIndex]];
	}

	//temptopologyPath = topologyPath[minPathIndex[minIndex]];						//暂时使用这条路径

	return temptopologyPath;
}


//机器人在到达某个high-level规划的节点后，查询下一个节点，给机器人规划从当前节点到下一个节点的具体路径
//这里算是个局部预规划，到时动态调整的时候要保证机器人的路径从当前位置到最近的下一个节点的位置。
//因此在这里规划完当前节点到下一个节点的路径时，可考虑删除当前节点，但是删了就失去方向了，emmmm再想想吧
//一般就是机器人上一段路走完之后+监测是否到了这个节点的位置：就可以开始下一段路的路径规划了
int ReachNewNode(int robotId, vector<MapNode> Map)
{
	int robotX = updateRobotInfo[robotId].attribute.robotCurrentX;
	int robotY = updateRobotInfo[robotId].attribute.robotCurrentY;

	int flag = 0;

	Node2D node1, node2;
	node1.x = updateRobotInfo[robotId].highLevelPath[updateRobotInfo[robotId].highLevelPath.size() - 1].x * (shelfWidth + road) + start_x - 2;
	node1.y = updateRobotInfo[robotId].highLevelPath[updateRobotInfo[robotId].highLevelPath.size() - 1].y * (shelfHeight + road) + start_y - 2;

	if (robotX >= node1.x - 2 && robotX <= node1.x + 2 && robotY >= node1.y - 2 && robotY <= node1.y + 2) {	//证明机器人在这个节点的八邻域范围内,代表是真的到达了一个节点

		if (updateRobotInfo[robotId].highLevelPath.size() > 1) {											//1.在high-level路径中或未进入high-level地图路径
			node2.x = updateRobotInfo[robotId].highLevelPath[updateRobotInfo[robotId].highLevelPath.size() - 2].x * (shelfWidth + road) + start_x - 2;
			node2.y = updateRobotInfo[robotId].highLevelPath[updateRobotInfo[robotId].highLevelPath.size() - 2].y * (shelfHeight + road) + start_y - 2;

			PathNode tempNode;
			tempNode.index = -1;

			if (updateRobotInfo[robotId].robot_path.size() == 1) {
				tempNode = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1];
				updateRobotInfo[robotId].robot_path.pop_back();
				int a = 0;
				//robotY = node2.y;
				robotX = updateRobotInfo[robotId].attribute.taskX;
				robotY = updateRobotInfo[robotId].attribute.taskY;

			}

			PathNode pathnode;

			if (node1.y == node2.y) {								//水平方向
				int step = node2.x - robotX;
				int direction = 0;
				if (step > 0)										//向右走
					direction = -1;
				else if (step < 0)									//向左走
					direction = 1;
				else 												//在原地
					direction = 0;

				pathnode.x = node2.x;
				pathnode.y = robotY;

				for (int i = 0; i < abs(step); i++) {
					pathnode.robotID = robotId + 1;
					pathnode.index = pathnode.x + pathnode.y * mapWidth;
					updateRobotInfo[robotId].robot_path.push_back(pathnode);
					pathnode.x += direction;
				}

				updateRobotInfo[robotId].attribute.taskX = node2.x;
				updateRobotInfo[robotId].attribute.taskY = robotY;

				if (updateRobotInfo[robotId].attribute.robotStatus > 0)
					taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].planPeriod += abs(step);
			}
			else if (node1.x == node2.x) {							//垂直方向
				int step = node2.y - robotY;
				int direction = 0;
				if (step > 0) 										//向下走
					direction = -1;
				else if (step < 0)									//向上走
					direction = 1;
				else 												//在原地
					direction = 0;

				pathnode.x = robotX;
				pathnode.y = node2.y;

				for (int i = 0; i < abs(step); i++) {
					pathnode.robotID = robotId;
					pathnode.index = pathnode.x + pathnode.y * mapWidth;
					updateRobotInfo[robotId].robot_path.push_back(pathnode);
					pathnode.y += direction;
				}

				updateRobotInfo[robotId].attribute.taskX = robotX;
				updateRobotInfo[robotId].attribute.taskY = node2.y;

				if (updateRobotInfo[robotId].attribute.robotStatus > 0)
					taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].planPeriod += abs(step);
			}

			if (tempNode.index != -1) {
				updateRobotInfo[robotId].robot_path.push_back(tempNode);
			}

			updateRobotInfo[robotId].highLevelPath.pop_back();
		}
		else if (updateRobotInfo[robotId].highLevelPath.size() == 1) {								//2.判断是要去取货还是卸货

			if (updateRobotInfo[robotId].robot_path.size() == 1) {
				updateRobotInfo[robotId].robot_path.pop_back();
			}

			if (updateRobotInfo[robotId].attribute.robotStatus == 1) {								//(1)去取货，已经在目标货区的四周，直接从当前位置进去目标位置吧
				updateRobotInfo[robotId].attribute.taskX = updateRobotInfo[updateRobotInfo[robotId].attribute.relatedRobotId - 1].attribute.taskX + 1;
				updateRobotInfo[robotId].attribute.taskY = updateRobotInfo[updateRobotInfo[robotId].attribute.relatedRobotId - 1].attribute.taskY;
				//chooseHandoverNode(updateRobotInfo[robotId].attribute.goodsId, updateRobotInfo[robotId].attribute.area, robotId);

				updateRobotInfo[robotId].robot_map = CreateFreightRobotMap(updateRobotInfo[robotId].attribute.area, Map);

				bool isSuccess = useAStar(robotId, updateRobotInfo[robotId].robot_map);
				if (isSuccess == false) {
					if (updateRobotInfo[robotId].attribute.robotCurrentX == updateRobotInfo[robotId].attribute.taskX && updateRobotInfo[robotId].attribute.robotCurrentY == updateRobotInfo[robotId].attribute.taskY) {
						pushback(robotId, 1);
					}
					else {
						int a = 0;
						pushback(robotId, 1);
						flag = 1;
						updateRobotInfo[robotId].attribute.taskX = updateRobotInfo[robotId].highLevelPath[updateRobotInfo[robotId].highLevelPath.size() - 1].x * (shelfWidth + road) + start_x - 2;
						updateRobotInfo[robotId].attribute.taskY = updateRobotInfo[robotId].highLevelPath[updateRobotInfo[robotId].highLevelPath.size() - 1].y * (shelfHeight + road) + start_y - 2;

					}
				}
				updateRobotInfo[robotId].attribute.robotNextX = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].x;
				updateRobotInfo[robotId].attribute.robotNextY = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].y;

				if (updateRobotInfo[robotId].attribute.robotStatus > 0)
					taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].planPeriod += updateRobotInfo[robotId].robot_path.size();

			}
			else if (updateRobotInfo[robotId].attribute.robotStatus == 3) {							//(2)去卸货，找个点，规划过去
				UnloadGoods(robotId);
				//	int target = ChooseEndPoint00(robotId);
				//	FreightReturn00(robotId, target, Map);

			}
			else if (updateRobotInfo[robotId].attribute.robotStatus == 0) {							//(3)回货区旁的默认位置
				updateRobotInfo[robotId].attribute.taskX = updateRobotInfo[robotId].attribute.robotDefaultX;
				updateRobotInfo[robotId].attribute.taskY = updateRobotInfo[robotId].attribute.robotDefaultY;

				updateRobotInfo[robotId].robot_map = Map;
				if (globalPeriod == 1056)
					int a = 0;
				bool isSuccess = useAStar(robotId, updateRobotInfo[robotId].robot_map);
				if (isSuccess == false) {
					if (updateRobotInfo[robotId].attribute.robotCurrentX == updateRobotInfo[robotId].attribute.taskX && updateRobotInfo[robotId].attribute.robotCurrentY == updateRobotInfo[robotId].attribute.taskY) {
						pushback(robotId, 1);
					}
					else {
						int a = 0;
					}
				}
				updateRobotInfo[robotId].attribute.robotNextX = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].x;
				updateRobotInfo[robotId].attribute.robotNextY = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].y;
			}
			updateRobotInfo[robotId].highLevelPath.pop_back();
			//taskPeriodTime[updateRobotInfo[robotId].attribute.taskId - 1].planPeriod += updateRobotInfo[robotId].robot_path.size();
		}
		else {
			int a = 0;
			if (updateRobotInfo[robotId].robot_path.size() == 1) {
				int a = 0;
			}
		}
	}
	else {
		int a = 0;
	}

	updateRobotInfo[robotId].attribute.robotNextX = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].x;
	updateRobotInfo[robotId].attribute.robotNextY = updateRobotInfo[robotId].robot_path[updateRobotInfo[robotId].robot_path.size() - 1].y;
	updateRobotInfo[robotId].attribute.oncePathSize = updateRobotInfo[robotId].robot_path.size();
	updateRobotInfo[robotId].attribute.onceWorkPreiod = 0;

	return 0;
}


//计算OpenList中G值最小的点，把这个点从OpenList中去除，作为当前操作节点
NodeInfo popSmallestNode(vector<NodeInfo>& OpenList)
{
	float value = 10000;
	int index = 0;
	for (int i = 0; i < OpenList.size(); i++) {
		float f = OpenList[i].g + OpenList[i].h;
		if (f < value) {
			value = f;
			index = i;
		}
	}
	NodeInfo output = OpenList[index];
	OpenList.erase(OpenList.begin() + index);

	return output;
}

//A*静态地图路径规划算法，用于计算实际地图的A*规划
bool Astar(vector<MapNode> Map, int width, int height, int startX, int startY, int endX, int endY, vector<PathNode>& path)
{
	if (startX < 0 || startX >= width || startY < 0 || startY >= height || endX < 0 || endX >= width || endY < 0 || endY >= height) {
		return false;
	}
	if (Map[startX + startY * width].isAccessible == 1 || Map[endX + endY * width].isAccessible == 1) {
		return false;
	}
	if (startX == endX && startY == endY) {
		return false;
	}

	int directions[4][2] = { {0,-1},{-1,0},{1,0},{0,1} };
	short* isVisited = new short[height * width];		//表示节点状态：0:未访问，1:open 2:close

	for (int i = 0; i < height * width; i++) {
		isVisited[i] = 0;
	}

	NodeInfo* nodes = new NodeInfo[width * height];

	NodeInfo startNode, endNode;
	startNode.x = startX;
	startNode.y = startY;
	startNode.g = 0;
	startNode.h = abs(startNode.x - endX) + abs(startNode.y - endY);	//曼哈顿距离，起始点到目标点的绝对距离
	startNode.f = startNode.g + startNode.h;
	startNode.index = startY * width + startX;
	startNode.preIndex = -1;											//起始点没有父节点，记为-1

	endNode.x = endX;
	endNode.y = endY;

	NodeInfo PredNode;				//当前节点
	NodeInfo SuccNode;				//子节点
	PathNode pathNode;				//路径节点

	int PredId, SuccId;				//节点id

	vector<NodeInfo> OpenList;
	vector<NodeInfo> CloseList;
	OpenList.push_back(startNode);
	isVisited[startNode.index] = 1;
	nodes[startNode.index] = startNode;

	while (!OpenList.empty()) {
		PredNode = popSmallestNode(OpenList);
		PredId = PredNode.y * width + PredNode.x;
		//若已到达终点
		if (PredNode.x == endNode.x && PredNode.y == endNode.y) {
			NodeInfo tempNode = PredNode;
			while (tempNode.preIndex > -1) {
				pathNode.x = tempNode.x;
				pathNode.y = tempNode.y;
				pathNode.index = tempNode.x + width * tempNode.y;
				path.push_back(pathNode);
				tempNode = nodes[tempNode.preIndex];
			}
			pathNode.x = tempNode.x;
			pathNode.y = tempNode.y;
			pathNode.index = tempNode.x + width * tempNode.y;
			//	path.push_back(pathNode);

			delete[] isVisited;
			delete[] nodes;

			return true;
		}
		//仍未找到终点
		isVisited[PredId] = 2;								//把当前点加入到闭集合中
		CloseList.push_back(nodes[PredId]);

		try {
			for (int i = 0; i < 4; i++) {
				int succX = PredNode.x + directions[i][0];
				int succY = PredNode.y + directions[i][1];
				if (succX >= 0 && succX < width && succY >= 0 && succY < height && (Map[succX + succY * width].isAccessible != 1 && isVisited[succY * width + succX] != 2)) {
					//边界检测+障碍物检测+是否在Close表中
					SuccNode.x = succX;
					SuccNode.y = succY;
					SuccNode.index = succY * width + succX;
					SuccNode.preIndex = PredNode.index;
					SuccNode.g = PredNode.g + sqrt((succX - PredNode.x) * (succX - PredNode.x) + (succY - PredNode.y) * (succY - PredNode.y));
					SuccNode.h = abs(SuccNode.x - endNode.x) + abs(succY - endNode.y);
					SuccNode.f = SuccNode.g + SuccNode.h;
					SuccId = SuccNode.y * width + SuccNode.x;

					if (isVisited[SuccId] == 0) {
						isVisited[SuccId] = 1;
						nodes[SuccId] = SuccNode;
						OpenList.push_back(SuccNode);
					}
					else if (isVisited[SuccId] == 1 && SuccNode.f + 0.00001 < nodes[SuccId].f) {
						nodes[SuccId] = SuccNode;
						for (int i = 0; i < OpenList.size(); i++) {
							if (OpenList[i].x == SuccNode.x && OpenList[i].y == SuccNode.y) {
								OpenList[i] = SuccNode;
								break;
							}
						}
					}
				}
			}
		}
		catch (exception e) {
			cout << "ERROR" << e.what() << "\n" << endl;
		}
	}
	delete[] isVisited;
	delete[] nodes;
	return false;
}



//A*静态地图路径规划算法,用于计算拓扑图的A*规划
bool Astar2(int width, int height, int startX, int startY, int endX, int endY, vector<Node2D>& path)
{
	if (startX < 0 || startX >= width || startY < 0 || startY >= height || endX < 0 || endX >= width || endY < 0 || endY >= height) {
		return false;
	}

	int directions[4][2] = { {0,-1},{-1,0},{1,0},{0,1} };
	short* isVisited = new short[height * width];		//表示节点状态：0:未访问，1:open 2:close

	for (int i = 0; i < height * width; i++) {
		isVisited[i] = 0;
	}

	NodeInfo* nodes = new NodeInfo[width * height];

	NodeInfo startNode, endNode;
	startNode.x = startX;
	startNode.y = startY;
	startNode.g = 0;
	startNode.h = abs(startNode.x - endX) + abs(startNode.y - endY);	//曼哈顿距离，起始点到目标点的绝对距离
	startNode.f = startNode.g + startNode.h;
	startNode.index = startY * width + startX;
	startNode.preIndex = -1;											//起始点没有父节点，记为-1

	endNode.x = endX;
	endNode.y = endY;

	NodeInfo PredNode;				//当前节点
	NodeInfo SuccNode;				//子节点
	Node2D pathNode;				//路径节点

	int PredId, SuccId;				//节点id
	float extraCost = 0;

	vector<NodeInfo> OpenList;
	vector<NodeInfo> CloseList;
	OpenList.push_back(startNode);
	isVisited[startNode.index] = 1;
	nodes[startNode.index] = startNode;

	while (!OpenList.empty()) {
		PredNode = popSmallestNode(OpenList);
		PredId = PredNode.y * width + PredNode.x;
		//若已到达终点
		if (PredNode.x == endNode.x && PredNode.y == endNode.y) {
			NodeInfo tempNode = PredNode;
			while (tempNode.preIndex > -1) {
				pathNode.x = tempNode.x;
				pathNode.y = tempNode.y;
				//	pathNode.index = tempNode.x + width * tempNode.y;
				path.push_back(pathNode);

				tempNode = nodes[tempNode.preIndex];
			}
			pathNode.x = tempNode.x;
			pathNode.y = tempNode.y;
			//	pathNode.index = tempNode.x + width * tempNode.y;
			path.push_back(pathNode);

			delete[] isVisited;
			delete[] nodes;

			return true;
		}
		//仍未找到终点
		isVisited[PredId] = 2;								//把当前点加入到闭集合中
		CloseList.push_back(nodes[PredId]);

		try {
			for (int i = 0; i < 4; i++) {
				int succX = PredNode.x + directions[i][0];
				int succY = PredNode.y + directions[i][1];
				if (succX >= 0 && succX < width && succY >= 0 && succY < height) {
					//边界检测+障碍物检测+是否在Close表中
					SuccNode.x = succX;
					SuccNode.y = succY;
					SuccNode.index = succY * width + succX;
					SuccNode.preIndex = PredNode.index;
					extraCost = CalHeatValueBetweenTwoNode(PredNode, SuccNode, CreatPoints());
					SuccNode.g = PredNode.g + sqrt((succX - PredNode.x) * (succX - PredNode.x) + (succY - PredNode.y) * (succY - PredNode.y)) + extraCost;
					SuccNode.h = abs(SuccNode.x - endNode.x) + abs(succY - endNode.y);
					SuccNode.f = SuccNode.g + SuccNode.h;
					SuccId = SuccNode.y * width + SuccNode.x;

					if (isVisited[SuccId] == 0) {
						isVisited[SuccId] = 1;
						nodes[SuccId] = SuccNode;
						OpenList.push_back(SuccNode);
					}
					else if (isVisited[SuccId] == 1 && SuccNode.f + 0.00001 < nodes[SuccId].f) {
						nodes[SuccId] = SuccNode;
						for (int i = 0; i < OpenList.size(); i++) {
							if (OpenList[i].x == SuccNode.x && OpenList[i].y == SuccNode.y) {
								OpenList[i] = SuccNode;
								break;
							}
						}
					}
				}
			}
		}
		catch (exception e) {
			cout << "ERROR" << e.what() << "\n" << endl;
		}
	}
	delete[] isVisited;
	delete[] nodes;
	return false;
}


vector<MapNode> createBackMap00(vector<MapNode> Map, int area)
{
	int startX = start_x, startY = start_y;

	int x = (area - 1) / shelfYnum;
	int y = (area - 1) % shelfYnum;

	//1.把该货区点亮了
	for (int i = startX + x * (shelfWidth + road); i < startX + x * (shelfWidth + road) + shelfWidth; i++) {
		for (int j = startY + y * (shelfHeight + road); j < startY + y * (shelfHeight + road) + shelfHeight; j++) {
			if (Map[i + j * mapWidth].nodeType == 2) {
				Map[i + j * mapWidth].isAccessible = 0;
			}
		}
	}


	if ((EndNode[0].x != EndNode[EndNode.size() - 1].x && (x != 0 && x != shelfXnum - 1)) || (EndNode[0].x == EndNode[EndNode.size() - 1].x && x != shelfXnum - 1)) {

		int area_x = start_x + (shelfWidth + road) * x;
		int area_y = start_y + (shelfHeight + road) * y;

		for (int i = area_x - road; i < area_x; i++) {
			for (int j = area_y; j < area_y + shelfHeight; j++) {
				Map[i + j * mapWidth].isAccessible = 1;
			}
		}

		for (int i = area_x + shelfWidth; i < area_x + shelfWidth + road; i++) {
			for (int j = area_y; j < area_y + shelfHeight; j++) {
				Map[i + j * mapWidth].isAccessible = 1;
			}
		}
	}

	return Map;
}


//1.从货区到传送带
//Map 是除了道路全都能走，要把该货区点亮，还有把一部分路抹去
vector<MapNode> createBackMap(vector<MapNode> Map, int area)
{
	int startX = start_x, startY = start_y;

	int x = (area - 1) / shelfYnum;
	int y = (area - 1) % shelfYnum;

	//1.把该货区点亮了
	for (int i = startX + x * (shelfWidth + road); i < startX + x * (shelfWidth + road) + shelfWidth; i++) {
		for (int j = startY + y * (shelfHeight + road); j < startY + y * (shelfHeight + road) + shelfHeight; j++) {
			if (Map[i + j * mapWidth].nodeType == 2) {
				Map[i + j * mapWidth].isAccessible = 0;
			}
		}
	}

	if (x == 0 || x == shelfXnum - 1) {

	}
	else {
		//2.封杀道路,留下这个货区的下面的一条路，
		//int rowUp = startY + y * (shelfHeight + road);
		int rowBottom = startY + y * (shelfHeight + road) + shelfHeight;
		int columnLeft = startX;
		int columnRight = startX + (shelfXnum - 1) * (shelfWidth + road) + shelfWidth;

		for (int i = columnLeft - 4; i < columnRight + 4; i++) {
			for (int j = 1; j < mapHeight; j++) {
				if (j == rowBottom) {
					if (Map[i + j * mapWidth].nodeType == 1) {
						Map[i + j * mapWidth].isAccessible = 0;
					}
				}
				else {
					if (Map[i + j * mapWidth].nodeType == 1) {
						Map[i + j * mapWidth].isAccessible = 1;
					}
				}
			}
		}
	}

	return Map;
}

//2.从传送带到货区
//Map 是除了道路全都能走，要把该货区点亮，还有把一部分路抹去
vector<MapNode> createComeMap(vector<MapNode> Map, int area, int robotX, int robotY, int status, int robotId)
{
	int startX = start_x, startY = start_y;

	if (area == 0) {
		return Map;
	}

	int x = (area - 1) / shelfYnum;
	int y = (area - 1) % shelfYnum;

	//1.把该货区点亮了
	if (status == 0) {
		for (int i = startX + x * (shelfWidth + road); i < startX + x * (shelfWidth + road) + shelfWidth; i++) {
			for (int j = startY + y * (shelfHeight + road); j < startY + y * (shelfHeight + road) + shelfHeight; j++) {
				if (Map[i + j * mapWidth].nodeType == 2) {
					Map[i + j * mapWidth].isAccessible = 0;
				}
			}
		}
	}

	if (x == 0 || x == shelfXnum - 1) {

	}
	else {
		//2.封杀道路,留下这个货区的上面的一条路，
		int rowUp = startY + y * (shelfHeight + road) - 1;
		//int rowBottom = rowUp + ;
		int columnLeft = startX - 6;
		int columnRight = startX + (shelfXnum - 1) * (shelfWidth + road) + shelfWidth + 6;

		if ((robotX < columnRight && robotX >= columnLeft) && (robotY != rowUp)) {
			int a = 0;
		}
		else {
			for (int i = columnLeft; i < columnRight; i++) {
				for (int j = 1; j < mapHeight; j++) {
					if (j == rowUp || i == updateRobotInfo[robotId].attribute.robotDefaultX) {
						if (Map[i + j * mapWidth].nodeType == 1) {
							Map[i + j * mapWidth].isAccessible = 0;
						}
					}
					else {
						if (Map[i + j * mapWidth].nodeType == 1) {
							Map[i + j * mapWidth].isAccessible = 1;
						}
					}
				}
			}

		}
	}

	return Map;
}
