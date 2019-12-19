//
//  Urban Traffic Management.hpp
//  Urban Traffic Management
//
//  Created by 龚朝瑞 on 2019/12/18.
//  Copyright © 2019 龚朝瑞. All rights reserved.
//

#ifndef Urban_Traffic_Management_hpp
#define Urban_Traffic_Management_hpp
#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>
#endif /* Urban_Traffic_Management_hpp */

using namespace std;

struct Bus_point { // 公交车站点信息
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

// 用户管理



/*----------------------------------------------*/
void build_sta(); // 建站
void build_map(); // 建图

// 站点管理
void Bus_sta_add(); // 站点添加
void Bus_sta_del(); // 站点删除


// 线路管理



// 查询功能
