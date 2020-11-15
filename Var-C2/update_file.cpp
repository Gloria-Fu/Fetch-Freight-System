#include "update_file.h"
#include "public.h"
#include "goods_area.h"


int CreateMap(int width, int height, int shelfX, int shelfY, int conveyor_num)
{
	int** map;
	map = (int**)malloc(width * sizeof(int*));

	for (int i = 0; i < width; i++) {
		map[i] = (int*)malloc(height * sizeof(int));
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			map[i][j] = -1;
		}
	}

	//1.创建边框
	for (int col = 0, row = 0; col < width; col++) {
		map[col][row] = 0;
	} //上边
	for (int col = 0, row = 1; row < height; row++) {
		map[col][row] = 0;
	} //左边
	for (int col = 1, row = height - 1; col < width; col++) {
		map[col][row] = 0;
	} //下边
	for (int col = width - 1, row = 1; row < height - 1; row++) {
		map[col][row] = 0;
	} //右边


	//2.创建卸货点
	try {
		ofstream outfile;
		outfile.open("endNode.txt", ofstream::out);

		if (!outfile)
			cout << "\n shift!\n" << endl;

		int j = 0;
		if (conveyor_num == 2) {
			for (int col = 5, row = 1; row < height - 1; row++) {			//左边
				map[col][row] = 4;
				if (j != 0)
					outfile << endl;
				j++;
				outfile << j << "\t"
					<< col << "\t"
					<< row;
			}
		}

		for (int col = width - 6, row = 1; row < height - 1; row++) {
			map[col][row] = 4;
			if (j != 0)
				outfile << endl;
			j++;
			outfile << j << "\t"
				<< col << "\t"
				<< row;
		}
		outfile.close();
	}
	catch (exception & e) {
		cout << "Writing file error!\n" << endl;
	}

	//3.创建货物和货架区域位置
	try {
		ofstream outfile;
		outfile.open("goods_xy.txt", ofstream::out);

		if (!outfile)
			cout << "\n shift!\n" << endl;

		int shelf_x, shelf_y;
		int x_gap = shelfWidth + road, y_gap = shelfHeight + road;
		int goods_x, goods_y1, goods_y2;
		int k = 0;
		for (int i = 0; i < shelfX; i++) {
			for (int j = 0; j < shelfY; j++) {
				shelf_x = (start_x)+i * x_gap;
				shelf_y = (start_y)+j * y_gap;
				goods_x = shelf_x + road - 1;
				goods_y1 = shelf_y + road - 1;
				goods_y2 = goods_y1 + goodsHeight + road;

				for (int col = shelf_x; col < shelfWidth + shelf_x; col += 1) {
					for (int row = shelf_y; row < shelfHeight + shelf_y; row += 1) {
						if ((col >= goods_x && col < goods_x + goodsWidth) && ((row >= goods_y1 && row < goods_y1 + goodsHeight) || (row >= goods_y2 && row < goods_y2 + goodsHeight))) {
							map[col][row] = 3;				//货物
							k++;
							outfile << k << "\t"
								<< col << "\t"
								<< row << endl;
						}
						else {
							map[col][row] = 2;				//没货物的货架区域
						}
					}
				}
			}
		}

		outfile.close();
	}
	catch (exception & e) {
		cout << "Writing file error!\n" << endl;
	}

	//4.创建道路
	/*for (int row = 1; row < height - 1; row++) {
		for (int col = 1; col < width - 1; col++) {
			if (map[col][row] != 2 && map[col][row] != 3 && map[col][row] != 4)
				map[col][row] = 1;
		}
	}*/
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			if (map[col][row] == -1)
				map[col][row] = 1;
		}
	}


	try {
		ofstream outfile;
		outfile.open("map.txt", ofstream::out);

		if (!outfile)
			cout << "\n shift!\n" << endl;

		//将数据输出至out.txt文件中
		int t = 0, j = 0;
		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {
				if (map[col][row] == 1 || map[col][row] == 4)
					t = 0;
				else
					t = 1;
				if (row == height - 1 && col == width - 1) {
					outfile << row * width + col + 1 << "\t"
						<< col << "\t"
						<< row << "\t"
						<< map[col][row] << "\t"
						<< t;
				}
				else {
					outfile << row * width + col + 1 << "\t"
						<< col << "\t"
						<< row << "\t"
						<< map[col][row] << "\t"
						<< t << endl;
				}
			}
		}

		outfile.close();
	}
	catch (exception & e) {
		cout << "Writing file error!\n" << endl;
	}

	return 0;
}




vector<MapNode> getMapNode(void)
{
	MapNode currentNode;
	vector<MapNode> Map;

	//创建二维数组分别存储int及string数据
	int rowNum = mapWidth * mapHeight;
	int columnNum = 5;
	int Node[5] = {0};

	ifstream myfile("map.txt");

	if (!myfile.is_open())
	{
		cout << "can not open this file" << endl;
		//getchar();
	}

	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < columnNum; j++)
		{
			myfile >> Node[j];
		}
		currentNode.nodeId = Node[0];
		currentNode.x = Node[1];
		currentNode.y = Node[2];
		currentNode.nodeType = Node[3];
		currentNode.isAccessible = Node[4];
		currentNode.distance = 0;
		Map.push_back(currentNode);
	}

	myfile.close();

	return Map;
}




int readParameters(void)
{
	int rowNum = 9;
//	int columnNum = 2;
	int Node[9] = {0};

	ifstream myfile("parameter.txt"); 

	if (!myfile.is_open())
	{
		cout << "can not open this file" << endl;
		//return -1;
	}

	for (int i = 0; i < rowNum; i++)
	{
		/*for (int j = 0; j < columnNum; j++)
		{*/
			myfile >> Node[i];
	//	}
	}
	mapWidth = Node[0];
	mapHeight = Node[1];
	shelfXnum = Node[2];
	shelfYnum = Node[3];
	areaNum = Node[4];
	goodsTypeNum = Node[5];
	pickingRobotNum = Node[6];
	freightRobotNum = Node[7];
	conveyorNum = Node[8];

	if (conveyorNum == 1)
		start_x = 4;
	else
		start_x = 18;

	myfile.close();

	robotNum = freightRobotNum + pickingRobotNum;

	return 0;
}


vector<int> getTask(int goodsTypeNum)
{
	int rowNum = goodsTypeNum;
	int columnNum = 2;
	int Node[2] = { 0 };

	vector<int> taskRequirement;

	ifstream myfile("task.txt");
	if (!myfile.is_open())
	{
		cout << "can not open this file" << endl;
	}

	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < columnNum; j++)
		{
			myfile >> Node[j];
		}
		
		taskRequirement.push_back(Node[1]);
	}

	myfile.close();

	return taskRequirement;
}



int writeRobot(void)
{
	try {
		ofstream outfile;
		outfile.open("robots.txt", ofstream::out);

		Node2D node;

		if (!outfile) {
			cout << "\n shift!\n" << endl;
		}

		//将数据输出至out.txt文件中
		for (int i = 0; i < updateRobotInfo.size(); i++)
		{
			if (i == updateRobotInfo.size() - 1) {
				outfile << updateRobotInfo[i].attribute.robotId			<< "\t"
					<< updateRobotInfo[i].attribute.robotCurrentX		<< "\t"
					<< updateRobotInfo[i].attribute.robotCurrentY		<< "\t"
					<< updateRobotInfo[i].attribute.robotType			<< "\t"
					<< updateRobotInfo[i].attribute.goodsType;
			}
			else {
				outfile << updateRobotInfo[i].attribute.robotId			<< "\t"
					<< updateRobotInfo[i].attribute.robotCurrentX		<< "\t"
					<< updateRobotInfo[i].attribute.robotCurrentY		<< "\t"
					<< updateRobotInfo[i].attribute.robotType			<< "\t"
					<< updateRobotInfo[i].attribute.goodsType			<<endl;
			}
		}


		outfile.close();
	}
	catch (exception &e) {
		cout << "Writing file error!\n" << endl;
	}

	return 0;
}


int writeRobot2(void)
{
	try {
		ofstream outfile;
		outfile.open("robots2.txt", ofstream::out);

		Node2D node;

		if (!outfile) {
			cout << "\n shift!\n" << endl;
		}

		//将数据输出至out.txt文件中
		for (int i = 0; i < updateRobotInfo.size(); i++)
		{
			if (i == updateRobotInfo.size() - 1) {
				outfile << updateRobotInfo[i].attribute.robotId	<< "\t"
					<< updateRobotInfo[i].attribute.robotCurrentX << "\t"
					<< updateRobotInfo[i].attribute.robotCurrentY << "\t"
					<< updateRobotInfo[i].attribute.robotType		<< "\t"
					<< updateRobotInfo[i].attribute.goodsType;
			}
			else {
				outfile << updateRobotInfo[i].attribute.robotId		<< "\t"
					<< updateRobotInfo[i].attribute.robotCurrentX		<< "\t"
					<< updateRobotInfo[i].attribute.robotCurrentY		<< "\t"
					<< updateRobotInfo[i].attribute.robotType			<< "\t"
					<< updateRobotInfo[i].attribute.goodsType			<< endl;
			}
		}


		outfile.close();
	}
	catch (exception & e) {
		cout << "Writing file error!\n" << endl;
	}

	return 0;
}


int getRobotDefault(void)
{
	int columnNum = 5;
	int Node[5] = { 0 };

	ifstream myfile("robots2.txt");

	if (!myfile.is_open())
	{
		cout << "can not open this file" << endl;
		return -1;
	}

	for (int i = 0; i < robotNum; i++)
	{
		for (int j = 0; j < columnNum; j++)
		{
			myfile >> Node[j];
		}

		updateRobotInfo[i].attribute.robotDefaultX = Node[1];
		updateRobotInfo[i].attribute.robotDefaultY = Node[2];
		updateRobotInfo[i].attribute.taskX = Node[1];
		updateRobotInfo[i].attribute.taskY = Node[2];
	}

	myfile.close();

	return 0;
}


//读取本轮货物信息
int getGoodsNode2(void)
{
	GoodsInfo newGoods;

	//创建二维数组分别存储int及string数据
	int rowNum = eachGoodsNum * areaNum;
	int columnNum = 6;
	int Node[6] = { 0 };

	ifstream myfile("goods.txt");

	if (!myfile.is_open())
	{
		cout << "can not open this file" << endl;
		return -1;
	}

	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < columnNum; j++)
		{
			myfile >> Node[j];
		}

		newGoods.goods_Id = Node[0];
		newGoods.goods_X = Node[1];
		newGoods.goods_Y = Node[2];
		newGoods.goods_Num = Node[3];
		newGoods.goods_Type = Node[4];
		newGoods.goods_status = Node[5];

		GoodsInformation[newGoods.goods_Type - 1].push_back(newGoods);
		GoodsContents.push_back(newGoods);
	}

	myfile.close();

	return 0;
}


int getEndNode(void)
{
	MapNode currentNode;
	int columnNum = 3;
	int Node[3] = { 0 };

	ifstream myfile("endNode.txt");

	if (!myfile.is_open()){
		cout << "can not open this file" << endl;
		return -1;
	}

	while (!myfile.eof()) {
		for (int j = 0; j < columnNum; j++){
			myfile >> Node[j];
		}
		currentNode.nodeId = Node[0];
		currentNode.x = Node[1];
		currentNode.y = Node[2];
		currentNode.nodeType = 4;
		currentNode.isAccessible = 0;
		EndNode.push_back(currentNode);
	}

	myfile.close();

	return 0;
}


//将每轮运行时间输出
int OutputResult(int param, int index, char* s3)
{
	try {
		ofstream outfile;
		outfile.open(s3, ofstream::out);

		if (!outfile) {
			cout << "\n shift!\n" << endl;
		}

		//将数据输出至out.txt文件中
		if (param == 0) {
			for (int i = 0; i < taskProcess.size(); i++) {
				outfile << taskProcess[i].finishPeriod << endl;
			}
			outfile << globalPeriod << endl;
		}
		else if (param == -1) {
			outfile << "-1" << endl;
		}
		else if (param == -2) {
			outfile << "-2" << endl;
		}

		outfile.close();
	}
	catch (exception & e) {
		cout << "a" << endl;
	}

	return 0;
}

//读取每一轮新的货物信息和任务信息
int ReadGoodsAndTasks(int index, char* s1, char* s2)
{
	try {
		ifstream infile(s1);
		ofstream outfile("goods.txt", ofstream::out);

		char c;
		while (infile.get(c)) {
			outfile << c;
		}

		infile.close();
		outfile.close();
	}
	catch (exception & e) {
		cout << "a" << endl;
	}

	try {
		cout << s2 << endl;
		ifstream infile(s2);
		ofstream outfile("task.txt", ofstream::out);

		char c;
		while (infile.get(c)) {
			outfile << c;
		}

		infile.close();
		outfile.close();
	}
	catch (exception & e) {
		cout << "a" << endl;
	}


	return 0;
}

