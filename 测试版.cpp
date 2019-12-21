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

vector <Bus_point> bus_point;
vector <Bus_line> bus_line[10];
vector <Road_line> road_line;
map <int, string> name_hash;
int mp[100][100]; // 地图
int dist[100], flag[100], pre[100];
int num_of_sta, line_of_bus; // 站点个数, 公交线路个数


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
	
	for(int i = 0;i < road_line.size(); i++) {
		road_line[i].road_num = i;
//		cout << i << endl;
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

int find_bus_line(int fir, int sec) {
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

void go() {
	int choose;
	printf("1、最短路查询\n");
	
	
	printf("请选择: ");
	scanf("%d", &choose);
	if(choose == 1) {
		min_dist();
	}
}

int main() {
	build_sta();
	build_map();
	build_line();
	go();
}
