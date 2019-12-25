#include <bits/stdc++.h>
using namespace std;
const int inf = 1e9;

struct Bus_point { // 公交车站点信息
	int sta_num;
	string sta_name; // 站点名称
	vector<string> bus_num; // 公交车号
};

struct Road_line {
	struct Bus_point Left, Right; // 一条线路的左右段的站点
	int cost; // 这条路线的长度
	int road_num; // 路线编号
};

struct Bus_line { // 路线信息
	string sta_name; // 经过站点名称
	int sta_num; // 站点号码
};

struct node{
	int u,step;
	node(){}
	node(int a,int sp)
	{
		u = a; step = sp;
	}
	bool operator < (const node& a)const{
		return step > a.step;
	}
};

struct Short_trasfer { // 最短换乘需要存储的信息
	int bp; // 当前所在的站点是什么
	vector <int> bus_proccess; // 到达这个站点需要做什么车以及之前坐的车
	vector <int> bus_line;
	int trasfer; // 到达这个站点需要换乘几次
};

vector <Bus_point> bus_point; // 存储公交车站信息
vector <Bus_line> bus_line[10]; // 存储公交线路
vector <Road_line> road_line; // 存储路段信息
map <int, string> name_hash; // 存储下标与公交车线路的关系
queue <Short_trasfer> q; // 存储最短换乘临时容器
int mp[100][100]; // 地图
int dist[100], flag[100], pre[100]; // 从起点到任意点的最小距离, 是否到过该点, 走过点的前缀
int num_of_sta, line_of_bus; // 站点个数, 公交线路个数
int shortest;


void input_file() { // 写入文件操作
	FILE *fp;//文件指针 
	
	
	/***************车站信息***************/
	fp = fopen("/Users/gongzhaorui/Desktop/trafic_information/bus_sta.txt","w+"); // 以重写车站信息的文件
	if(fp != NULL) {
		for(int i = 0;i < bus_point.size(); i++) {
			fprintf(fp, "%s  ", bus_point[i].sta_name.c_str());
			for(int j = 0;j < bus_point[i].bus_num.size(); j++) {
				if(j == bus_point[i].bus_num.size()-1) fprintf(fp, "%s\n", bus_point[i].bus_num[j].c_str());
				else fprintf(fp, "%s ", bus_point[i].bus_num[j].c_str());
			}
		}
	}
	else printf("车站信息文件打开失败！\n");
	fclose(fp);
	/***************车站信息***************/
	
	
	
	/***************公交线路信息***************/
	fp = fopen("/Users/gongzhaorui/Desktop/trafic_information/bus_line.txt","w+");
	if(fp != NULL) {
		for(int i = 1;i <= line_of_bus; i++) {
			fprintf(fp, "%s ", name_hash[i].c_str());
			for(int j = 0;j < bus_line[i].size(); j++) {
				if(j == bus_line[i].size()-1) fprintf(fp, "%s\n", bus_line[i][j].sta_name.c_str());
				else fprintf(fp, "%s ", bus_line[i][j].sta_name.c_str());
			}
		}
	}
	else printf("公交线路信息文件打开失败！\n");
	fclose(fp);
	/***************公交线路信息***************/
	
	
	
	/***************道路信息***************/
	fp = fopen("/Users/gongzhaorui/Desktop/trafic_information/road.txt","w+");
	if(fp != NULL) {
		for(int i = 0;i < road_line.size(); i++) {
			fprintf(fp, "%d %d %d\n", road_line[i].Left.sta_num, road_line[i].Right.sta_num, road_line[i].cost);
		}
	}
	else printf("道路信息文件打开失败！\n");
	fclose(fp);
	/***************道路信息***************/
	printf("公交系统信息更新成功!\n");
}

int check_point_exist(string s) { // 检查站点是否存在
	for(int i = 0;i < bus_point.size(); i++) {
		if(s == bus_point[i].sta_name) return 1; // 如果传入的与存在的的相符合,证明该站点存在
	}
	return 0;
}

void build_sta() { // 建站
	struct Bus_point bp;
	FILE *fp;
	char s[30];
	fp = fopen("/Users/gongzhaorui/Desktop/trafic_information/bus_sta.txt", "r");
	bool t = 0;
	if(fp != NULL) {
		while(!feof(fp)) { // 直到读到文件末尾
			fscanf(fp, "%s", s); // 字符串读取
			if(strlen(s) < 4) { // 字符串长度小于4的按照公交车线路名称处理
				bp.bus_num.push_back(s);
			} 
			else { // 否则按照站点名称处理
				num_of_sta++; // 站点总数++
				if(t) bus_point.push_back(bp); 
				t=1;
				bp.bus_num.clear(); // 清空结构体进行下一次的存储
				bp.sta_name = s;
			}
		}
		bus_point.push_back(bp); // 特殊处理最后一次站点加入
	}
	else {
		printf("文件打开失败!\n");
	}
	
	for(int i = 0; i < bus_point.size(); i++) {
		bus_point[i].sta_num = i; // 给每个站点编号，方便进行下一步操作
	}
	
	// 输出测试
//	for(int i = 0; i < bus_point.size(); i++) {
//		cout << bus_point[i].sta_num << " " << bus_point[i].sta_name<< " ";
//		for(int j = 0;j < bus_point[i].bus_num.size(); j++) {
//			cout << bus_point[i].bus_num[j] << " ";
//		}
//		puts("");
//	}
}


void build_line() { // 建公交线路
	struct Bus_line bl; // 定义存储道路信息的结构体
	char s[30]; 
	FILE *fp;
	fp = fopen("/Users/gongzhaorui/Desktop/trafic_information/bus_line.txt", "r");
	if(fp != NULL) {
		while(!feof(fp)) {
			fscanf(fp, "%s", s);
//			puts(s);
			if(strlen(s) < 4) { // 如果长度小于4 按照线路名称处理
				line_of_bus++; // 线路名称会放在各个站点最前面 所以遇到后需要将总线路++
				name_hash[line_of_bus] = s;
			}
			else { // 否则就是站点名称
				bl.sta_name = s;
				for(int i = 0;i < bus_point.size(); i++) { // 寻找该站点的编号
					if(bus_point[i].sta_name == s) { 
						bl.sta_num = bus_point[i].sta_num; 
						break;
					}
				}
				bus_line[line_of_bus].push_back(bl); // 将这个线路加入到链表中
			}
		}
		
// 		测试输出
//		for(int i = 1;i <= line_of_bus; i++) {
//			for(int j = 0;j < bus_line[i].size(); j++) {
//				cout  << bus_line[i][j].sta_num << " " << bus_line[i][j].sta_name << endl;
//			}
//			puts("");
//		}
	}
	else {
		printf("文件打开失败!\n");
	}
}

void init_map() { // 初始化图
	for(int i = 0;i < 100; i++) { // 初始化将每个点之间的路初始化为无穷大
		for(int j = 0;j < 100; j++) {
			mp[i][j] = inf;
		}
	}
	for(int i = 0;i < road_line.size(); i++) { // 从道路信息中将站点编号和道路长度提取出来，加入数组中
		mp[road_line[i].Left.sta_num][road_line[i].Right.sta_num] = road_line[i].cost;
		mp[road_line[i].Right.sta_num][road_line[i].Left.sta_num] = road_line[i].cost;
		
//		printf("%d -> %d = %d\n", road_line[i].Left.sta_num,road_line[i].Right.sta_num,road_line[i].cost);
	}

}


void build_map() { // 建图
	struct Bus_point bp;
	struct Road_line rl;
	FILE *fp;
	int a, b, c;
	
	// 建立线路关系
	fp = fopen("/Users/gongzhaorui/Desktop/trafic_information/road.txt", "r");
	if(fp != NULL) {
		while(!feof(fp)) {
			fscanf(fp, "%d %d %d", &a, &b, &c);
			rl.Left = bus_point[a]; // 该条路左边的站点
			rl.Right = bus_point[b]; // 该条路右边的站点
			rl.cost = c; // 该条路的长度
			road_line.push_back(rl); // 将路段信息加入链表
		}
	}
	else {
		printf("文件打开失败!\n");
	}
		
//		测试输出
//		for(int i = 0;i < road_line.size(); i++) {
//			printf("%d %d %d\n", road_line[i].Left.sta_num, road_line[i].Right.sta_num, road_line[i].cost);
//		}
	for(int i = 0;i < road_line.size(); i++) { // 将每条路段编号
			road_line[i].road_num = i;
//			cout << i << endl;
	}	
	init_map();	// 建图
}


void dijkstra(int st) // 最短路算法
{
	priority_queue <node> Q;
	Q.push(node(st,0));
	memset(flag, 0, sizeof(flag));
	for(int i = 0;i < num_of_sta; i++) {
		dist[i] = inf;
		if(dist[i] == inf) pre[i] = -1;
		else pre[i] = st;
	}
		
	dist[st] = 0;
	while(!Q.empty())
	{
		node it = Q.top();
		Q.pop();
		int t = it.u;
		if(flag[t])
			continue;
		flag[t] = 1;
		for(int i = 0;i < num_of_sta; i++)
		{
			if(!flag[i] && mp[t][i] < inf)
			{
				if(dist[i] > dist[t] + mp[t][i])
				{
					dist[i] = dist[t] + mp[t][i];
					Q.push(node(i,dist[i]));
					pre[i] = t;
				}
			}
		}
	}
}

int find_bus_line(int fir, int sec) { // 最短路中为了查询两站之间坐什么车 返回道路序号
	for(int i = 1;i <= line_of_bus; i++) { // 枚举线路
		for(int j = 0;j < bus_line[i].size()-1; j++) { // 该条线路上的所有站点
			if((bus_point[fir].sta_name == bus_line[i][j].sta_name && bus_point[sec].sta_name == bus_line[i][j+1].sta_name) || (bus_point[sec].sta_name == bus_line[i][j].sta_name && bus_point[fir].sta_name == bus_line[i][j+1].sta_name)) // fir 与 sec 在某条线路上是前后站的时候返回这条线路
			return i;
		}
	}
	return 0;
}


void find_path(int start, int end) { // 寻找最短路应该怎么走
	int x, fir, sec;
	stack <int> s;
	cout << "起点为: " << bus_point[start].sta_name  << endl;
	cout << "终点为: " << bus_point[end].sta_name << endl;
	if(dist[end] == inf) { // 从起点到终点的距离为无穷大时 证明没有线路可以走
		cout << "抱歉," << bus_point[start].sta_name << "站 到 " << bus_point[end].sta_name << "站 尚未开通公交车, 请选择其他出行方式!" << endl;
		return;
	}
	x = pre[end]; // 
	s.push(-1);
	while(x != -1) {
		s.push(x);
		x = pre[x];
	}
	fir = s.top(); s.pop();
	cout << "起点到终点的最短路径为: " ;
	while(!s.empty()) {
		sec = s.top();
		if(sec == -1) sec = end;
		int num = find_bus_line(fir, sec);
//		cout << "num = " << num << endl;
		cout << bus_point[fir].sta_name << " -乘坐" << name_hash[num] << "路-> ";
		fir = sec; s.pop();
	}
	cout << bus_point[end].sta_name;
	cout << "; \n最短距离为: " << dist[end] << endl;
}

void min_dist() {
	string start, end;
	int start_num, end_num;
//	printf("公交站的总个数个数为: %d\n", num_of_sta);
	printf("请输入起点位置: "); 
	cin >> start;
	for(int i = 0;i < bus_point.size(); i++) { // 寻找站点对应的编号
		if(bus_point[i].sta_name == start) {
			start_num = bus_point[i].sta_num;
			break;
		}
	}
	printf("请输入终点位置: ");
	cin >> end;
	for(int i = 0;i < bus_point.size(); i++) { // 寻找站点对应的编号
		if(bus_point[i].sta_name == end) {
			end_num = bus_point[i].sta_num;
			break;
		}
	}
	
	dijkstra(start_num);
	find_path(start_num,end_num);
}

void ask_point() { // 站点信息询问
	string point_name;
	printf("请输入需要查询站点的名称: ");
	cin >> point_name;
	for(int i = 0;i < bus_point.size(); i++) { // 
		if(point_name == bus_point[i].sta_name) {
			printf("本站可以搭乘的公交车有:\n");
			for(int j = 0;j < bus_point[i].bus_num.size(); j++) {
				cout << bus_point[i].bus_num[j] << "路" << endl;
			}
		}
	}
}

void ask_line() { // 线路询问
	string line_name;
	int line_num;
	printf("请输入你需要查询的线路:");
	while(cin >> line_name) { // 直到输入正确
		int cnt; // 它的大小可以判断是否找到了正确线路
		for(int i = 1;i <= line_of_bus; i++) { 
			cnt = i;
			if(line_name == name_hash[i]) {
				line_num = i;
				cnt = 0;
				break;
			}
		}
		cnt++;
		if(cnt > line_of_bus) printf("不好意思,没有查到这条公交线路,请重新输入!\n");
		else break;
	}
	
	cout << line_name; 
	printf("路经过以下站点:\n");
	for(int i = 0;i < bus_line[line_num].size(); i++) {
		cout << bus_line[line_num][i].sta_name;
		if(i < bus_line[line_num].size()-1) cout << " -- ";
	}
	puts("");
}

void change_point() { // 站点修改
	string point_name;
	int point_num;
	printf("请输入需要修改站点的名称: ");
	while(cin >> point_name) {
		bool is_found = 0;
		for(int i = 0;i < bus_point.size(); i++) { // 遍历站点信息，找到需要修改的那一个
			if(point_name == bus_point[i].sta_name) { // 如果找到了
				is_found = 1;
				point_num = i; // 记录站点的编号
				int choose;
				char YN;
				printf("1、为这个站点修改名称\n");
				printf("2、修改这个站到另一个站点的距离\n");
				scanf("%d", &choose);
					
				if(choose == 1) {
					string point_name_change;
					printf("请输入新的站点名称: ");
					cin >> point_name_change;
					cout << "你确定需要将 " << bus_point[i].sta_name << "站 的名字换成 " << point_name_change << "站 吗?(Y or N) ";
					cin >> YN;
					if(YN == 'Y' || YN == 'y') {
						// 进行两处的名字修改
						bus_point[i].sta_name = point_name_change; // 1、站点中的名字修改
						for(int i = 0;i < line_of_bus; i++) { // 2、路线中站点名字的修改
							for(int j = 0;j < bus_line[i].size(); j++) {
								if(bus_line[i][j].sta_name == point_name) {
									bus_line[i][j].sta_name = point_name_change;
								}
							}
						}
						printf("修改站点名称成功!\n");
					}
					else printf("已取消更名\n");
				}
					
					
				if(choose == 2) {
					string point_name_next;
					int len; 
					printf("请输入另一个站点的名称: ");
					cin >> point_name_next;
					for(int j = 0;j < bus_point.size(); j++) { // 遍历寻找下一个点的编号
						if(point_name_next == bus_point[j].sta_name) {
							if(mp[i][j] == inf) { // 两点之间的距离为无穷大，即并不相邻
								printf("这两个站并不相邻, 如果想要建立联系, 请另外增加公交线路!\n");
								break;
							}
							for(int k = 0;k < road_line.size(); k++) { // 遍历道路信息
								if((road_line[k].Left.sta_name == point_name && road_line[k].Right.sta_name == point_name_next) || (road_line[k].Right.sta_name == point_name && road_line[k].Left.sta_name == point_name_next)) { // 找到这两个点所在的道路
									printf("请输入这两个站点的更新距离: ");
									cin >> len;
									cout << "你确定需要将 " << point_name << "站 到 " << point_name_next << "站 的距离更新为 " << len << " 吗? (Y or N) ";
									cin >> YN;
									if(YN == 'Y' || YN == 'y') {
										road_line[k].cost = len;
										mp[i][j] = len; mp[j][i] = len;
										printf("距离更新成功!\n");
									}
									else printf("已取消距离更新!\n");
								}
							}
						}
					}
				}
			
			}
		
		}
		if(!is_found) {
			printf("没有找到这个站点,请重新输入\n");
		}
		else break;
	}
	
}


void del_point_init(int pos) {
	for(int i = pos;i < bus_point.size(); i++) {
		bus_point[i].sta_num--;
	}
}


void del_point() { // 删除站点
	string point_name;
	printf("输入需要删除站点的名称: ");
	while(cin >> point_name) {
		int check = check_point_exist(point_name); // 判断点是否存在
		if(!check) {
			printf("抱歉, 系统中没有找到这个点, 请重新输入\n");
		}
		else break;
	}
	
	bool can_del = 1;
	for(int i = 0;i < road_line.size(); i++) {
		// 判断道路中这个点是否与其他点有联系
		if(road_line[i].Left.sta_name == point_name || road_line[i].Right.sta_name == point_name) {
			printf("这个点与其他站点还具有关联, 不能删除!\n");
			can_del = 0;
			break;
		}
	}
	
	if(can_del) {
		for(int i = 0;i < bus_point.size(); i++) {
			if(bus_point[i].sta_name == point_name) { // 第i个站点为即将删除的站点
				bus_point.erase(bus_point.begin()+i); // 删除第i个站点
				del_point_init(i); // 将后面的站点序号向前移动一位
				num_of_sta--; // 站点总数--
				cout << point_name << "站 删除成功!" << endl;
			}
		}
	}
}

void add_point() { // 添加站点
	string line_name, add_name;
	int line_num, pos;
	struct Bus_line bl; // 存储公交线路
	struct Bus_point bp; // 存储站点信息
	struct Road_line rl; // 存储线路信息
	
	/***************添加进入公交线路***************/
	
	printf("请输入你需要在哪条路线上添加站点: ");
	cin >> line_name;
	for(int i = 1;i <= line_of_bus; i++) { // 寻找公交车序号
		if(name_hash[i] == line_name) {
			line_num = i;
			break;
		}
	}
	
	printf("你需要把站点放在第几个位置: ");
	cin >> pos; pos--; // 站点是从第0个开始的 所以需要减1
	
	printf("请输入你需要添加站点的名称:");
	cin >> add_name;
	bl.sta_name = add_name;
	bl.sta_num = pos;
	bus_line[line_num].insert(bus_line[line_num].begin()+pos, bl); // 在第pos处将bl这个站点添加进线路
	for(int i = pos+1;i < bus_line[line_num].size(); i++) bus_line[line_num][i].sta_num++; // 将第pos后的站点序号向后移动一位
	
	/***************添加进入站点***************/
	bp.sta_name = add_name; // 站点名称添加
	bp.bus_num.push_back(line_name); // 站点添加可以经过的线路
	bp.sta_num = num_of_sta++; // 添加站点序号，并且将总站点数量加1
	bus_point.push_back(bp); // 将结构体信息存入站点
	
	/***************添加进入道路***************/
	char YN;
	string point_name;
	string point_name_left, point_name_right;
	int Left, Right;
	bool exit1 = 0, exit2 = 0;
	
	printf("站点左端是否连接有车? (Y or N)");
	cin >> YN;
	if(YN == 'Y' || YN == 'y') {
		exit1 = 1;
		rl.Right = bp;
		Right = bp.sta_num;
		printf("请输入这个车站的名称: ");
		cin >> point_name;
		point_name_left = point_name;
		for(int i = 0;i < bus_point.size(); i++) { // 寻找输入点的序号
			if(bus_point[i].sta_name == point_name) {
				rl.Left = bus_point[i]; // 添加到新加点的左边
				Left = bus_point[i].sta_num;
				break;
			}
		}
		printf("请输入这两个个站点的距离: ");
		cin >> rl.cost;
		rl.road_num = road_line.size(); // 为这条路添加编号
		road_line.push_back(rl); // 加入到道路信息中去
		mp[Left][Right] = rl.cost; mp[Right][Left] = rl.cost; // 添加到图
	}
	else puts("OK!");
	
	// 右边添加与左边同理
	printf("站点右端是否连接有车? (Y or N)");
	cin >> YN;
	if(YN == 'Y' || YN == 'y') {
		exit2 = 1;
		rl.Left = bp;
		Left = bp.sta_num;
		printf("请输入这个车站的名称: ");
		cin >> point_name;
		point_name_right = point_name;
		for(int i = 0;i < bus_point.size(); i++) {
			if(bus_point[i].sta_name == point_name) {
				rl.Right = bus_point[i];
				Right = bus_point[i].sta_num;
				break;
			}
		}
		printf("请输入这两个个站点的距离: ");
		cin >> rl.cost;
		rl.road_num = road_line.size();
		road_line.push_back(rl);
		mp[Left][Right] = rl.cost; mp[Right][Left] = rl.cost;
	}
	else puts("OK!");
	
	if(exit1 && exit2) {// 如果新添加的点既有左边的点又有右边的点，那么需要删除原来本来连接的一条边
		for(int i = 0;i < road_line.size(); i++) { 
			if((point_name_left == road_line[i].Left.sta_name && point_name_right == road_line[i].Right.sta_name) || (point_name_left == road_line[i].Right.sta_name && point_name_right == road_line[i].Left.sta_name)) {
				road_line.erase(road_line.begin()+i);
			}
		}
	}
	
	// 重新建图
	init_map();
}


void del_line() { // 删除线路
	string line_name;
	int line_name_num;
	char YN;
	printf("请输入需要删除的线路: ");
	cin >> line_name;
	
	for(int i = 1;i <= line_of_bus; i++) { // 找到公交线对应的序号
		if(name_hash[i] == line_name) {
			line_name_num = i; 
			break;
		}
	}
	
	printf("请再次确认是否需要删除这条线路? (Y or N) ");
	cin >> YN;
	if(YN == 'Y' || YN == 'y') {
		// 一一解除两两路线的道路关联
		string fir, sec;
		fir = bus_line[line_name_num][0].sta_name; // 设为解除两两站点关系的第一个
		for(int i = 1;i < bus_line[line_name_num].size(); i++) { // 一一枚举下去
			sec = bus_line[line_name_num][i].sta_name; // 第二个设置为第一个的后面
			
			// 寻找在 fir 和 sec 这两个站点所在的路
			for(int j = 0;j < road_line.size(); j++) {
				if((fir == road_line[j].Left.sta_name && sec == road_line[j].Right.sta_name) || (fir == road_line[j].Right.sta_name && sec == road_line[j].Left.sta_name)) {
					
					// 寻找是否有其他线路经过这条路，如果没有的话就可以删除，如果有就不能删除。
					int cnt = 0;
					for(int k = 1;k <= line_of_bus; k++) {
						for(int l = 1;l <= bus_line[k].size()-1; l++) {
							if((bus_line[k][l].sta_name == fir && bus_line[k][l+1].sta_name == sec) || (bus_line[k][l].sta_name == sec && bus_line[k][l+1].sta_name == fir)) {
								cnt++;
							}
						}
					}
					if(cnt <= 1) {
						road_line.erase(road_line.begin()+j);
					} 
				}
			}
			fir = sec; // 更新第一个站点
		}
		// 从其他存储中删除这条线路信息
		name_hash.erase(line_name_num);
		// 全体向前移动
		for(int i = line_name_num+1; i <= line_of_bus; i++) {
			name_hash[i-1] = name_hash[i];
		}
		line_of_bus--;
		
		bus_line[line_name_num].erase(bus_line[line_name_num].begin(), bus_line[line_name_num].end());
		for(int i = 0;i < bus_point.size(); i++) {
			for(int j = 0;j < bus_point[i].bus_num.size(); j++) {
				if(bus_point[i].bus_num[j] == line_name) {
					bus_point[i].bus_num.erase(bus_point[i].bus_num.begin()+j);
					break;
				}
			}
		}
		
		// 重新建图
		init_map();
		printf("删除线路成功!\n");
	}
	else printf("已取消删除公交线路\n");
}


void add_line() { // 添加线路
	string new_line, pre_sta, new_sta;
	int new_sta_num, pre_sta_num;
	struct Bus_line bl;
	struct Road_line rl;
	
	printf("请输入该线路的名称: ");
	cin >> new_line;
	name_hash[++line_of_bus] = new_line;
	
	int cnt = 0;
	while(1) {
		cout << "(输入ok以停止) 请输入 " << new_line << "线 的第" << ++cnt << "个站: ";
		cin >> new_sta;
		if(new_sta == "ok") {
			cout << new_sta << "线 添加完成!" << endl;
			break;
		} 
		/***************公交车站点信息更新***************/
		int t = check_point_exist(new_sta);
		if(!t) {
			printf("抱歉, 不支持增加不存在的站点,请重新输入\n");
			cnt--;
			continue;
		}
		
		for(int i = 0;i < bus_point.size(); i++) {
			if(bus_point[i].sta_name == new_sta) {
				bus_point[i].bus_num.push_back(new_line);
				new_sta_num = i;
				break;
			}
		}
		
		/***************道路信息更新***************/
		if(cnt == 1) {
			pre_sta = new_sta;
			pre_sta_num = new_sta_num;
		}
		else {
			bool exit_road = 0;
			for(int i = 0;i < road_line.size(); i++) {
				if((pre_sta == road_line[i].Left.sta_name && new_sta == road_line[i].Right.sta_name) || (pre_sta == road_line[i].Right.sta_name && new_sta == road_line[i].Left.sta_name)) {
					printf("此站与前一个站已经存在路程,因此不再添加\n");
					exit_road = 1;
					break;
				}
			}
			if(!exit_road) {
				int len;
				cout << "未检测到 " << pre_sta <<"站 与 " << new_sta << "站 有路程,请添加: ";
				cin >> len;
				
				rl.Left = bus_point[pre_sta_num];
				rl.Right = bus_point[new_sta_num];
				rl.cost = len;
				road_line.push_back(rl);
			}
			pre_sta = new_sta;
			pre_sta_num = new_sta_num;
		}
	}
	
	// 重新建图
	init_map();
	
}

void change_line() {
	string line_name;
	int line_name_num;
	printf("输入需要修改的线路 :");
	cin >> line_name;
	
	for(int i = 1;i <= line_of_bus; i++) {
		if(name_hash[i] == line_name) {
			line_name_num = i; 
			break;
		}
	}
	
	string fir, sec;
	fir = bus_line[line_name_num][0].sta_name;
	for(int i = 1;i < bus_line[line_name_num].size(); i++) {
		sec = bus_line[line_name_num][i].sta_name;
		
		// 寻找在 fir 和 sec 这两个站点所在的路
		for(int j = 0;j < road_line.size(); j++) {
			if((fir == road_line[j].Left.sta_name && sec == road_line[j].Right.sta_name) || (fir == road_line[j].Right.sta_name && sec == road_line[j].Left.sta_name)) {
				
				// 寻找是否有其他线路经过这条路，如果没有的话就可以删除，如果有就不能删除。
				int cnt = 0;
				for(int k = 1;k <= line_of_bus; k++) {
					for(int l = 1;l <= bus_line[k].size()-1; l++) {
						if((bus_line[k][l].sta_name == fir && bus_line[k][l+1].sta_name == sec) || (bus_line[k][l].sta_name == sec && bus_line[k][l+1].sta_name == fir)) {
							cnt++;
						}
					}
				}
				if(cnt <= 1) {
					road_line.erase(road_line.begin()+j);
				} 
			}
		}
		fir = sec;
	}
	
	// 从其他存储中删除这条线路信息
	name_hash.erase(line_name_num);
	// 全体向前移动
	for(int i = line_name_num+1; i <= line_of_bus; i++) {
		name_hash[i-1] = name_hash[i];
	}
	line_of_bus--;
	
	bus_line[line_name_num].erase(bus_line[line_name_num].begin(), bus_line[line_name_num].end());
	for(int i = 0;i < bus_point.size(); i++) {
		for(int j = 0;j < bus_point[i].bus_num.size(); j++) {
			if(bus_point[i].bus_num[j] == line_name) {
				bus_point[i].bus_num.erase(bus_point[i].bus_num.begin()+j);
				break;
			}
		}
	}

	
	/*********上为删除*********下为添加******总为修改***********/
	
	string new_line, pre_sta, new_sta;
	int new_sta_num, pre_sta_num;
	struct Bus_line bl;
	struct Road_line rl;
	
	new_line = line_name;
	name_hash[++line_of_bus] = new_line;
	
	int cnt = 0;
	while(1) {
		cout << "(输入ok以停止) 请输入修改后 " << new_line << "线 的第" << ++cnt << "个站: ";
		cin >> new_sta;
		if(new_sta == "ok") {
			cout << line_name <<"路 修改完成!" << endl;
			break;
		} 
		/***************公交车站点信息更新***************/
		int t = check_point_exist(new_sta);
		if(!t) {
			printf("抱歉, 不支持增加不存在的站点,请重新输入\n");
			cnt--;
			continue;
		}
		
		for(int i = 0;i < bus_point.size(); i++) {
			if(bus_point[i].sta_name == new_sta) {
				bus_point[i].bus_num.push_back(new_line);
				new_sta_num = i;
				break;
			}
		}
		
		/***************道路信息更新***************/
		if(cnt == 1) {
			pre_sta = new_sta;
			pre_sta_num = new_sta_num;
		}
		else {
			bool exit_road = 0;
			for(int i = 0;i < road_line.size(); i++) {
				if((pre_sta == road_line[i].Left.sta_name && new_sta == road_line[i].Right.sta_name) || (pre_sta == road_line[i].Right.sta_name && new_sta == road_line[i].Left.sta_name)) {
					printf("此站与前一个站同为其他站点的路段所以已经存在路程,因此不再添加\n");
					exit_road = 1;
					break;
				}
			}
			if(!exit_road) {
				int len;
				cout << "请重新输入 " << pre_sta <<"站 与 " << new_sta << "站 有路程: ";
				cin >> len;
				
				rl.Left = bus_point[pre_sta_num];
				rl.Right = bus_point[new_sta_num];
				rl.cost = len;
				road_line.push_back(rl);
			}
			pre_sta = new_sta;
			pre_sta_num = new_sta_num;
		}
	}
	
	// 重新建图
	init_map();
}


void bfs(int end_num) {
	while(!q.empty()) {
		int cnt = 0;
		struct Short_trasfer st, t;
		st = q.front(); q.pop();
		if(st.bp == end_num) {
			printf("最少需要换乘%d次\n", st.trasfer);
			cout << bus_point[st.bus_proccess[0]].sta_name;
			for(int i = 1;i < st.bus_proccess.size()-1; i++) {
				cout << " -" << name_hash[st.bus_line[cnt++]] << "路- "<< bus_point[st.bus_proccess[i]].sta_name;
			}
			cout << " -" << name_hash[st.bus_line[cnt++]] << "路- " << bus_point[st.bus_proccess[st.bus_proccess.size()-1]].sta_name << endl;
			return;
		}
		
		for(int i = 0;i < bus_point[st.bp].bus_num.size(); i++) { // 遍历此站公交车可以到达各个线路
			string line_name = bus_point[st.bp].bus_num[i]; // 807
			int line_name_num;
			
			for(int j = 1;j <= line_of_bus; j++) { // 寻找公交线路的编号
				if(name_hash[j] == line_name) {
					line_name_num = j;
					break;
				}
			}
			
//			cout << bus_line[line_name_num].size() << endl;getchar(); cout << "hello" << endl;
			for(int j = 0;j < bus_line[line_name_num].size(); j++) { // 寻找这路公交车可以到达的站点
				t.bp = bus_line[line_name_num][j].sta_num;
				t.trasfer = st.trasfer+1;
				t.bus_proccess = st.bus_proccess;
				t.bus_proccess.push_back(t.bp);
				t.bus_line = st.bus_line;
				t.bus_line.push_back(line_name_num);
				q.push(t);
			}
		}
		
		
	}
}


void shortest_transfer() {
	string start, end;
	int start_num, end_num;
//	printf("公交站的总个数个数为: %d\n", num_of_sta);
	printf("请输入起点位置: ");
	cin >> start;
	for(int i = 0;i < bus_point.size(); i++) {
		if(bus_point[i].sta_name == start) {
			start_num = bus_point[i].sta_num;
			break;
		}
	}
	
	printf("请输入终点位置: ");
	cin >> end;
	for(int i = 0;i < bus_point.size(); i++) {
		if(bus_point[i].sta_name == end) {
			end_num = bus_point[i].sta_num;
			break;
		}
	}
	
	while(!q.empty()) q.pop();
	struct Short_trasfer st;
	st.bp = start_num;
	st.trasfer = -1;
	st.bus_proccess.push_back(start_num);
	q.push(st);
	
	bfs(end_num);
}



int go() {
	int choose;
	printf("1、最短路查询\n");
	printf("2、查询站点信息\n");
	printf("3、查询线路信息\n");
	printf("4、修改站点信息\n");
	printf("5、删除站点\n");
	printf("6、添加站点\n");
	printf("7、删除线路\n");
	printf("8、增加线路\n");
	printf("9、修改线路\n");
	printf("10、最短换乘\n");
	printf("请选择: ");
	scanf("%d", &choose);
	if(choose == 1) {
		min_dist();
	}
	if(choose == 2) {
		ask_point();
	}
	if(choose == 3) {
		ask_line();
	}
	if(choose == 4) {
		change_point();
	}
	if(choose == 5) {
		del_point();
	}
	if(choose == 6) {
		add_point();
	}
	if(choose == 7) {
		del_line();
	}
	if(choose == 8) {
		add_line();
	}
	if(choose == 9) {
		change_line();
	}
	if(choose == 10) {
		shortest_transfer();
	}
	return choose;
}

int main() {
	build_sta();
	build_map();
	build_line();
	while(1) {
		int t = go();
		if(t == 0) {
			input_file();
			break;
		} 
	}
}