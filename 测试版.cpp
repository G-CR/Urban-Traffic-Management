#include <bits/stdc++.h>
using namespace std;

struct Bus_point { // 公交车站点信息
	int sta_num;
	string sta_name; // 站点名称
	vector<string> bus_num; // 公交车号
};

struct Road_line {
	struct Bus_point Left, Right; // 一条线路的左右段的站点
	int cost; // 这条路线的长度
};

struct Bus_line { // 路线信息
	string sta_name; // 经过站点名称
	int sta_num; // 站点号码
};

vector <Bus_point> bus_point;
vector <Bus_line> bus_line;
vector <Road_line> road_line;

void build_sta() { // 建站
	struct Bus_point bp;
	FILE *fp;
	char s[30];
	fp = fopen("/Users/gongzhaorui/Desktop/trafic_information/bus_sta.txt", "r");
	bool t = 0;
	if(fp != NULL) {
		while(!feof(fp)) {
			fscanf(fp, "%s", s);
			if(strlen(s) < 4) {
				bp.bus_num.push_back(s);
			}
			else {
				if(t) bus_point.push_back(bp);
				t=1;
				bp.bus_num.clear();
				bp.sta_name = s;
			}
		}
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

void build_map() {
	struct Bus_point bp;
		struct Road_line rl;
		FILE *fp;
		int a, b, c;
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
		
		//测试输出
//		for(int i = 0;i < road_line.size(); i++) {
//			printf("%d %d %d\n", road_line[i].Left.sta_num, road_line[i].Right.sta_num, road_line[i].cost);
//		}
}



int main() {
	build_sta();
	build_map();
}