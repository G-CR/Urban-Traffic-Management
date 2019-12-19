//
//  Urban Traffic Management.cpp
//  Urban Traffic Management
//
//  Created by 龚朝瑞 on 2019/12/18.
//  Copyright © 2019 龚朝瑞. All rights reserved.
//

#include "Urban Traffic Management.hpp"


void build_map() { // 建图
	struct Bus_point bp;
	FILE *fp;
	char s[30];
	fp = fopen("/Users/gongzhaorui/Desktop/trafic_information/bus_sta.txt", "r");
	int t = 0;
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
		cout << i+1 << " " << bus_point[i].sta_name<< " ";
		for(int j = 0;j < bus_point[i].bus_num.size(); j++) {
			cout << bus_point[i].bus_num[j] << " ";
		}
		puts("");
	}

}

void Bus_sta_add()  { // 站点添加
	
}
