#include "path_planning.h"
#include "conflict_resolution.h"
#include "calculate.h"


//1to2:�����״̬1���״̬2����������ѷ��䵽��������˵��ѷ��䵽�˻������ˣ���ҪΪ�˻������˹滮ȥȡ����·��
int PlanPath1to2(int param, vector<MapNode> Map, int robotId)
{
	if (param == 1) {
		PlanToGoodsArea(robotId, Map);							//1.����·���滮
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
	else if (param == 3) {										//3.���е�
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

//1to2:�����״̬3���״̬4���������Ѿ������������˽������ˣ���ҪΪ�˻������˹滮ȥж����·��
int PlanPath3to4(int param, vector<MapNode> Map, int robotId)
{
	if (param == 1) {
		LeaveGoodsArea(Map, robotId);							//1.����·���滮
	}
	else if (param == 2) {										//2.A*
		int target = ChooseEndPoint(robotId);					//��ȡ������յ�
		FreightReturn(robotId, target, Map);					//�滮����·��
	}
	else if (param == 3) {										//3.���е�
		int target = ChooseEndPoint(robotId);					//��ȡ������յ�
		FreightReturn1(robotId, target, Map);					//�滮����·��
	}

	return 0;
}

//4to5:�����״̬4���״̬5���������Ѿ������ж���ˣ�Ϊ�˻������˹滮��Ĭ��λ�õ�·��
int PlanPath4to5(int param, vector<MapNode> Map, int robotId)
{
	if (param == 1) {
		PlanToGoodsArea(robotId, Map);							//1.����·���滮
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
	else if (param == 3) {										//3.���е�
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
			updateRobotInfo[robotId].robot_map = CreateFreightRobotMap(updateRobotInfo[robotId].attribute.area, Map);							//�����û����˵�ͼ
		}
		else if (updateRobotInfo[robotId].attribute.robotStatus == 3) {
			updateRobotInfo[robotId].robot_map = Map;
		}

		updateRobotInfo[robotId].robot_path.clear();

		bool isSuccess = Astar(updateRobotInfo[robotId].robot_map, mapWidth, mapHeight, updateRobotInfo[robotId].attribute.robotCurrentX,	//�滮�����˴ӵ�ǰλ�õ���һ���ڵ��·��
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
			updateRobotInfo[robotId].robot_map = CreateFreightRobotMap(updateRobotInfo[robotId].attribute.area, Map);							//�����û����˵�ͼ

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




//�˻������˹滮���ǰ������,�ȼ����������ģ�ĳ��·����ж��������Ȼ��ѡ���յ㣬�յ���Ŀ��������ĸ���
//�����ǽӵ������Ĺ滮����˿�������Ŀ��������ƽ����Ĭ��λ�ò�һ���������й���״̬�����
vector<Node2D> PathPlanningOnTopology1(int robotId, vector<MapNode> Map)
{
	vector<Node2D> temptopologyPath;

	int areaId = updateRobotInfo[robotId].attribute.area;
	int robotX = updateRobotInfo[robotId].attribute.robotCurrentX;
	int robotY = updateRobotInfo[robotId].attribute.robotCurrentY;

	vector<Node2D> startPoints = OnWhichRoad(robotX, robotY, CreatPoints());

	if (startPoints.size() == 0) {				//�������������ڣ�����0
		return temptopologyPath;
	}

	vector<Node2D> endPoints;
	Node2D node;

	//������ʱ�ǲ��ܻ�������·�ϻ�����ж��������ѡ��û��������������ʮ��·�ڵ���Ϊ��ʼ�㣬Ŀ��������ĸ�����Ϊ�յ�����ϲ�·���滮
	int area_x, area_y;

	if (areaId > 0) {
		area_x = (areaId - 1) / shelfYnum;
		area_y = (areaId - 1) % shelfYnum;
	}

	if (updateRobotInfo[robotId].attribute.robotStatus == 1 && areaId != 0) {					//1.����������յ�������Ҫ�ӵ�ǰλ�����¹滮���յ�Ϊ�����ĸ��Ƕ���
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
	else if (updateRobotInfo[robotId].attribute.robotStatus == 0 && areaId != 0) {				//2.������ж���Ҫ��ȥĬ��λ�ã��յ�Ϊ�������������㣬����֮���پ���滮
		node.x = area_x;
		node.y = area_y;
		endPoints.push_back(node);
		node.y += 1;
		endPoints.push_back(node);
	}
	else if (updateRobotInfo[robotId].attribute.robotStatus == 0 && areaId == 0) {				//3.����������û�������˵����Ҫͨ����Ĭ��λ���ж�Ҫȥ��λ��
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
	for (int j = 0; j < startPoints.size(); j++) {										//����ÿ������ÿ���յ��·��
		for (int i = 0; i < endPoints.size(); i++) {
			bool isSuccess = Astar2(shelfXnum + 1, shelfYnum + 1, startPoints[j].x, startPoints[j].y, endPoints[i].x, endPoints[i].y, topologyPath[k]);	//��������еĿ���·��
			k++;
			if (isSuccess == false)
				int a = 0;
		}
	}



	int minPathSize = 1000;
	vector<int> minPathIndex;
	vector<Node2D> minStartNodes, minEndNodes;
	for (int i = 0; i < topologyPath.size(); i++) {										//ѡ����̵�·����
		if (topologyPath[i].size() <= minPathSize) {
			minPathSize = topologyPath[i].size();
		}
	}
	for (int i = 0; i < topologyPath.size(); i++) {
		if (topologyPath[i].size() == minPathSize) {
			minPathIndex.push_back(i);
			minStartNodes.push_back(topologyPath[i][topologyPath[i].size() - 1]);		//ѡ����̵�·����������
			minEndNodes.push_back(topologyPath[i][0]);									//ѡ����̵�·��������յ�
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

	temptopologyPath = topologyPath[minPathIndex[nodeIndex]];			//��ʱʹ������·��

	return temptopologyPath;
}



//�˻��������뿪�����Ĳ��� 1�����ݵ�ǰ��ͨ�������Ҫ������ж������·��	2��ѡ������һ��Ҫ�����ĵ�·�ڵ�����Һ���ĵ��뿪���� 3��·���滮
int LeaveGoodsArea(vector<MapNode> Map, int robotId)
{
	updateRobotInfo[robotId].highLevelPath = PathPlanningOnTopology(robotId, updateRobotInfo[robotId].attribute.area, CreatPoints());

	//tempPath������ͼ�ڵ��·������ο��ƻ���������Щ·���أ�

	if (updateRobotInfo[robotId].highLevelPath.size() > 1) {
		Node2D firstNode;												//��������ֵ��ͼ�滮���ϲ�·����Ҫ�����ĵ�һ���ڵ������λ�ã�ʵ���ϲ�����һ��Ҫ������㣩
		firstNode.x = updateRobotInfo[robotId].highLevelPath[updateRobotInfo[robotId].highLevelPath.size() - 1].x;
		firstNode.y = updateRobotInfo[robotId].highLevelPath[updateRobotInfo[robotId].highLevelPath.size() - 1].y;

		//������ѡ��Ҫ�뿪�ĵ㣨��������ڻ������򿿽���Ե��һ���㣩
		//1.�����ж��Լ���������ֱ��A*�᲻����ֺܶ����Ƶ�λ��Ȼ��ײ��
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
	//�ɴ����·��һ����ֱ�Ӵ���ߵ�ж������˵
	else if (updateRobotInfo[robotId].highLevelPath.size() == 1) {		//Ӧ��ֻ�п������ʹ���һ�еĻ�����high-level����Ϊ0�ɣ�ֱ��ѡ���ðɣ��Ͼ���û��ȥ�Ǹ����򣬲������鰡
		//����������ж�����Ϊ�������ʹ�������Ҫ�ߵ��ڵ�
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


////////////////ж�����Ĳ���	��Ҫ��ѡ�������
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


//��������ͼ�����ӽڵ�(�����ʾ����ͼ�е����꣬.x.y��ʾ����ʵ��ͼ�е�����)
vector<vector<Node2D>> CreatPoints(void)
{
	vector<vector<Node2D>> Points;					//��ά�����ʾ��Χ������������ĵ�ͼ�ڵ�

	for (int i = 0; i < shelfXnum + 1; i++) {
		Points.push_back(vector<Node2D>());
	}

	int startX = start_x - road;
	int startY = start_y - road;

	Node2D pp;

	for (int row = 0; row < shelfYnum + 1; row++) {					//�����ң����ϵ���
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


//������������ͼ�ڵ�֮��ĵ�·������������������������������ͼ���꣬����ͼ�㣬Ҫ���������ͼ���Ӧ����ʵ���귶Χ�ڵĻ�����������
float CalHeatValueBetweenTwoNode(NodeInfo node1, NodeInfo node2, vector<vector<Node2D>> Points)
{
	float heatValue = 0;

	int leftStartX, leftStartY, rightEndX, rightEndY;				//����ÿ�μ���ľ��η�Χ·�ε����Ͻ���������½�����
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

	for (int i = 0; i < updateRobotInfo.size(); i++) {				//����ÿ�������˵�λ�ã�
		if (updateRobotInfo[i].attribute.robotCurrentX >= leftStartX && updateRobotInfo[i].attribute.robotCurrentX <= rightEndX				//��ʾ����������������·������
			&& updateRobotInfo[i].attribute.robotCurrentY >= leftStartY && updateRobotInfo[i].attribute.robotCurrentY <= rightEndY) {
			robotNums++;
		}
	}

	//������ֵӦ�ò��ܳ���1������̫�󣬲�Ȼ�ڹ滮��ʱ��һ���ڵ㵽��һ���ڵ����ֻ��1��Ҫ�ǳ���1��������ζ��Ҫ��������ô����·��
	//������������һ�������ˣ�������Ҫ������������������Ҫ�ߵľ�����road+shelfWidth���Ǿ�robotNum*2/(shelfWidth)��������������
	heatValue = float(robotNums) / float(road + shelfWidth);			//����ֵ�ļ��㷽ʽ

	return heatValue;
}


//���㴫�����Ļ����˵�����Ŀǰ���������ϣ���⣬Ҫ����ж����զ���֣�
//������ǻ����˵���ʵ���꣬�Լ�����ͼ�㣬Ҫȡ����ͼ���Ӧ����ʵ������м���û����˵�λ��
vector<Node2D> OnWhichRoad(int x, int y, vector<vector<Node2D>> Points)
{
	vector<Node2D> nearNodes;
	Node2D node;

	int a = 0;

	static int b = 0;
	b++;
	if (b == 10)
		int c = 0;

	if (x > (shelfWidth + road) * shelfXnum + start_x - road) {						//��������˵�λ����ж������Ļ������ַ�������1��ж�����������е㶼������ 
		for (int i = 0; i < shelfYnum; i++) {										//��2����ʱ������������˵�������
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
	else if (x < start_x) {						//��������˵�λ����ж������Ļ������ַ�������1��ж�����������е㶼������ 
		for (int i = 0; i < shelfYnum; i++) {										//��2����ʱ������������˵�������
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

			if (x >= node1.x - 1 && x <= node1.x + 1 && y >= node1.y - 1 && y <= node1.y + 1) {				//�ڽڵ㸽������ֻ��һ�����
				node.x = i / (shelfYnum + 1);
				node.y = i % (shelfYnum + 1);
				nearNodes.push_back(node);
				break;
			}
			else {																							//�����ڵ��м䣬���������
				if (i % (shelfYnum + 1) > 0) {								//��
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

				if (i % (shelfYnum + 1) < shelfYnum) {						//��
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

				if (i / (shelfYnum + 1) > 0) {								//��
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

				if (i / (shelfYnum + 1) < shelfXnum) {						//��
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


//���ڸ����ڻ�����ɽ��Ӻ󣬸��ݽ�ͨ����ͼ�ƻ��س�ʱ��������Ե�����ͼ·�������Ǻ��п������ߵĹ�������Ҫ����������̣�δȷ����
//������������ţ��Լ�����ͼ�㣬����ͼ������ʵ�������ſ���ת�������Ǹ�����ֵ������Ҫ������ͼ�еĵ���м�ȨA*�滮���ϲ�·���滮���õ����Ǽƻ�Ҫ������·�Σ�
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
	startPoints.push_back(node);										//�˻�����������������ĸ�����ڵ㶼�л��ᵱ�����ڵ�
	node.y += 1;
	startPoints.push_back(node);
	node.x += 1;
	startPoints.push_back(node);
	node.y -= 1;
	startPoints.push_back(node);

	if (start_x != 4) {
		for (int i = 0; i < shelfYnum + 1; i++) {
			node.x = 0;												//�������ʹ���һ���нڵ㶼�л��ᵱ���յ�ڵ�
			node.y = i;
			endPoints.push_back(node);
		}
	}
	for (int i = 0; i < shelfYnum + 1; i++) {
		node.x = shelfXnum;												//�������ʹ���һ���нڵ㶼�л��ᵱ���յ�ڵ�
		node.y = i;
		endPoints.push_back(node);
	}



	vector<vector<Node2D>> topologyPath;

	for (int i = 0; i < startPoints.size() * endPoints.size(); i++)
		topologyPath.push_back(vector<Node2D>());

	int k = 0;
	for (int j = 0; j < startPoints.size(); j++) {
		for (int i = 0; i < endPoints.size(); i++) {
			bool isSuccess = Astar2(shelfXnum + 1, shelfYnum + 1, startPoints[j].x, startPoints[j].y, endPoints[i].x, endPoints[i].y, topologyPath[k]);	//��������еĿ���·��
			if (isSuccess == false) {
				int a = 0;
			}
			k++;
		}
	}

	int minPathSize = 1000;
	vector<int> minPathIndex;
	vector<Node2D> minStartNodes;
	for (int i = 0; i < topologyPath.size(); i++) {									//ѡ����̵�·����
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

	//1.�����Ҫ��������̵�·���Ƿֱ�Ľ�ͨ����ֵ
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

	//2.������������ĸ�������
	int minDistance = 1000;
	int nodeIndex = -1;
	for (int i = 0; i < minStartNodes.size(); i++) {								//����̵�·������ѡ�����ͻ��������������·
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

	//temptopologyPath = topologyPath[minPathIndex[minIndex]];						//��ʱʹ������·��

	return temptopologyPath;
}


//�������ڵ���ĳ��high-level�滮�Ľڵ�󣬲�ѯ��һ���ڵ㣬�������˹滮�ӵ�ǰ�ڵ㵽��һ���ڵ�ľ���·��
//�������Ǹ��ֲ�Ԥ�滮����ʱ��̬������ʱ��Ҫ��֤�����˵�·���ӵ�ǰλ�õ��������һ���ڵ��λ�á�
//���������滮�굱ǰ�ڵ㵽��һ���ڵ��·��ʱ���ɿ���ɾ����ǰ�ڵ㣬����ɾ�˾�ʧȥ�����ˣ�emmmm�������
//һ����ǻ�������һ��·����֮��+����Ƿ�������ڵ��λ�ã��Ϳ��Կ�ʼ��һ��·��·���滮��
int ReachNewNode(int robotId, vector<MapNode> Map)
{
	int robotX = updateRobotInfo[robotId].attribute.robotCurrentX;
	int robotY = updateRobotInfo[robotId].attribute.robotCurrentY;

	int flag = 0;

	Node2D node1, node2;
	node1.x = updateRobotInfo[robotId].highLevelPath[updateRobotInfo[robotId].highLevelPath.size() - 1].x * (shelfWidth + road) + start_x - 2;
	node1.y = updateRobotInfo[robotId].highLevelPath[updateRobotInfo[robotId].highLevelPath.size() - 1].y * (shelfHeight + road) + start_y - 2;

	if (robotX >= node1.x - 2 && robotX <= node1.x + 2 && robotY >= node1.y - 2 && robotY <= node1.y + 2) {	//֤��������������ڵ�İ�����Χ��,��������ĵ�����һ���ڵ�

		if (updateRobotInfo[robotId].highLevelPath.size() > 1) {											//1.��high-level·���л�δ����high-level��ͼ·��
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

			if (node1.y == node2.y) {								//ˮƽ����
				int step = node2.x - robotX;
				int direction = 0;
				if (step > 0)										//������
					direction = -1;
				else if (step < 0)									//������
					direction = 1;
				else 												//��ԭ��
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
			else if (node1.x == node2.x) {							//��ֱ����
				int step = node2.y - robotY;
				int direction = 0;
				if (step > 0) 										//������
					direction = -1;
				else if (step < 0)									//������
					direction = 1;
				else 												//��ԭ��
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
		else if (updateRobotInfo[robotId].highLevelPath.size() == 1) {								//2.�ж���Ҫȥȡ������ж��

			if (updateRobotInfo[robotId].robot_path.size() == 1) {
				updateRobotInfo[robotId].robot_path.pop_back();
			}

			if (updateRobotInfo[robotId].attribute.robotStatus == 1) {								//(1)ȥȡ�����Ѿ���Ŀ����������ܣ�ֱ�Ӵӵ�ǰλ�ý�ȥĿ��λ�ð�
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
			else if (updateRobotInfo[robotId].attribute.robotStatus == 3) {							//(2)ȥж�����Ҹ��㣬�滮��ȥ
				UnloadGoods(robotId);
				//	int target = ChooseEndPoint00(robotId);
				//	FreightReturn00(robotId, target, Map);

			}
			else if (updateRobotInfo[robotId].attribute.robotStatus == 0) {							//(3)�ػ����Ե�Ĭ��λ��
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


//����OpenList��Gֵ��С�ĵ㣬��������OpenList��ȥ������Ϊ��ǰ�����ڵ�
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

//A*��̬��ͼ·���滮�㷨�����ڼ���ʵ�ʵ�ͼ��A*�滮
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
	short* isVisited = new short[height * width];		//��ʾ�ڵ�״̬��0:δ���ʣ�1:open 2:close

	for (int i = 0; i < height * width; i++) {
		isVisited[i] = 0;
	}

	NodeInfo* nodes = new NodeInfo[width * height];

	NodeInfo startNode, endNode;
	startNode.x = startX;
	startNode.y = startY;
	startNode.g = 0;
	startNode.h = abs(startNode.x - endX) + abs(startNode.y - endY);	//�����پ��룬��ʼ�㵽Ŀ���ľ��Ծ���
	startNode.f = startNode.g + startNode.h;
	startNode.index = startY * width + startX;
	startNode.preIndex = -1;											//��ʼ��û�и��ڵ㣬��Ϊ-1

	endNode.x = endX;
	endNode.y = endY;

	NodeInfo PredNode;				//��ǰ�ڵ�
	NodeInfo SuccNode;				//�ӽڵ�
	PathNode pathNode;				//·���ڵ�

	int PredId, SuccId;				//�ڵ�id

	vector<NodeInfo> OpenList;
	vector<NodeInfo> CloseList;
	OpenList.push_back(startNode);
	isVisited[startNode.index] = 1;
	nodes[startNode.index] = startNode;

	while (!OpenList.empty()) {
		PredNode = popSmallestNode(OpenList);
		PredId = PredNode.y * width + PredNode.x;
		//���ѵ����յ�
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
		//��δ�ҵ��յ�
		isVisited[PredId] = 2;								//�ѵ�ǰ����뵽�ռ�����
		CloseList.push_back(nodes[PredId]);

		try {
			for (int i = 0; i < 4; i++) {
				int succX = PredNode.x + directions[i][0];
				int succY = PredNode.y + directions[i][1];
				if (succX >= 0 && succX < width && succY >= 0 && succY < height && (Map[succX + succY * width].isAccessible != 1 && isVisited[succY * width + succX] != 2)) {
					//�߽���+�ϰ�����+�Ƿ���Close����
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



//A*��̬��ͼ·���滮�㷨,���ڼ�������ͼ��A*�滮
bool Astar2(int width, int height, int startX, int startY, int endX, int endY, vector<Node2D>& path)
{
	if (startX < 0 || startX >= width || startY < 0 || startY >= height || endX < 0 || endX >= width || endY < 0 || endY >= height) {
		return false;
	}

	int directions[4][2] = { {0,-1},{-1,0},{1,0},{0,1} };
	short* isVisited = new short[height * width];		//��ʾ�ڵ�״̬��0:δ���ʣ�1:open 2:close

	for (int i = 0; i < height * width; i++) {
		isVisited[i] = 0;
	}

	NodeInfo* nodes = new NodeInfo[width * height];

	NodeInfo startNode, endNode;
	startNode.x = startX;
	startNode.y = startY;
	startNode.g = 0;
	startNode.h = abs(startNode.x - endX) + abs(startNode.y - endY);	//�����پ��룬��ʼ�㵽Ŀ���ľ��Ծ���
	startNode.f = startNode.g + startNode.h;
	startNode.index = startY * width + startX;
	startNode.preIndex = -1;											//��ʼ��û�и��ڵ㣬��Ϊ-1

	endNode.x = endX;
	endNode.y = endY;

	NodeInfo PredNode;				//��ǰ�ڵ�
	NodeInfo SuccNode;				//�ӽڵ�
	Node2D pathNode;				//·���ڵ�

	int PredId, SuccId;				//�ڵ�id
	float extraCost = 0;

	vector<NodeInfo> OpenList;
	vector<NodeInfo> CloseList;
	OpenList.push_back(startNode);
	isVisited[startNode.index] = 1;
	nodes[startNode.index] = startNode;

	while (!OpenList.empty()) {
		PredNode = popSmallestNode(OpenList);
		PredId = PredNode.y * width + PredNode.x;
		//���ѵ����յ�
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
		//��δ�ҵ��յ�
		isVisited[PredId] = 2;								//�ѵ�ǰ����뵽�ռ�����
		CloseList.push_back(nodes[PredId]);

		try {
			for (int i = 0; i < 4; i++) {
				int succX = PredNode.x + directions[i][0];
				int succY = PredNode.y + directions[i][1];
				if (succX >= 0 && succX < width && succY >= 0 && succY < height) {
					//�߽���+�ϰ�����+�Ƿ���Close����
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

	//1.�Ѹû���������
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


//1.�ӻ��������ʹ�
//Map �ǳ��˵�·ȫ�����ߣ�Ҫ�Ѹû������������а�һ����·Ĩȥ
vector<MapNode> createBackMap(vector<MapNode> Map, int area)
{
	int startX = start_x, startY = start_y;

	int x = (area - 1) / shelfYnum;
	int y = (area - 1) % shelfYnum;

	//1.�Ѹû���������
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
		//2.��ɱ��·,������������������һ��·��
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

//2.�Ӵ��ʹ�������
//Map �ǳ��˵�·ȫ�����ߣ�Ҫ�Ѹû������������а�һ����·Ĩȥ
vector<MapNode> createComeMap(vector<MapNode> Map, int area, int robotX, int robotY, int status, int robotId)
{
	int startX = start_x, startY = start_y;

	if (area == 0) {
		return Map;
	}

	int x = (area - 1) / shelfYnum;
	int y = (area - 1) % shelfYnum;

	//1.�Ѹû���������
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
		//2.��ɱ��·,������������������һ��·��
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
