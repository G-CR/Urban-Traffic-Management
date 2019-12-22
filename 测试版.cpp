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

struct Short_trasfer {
	int cost;
	vector <string> sta_name;
	vector <string> sta_num;
};

vector <Bus_point> bus_point;
vector <Bus_line> bus_line[10];
vector <Road_line> road_line;
map <int, string> name_hash;
queue <> short_q;
int mp[100][100]; // 地图
int dist[100], flag[100], pre[100];
int num_of_sta, line_of_bus; // 站点个数, 公交线路个数


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
	
	
}

int check_point_exist(string s) { // 检查站点是否存在
	for(int i = 0;i < bus_point.size(); i++) {
		if(s == bus_point[i].sta_name) return 1;
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
		while(!feof(fp)) {
			fscanf(fp, "%s", s);
//			puts(s);
			if(strlen(s) < 4) {
				bp.bus_num.push_back(s);
			}
			else {
				num_of_sta++;
				if(t) bus_point.push_back(bp);
//				cout << bp.sta_name << " ";
//				for(int i = 0;i < bp.bus_num.size(); i++) cout << bp.bus_num[i]<< " ";
//				puts("");
				t=1;
				bp.bus_num.clear();
				bp.sta_name = s;
			}
		}
		bus_point.push_back(bp);
	}
	else {
		printf("文件打开失败!\n");
	}
	for(int i = 0; i < bus_point.size(); i++) {
		bus_point[i].sta_num = i;
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


void build_line() { // 建线路
	struct Bus_line bl;
	char s[30];
	FILE *fp;
	fp = fopen("/Users/gongzhaorui/Desktop/trafic_information/bus_line.txt", "r");
	if(fp != NULL) {
		while(!feof(fp)) {
			fscanf(fp, "%s", s);
//			puts(s);
			if(strlen(s) < 4) {
				line_of_bus++;
				name_hash[line_of_bus] = s;
			}
			else {
				bl.sta_name = s;
				for(int i = 0;i < bus_point.size(); i++) {
					if(bus_point[i].sta_name == s) {
						bl.sta_num = bus_point[i].sta_num;
						break;
					}
				}
				bus_line[line_of_bus].push_back(bl);
			}
		}
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
			rl.Left = bus_point[a];
			rl.Right = bus_point[b];
			rl.cost = c;
			road_line.push_back(rl);
		}
	}
	else {
		printf("文件打开失败!\n");
	}
		
//		测试输出
//		for(int i = 0;i < road_line.size(); i++) {
//			printf("%d %d %d\n", road_line[i].Left.sta_num, road_line[i].Right.sta_num, road_line[i].cost);
//		}
	for(int i = 0;i < road_line.size(); i++) {
			road_line[i].road_num = i;
//			cout << i << endl;
}
		
		
	// 建图
	for(int i = 0;i < 100; i++) {
		for(int j = 0;j < 100; j++) {
			mp[i][j] = inf;
		}
	}
	for(int i = 0;i < road_line.size(); i++) {
		mp[road_line[i].Left.sta_num][road_line[i].Right.sta_num] = road_line[i].cost;
		mp[road_line[i].Right.sta_num][road_line[i].Left.sta_num] = road_line[i].cost;
//		printf("%d -> %d = %d\n", road_line[i].Left.sta_num,road_line[i].Right.sta_num,road_line[i].cost);
	}
	
}


void dijkstra(int st)
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
	for(int i = 1;i <= line_of_bus; i++) {
		for(int j = 0;j < bus_line[i].size()-1; j++) {
			if((bus_point[fir].sta_name == bus_line[i][j].sta_name && bus_point[sec].sta_name == bus_line[i][j+1].sta_name) || (bus_point[sec].sta_name == bus_line[i][j].sta_name && bus_point[fir].sta_name == bus_line[i][j+1].sta_name))
			return i;
		}
	}
	return 0;
}



void find_path(int start, int end) {
	int x, fir, sec;;
	stack <int> s;
	cout << "起点为: " << bus_point[start].sta_name  << endl;
	cout << "终点为: " << bus_point[end].sta_name << endl;
	if(dist[end] == inf) {
		cout << "抱歉," << bus_point[start].sta_name << "站 到 " << bus_point[end].sta_name << "站 尚未开通公交车, 请选择其他出行方式!" << endl;
		return;
	}
	x = pre[end];
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
	
	dijkstra(start_num);
	find_path(start_num,end_num);
}

void ask_point() {
	string point_name;
	printf("请输入需要查询站点的名称: ");
	cin >> point_name;
	for(int i = 0;i < bus_point.size(); i++) {
		if(point_name == bus_point[i].sta_name) {
			printf("本站可以搭乘的公交车有:\n");
			for(int j = 0;j < bus_point[i].bus_num.size(); j++) {
				cout << bus_point[i].bus_num[j] << "路" << endl;
			}
		}
	}
}

void ask_line() {
	string line_name;
	int line_num;
	printf("请输入你需要查询的线路:");
	while(cin >> line_name) {
		int cnt;
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

void change_point() {
	string point_name;
	int point_num;
	printf("请输入需要修改站点的名称: ");
	cin >> point_name;
	for(int i = 0;i < bus_point.size(); i++) {
		if(point_name == bus_point[i].sta_name) {
			point_num = i;
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
					bus_point[i].sta_name = point_name_change;
					for(int i = 0;i < line_of_bus; i++) {
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
				for(int j = 0;j < bus_point.size(); j++) {
					if(point_name_next == bus_point[j].sta_name) {
						if(mp[i][j] == inf) {
							printf("这两个站并不相邻, 如果想要建立联系, 请另外增加公交线路!\n");
							break;
						}
						for(int k = 0;k < road_line.size(); k++) {
							if((road_line[k].Left.sta_name == point_name && road_line[k].Right.sta_name == point_name_next) || (road_line[k].Right.sta_name == point_name && road_line[k].Left.sta_name == point_name_next)) {
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
}


void del_point_init(int pos) {
	for(int i = pos;i < bus_point.size(); i++) {
		bus_point[i].sta_num--;
	}
}


void del_point() { // 删除站点
	string point_name;
	printf("输入需要删除站点的名称: ");
	while(1) {
		cin >> point_name;
		int check = check_point_exist(point_name);
		if(!check) {
			printf("抱歉, 系统中没有找到这个点, 请重新输入\n");
		}
		else break;
	}
	
	bool can_del = 1;
	for(int i = 0;i < road_line.size(); i++) {
		if(road_line[i].Left.sta_name == point_name || road_line[i].Right.sta_name == point_name) {
			printf("这个点与其他站点还具有关联, 不能删除!\n");
			can_del = 0;
			break;
		}
	}
	
	if(can_del) {
		for(int i = 0;i < bus_point.size(); i++) {
			if(bus_point[i].sta_name == point_name) {
				bus_point.erase(bus_point.begin()+i);
				del_point_init(i);
				num_of_sta--;
				cout << point_name << "站 删除成功!" << endl;
			}
		}
	}
}

void add_point() {
	string line_name, add_name;
	int line_num, pos;
	struct Bus_line bl;
	struct Bus_point bp;
	struct Road_line rl;
	
	/***************添加进入公交线路***************/
	
	printf("请输入你需要在哪条路线上添加站点: ");
	cin >> line_name;
	for(int i = 0;i < line_of_bus; i++) {
		if(name_hash[i] == line_name) {
			line_num = i;
			break;
		}
	}
	
	printf("你需要把站点放在第几个位置: ");
	cin >> pos; pos--;
	
	printf("请输入你需要添加站点的名称:");
	cin >> add_name;
	bl.sta_name = add_name;
	bl.sta_num = pos;
	bus_line[line_num].insert(bus_line[line_num].begin()+pos, bl);
	for(int i = pos+1;i < bus_line[line_num].size(); i++) bus_line[line_num][i].sta_num++;
	
	/***************添加进入站点***************/
	bp.sta_name = add_name;
	bp.bus_num.push_back(line_name);
	bp.sta_num = num_of_sta++;
	bus_point.push_back(bp);
	
	/***************添加进入道路***************/
	char YN;
	string point_name;
	string point_name_left, point_name_right;
	int Left, Right;
	
	printf("站点左端是否连接有车? (Y or N)");
	cin >> YN;
	if(YN == 'Y' || YN == 'y') {
		rl.Right = bp;
		Right = bp.sta_num;
		printf("请输入这个车站的名称: ");
		cin >> point_name;
		point_name_left = point_name;
		for(int i = 0;i < bus_point.size(); i++) {
			if(bus_point[i].sta_name == point_name) {
				rl.Left = bus_point[i];
				Left = bus_point[i].sta_num;
				break;
			}
		}
		printf("请输入这两个个站点的距离: ");
		cin >> rl.cost;
		rl.road_num = road_line.size();
		road_line.push_back(rl);
		mp[Left][Right] = rl.cost; mp[Right][Left] = rl.cost;
	}
	
	
	printf("站点右端是否连接有车? (Y or N)");
	cin >> YN;
	if(YN == 'Y' || YN == 'y') {
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
	
	for(int i = 0;i < road_line.size(); i++) {
		if((point_name_left == road_line[i].Left.sta_name && point_name_right == road_line[i].Right.sta_name) || (point_name_left == road_line[i].Right.sta_name && point_name_right == road_line[i].Left.sta_name)) {
			road_line.erase(road_line.begin()+i);
		}
	}

	// 重新建图
	for(int i = 0;i < 100; i++) {
		for(int j = 0;j < 100; j++) {
			mp[i][j] = inf;
		}
	}
	for(int i = 0;i < road_line.size(); i++) {
			mp[road_line[i].Left.sta_num][road_line[i].Right.sta_num] = road_line[i].cost;
			mp[road_line[i].Right.sta_num][road_line[i].Left.sta_num] = road_line[i].cost;
	}
}



void del_line() {
	string line_name;
	int line_name_num;
	char YN;
	printf("请输入需要删除的线路: ");
	cin >> line_name;
	
	for(int i = 1;i <= line_of_bus; i++) {
		if(name_hash[i] == line_name) {
			line_name_num = i; 
			break;
		}
	}
	
	printf("请再次确认是否需要删除这条线路? (Y or N) ");
	cin >> YN;
	if(YN == 'Y' || YN == 'y') {
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
		
		// 重新建图
		for(int i = 0;i < 100; i++) {
			for(int j = 0;j < 100; j++) {
				mp[i][j] = inf;
			}
		}
		for(int i = 0;i < road_line.size(); i++) {
				mp[road_line[i].Left.sta_num][road_line[i].Right.sta_num] = road_line[i].cost;
				mp[road_line[i].Right.sta_num][road_line[i].Left.sta_num] = road_line[i].cost;
		}
		printf("删除线路成功!\n");
	}
	else printf("已取消删除公交线路\n");
}


void add_line() {
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
	for(int i = 0;i < 100; i++) {
		for(int j = 0;j < 100; j++) {
			mp[i][j] = inf;
		}
	}
	for(int i = 0;i < road_line.size(); i++) {
			mp[road_line[i].Left.sta_num][road_line[i].Right.sta_num] = road_line[i].cost;
			mp[road_line[i].Right.sta_num][road_line[i].Left.sta_num] = road_line[i].cost;
	}
	
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
	for(int i = 0;i < 100; i++) {
		for(int j = 0;j < 100; j++) {
			mp[i][j] = inf;
		}
	}
	for(int i = 0;i < road_line.size(); i++) {
			mp[road_line[i].Left.sta_num][road_line[i].Right.sta_num] = road_line[i].cost;
			mp[road_line[i].Right.sta_num][road_line[i].Left.sta_num] = road_line[i].cost;
	}


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
	return choose;
}

int main() {
	build_sta();
	build_map();
	build_line();
	while(1) {
		int t = go();
		if(t == 0) break;
	}
}