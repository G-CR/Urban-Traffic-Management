//
//  Urban Traffic Management.hpp
//  Urban Traffic Management
//
//  Created by 龚朝瑞 on 2019/12/18.
//  Copyright © 2019 龚朝瑞. All rights reserved.
//

#ifndef Urban_Traffic_Management_hpp
#define Urban_Traffic_Management_hpp

#include <stdio.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
#include <map>
#include <stack>
#include <string.h>

#endif /* Urban_Traffic_Management_hpp */
using namespace std;

//用户信息部分需要的数据结构
struct User {
    char nicheng[20];
    char mima[20];
    char name[20];
    char sex[20];
    char age[20];
};

// 线路部分需要的数据结构
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



// 用户账号管理函数
// 文件读取
void build_user();

void home_page(); // 程序入口

// 普通用户功能函数
void normal_login_page();
void normal_home_page();
void del_normal();
void change_normal_information();


// 管理员用户功能函数
void identity_verify();
void manager_login_page();
void manager_home_page();
void del_manager();
void change_manager_information();
void line_manage();
void point_manage();
void bus_enquiry();




// 公交线路管理函数
void input_file(); // 写入文件操作
int check_point_exist(string s); // 检查站点是否存在
void build_sta(); // 建站
void build_line(); // 建公交线路
void init_map(); // 初始化图
void build_map(); // 建图
void dijkstra(int st); // 最短路算法
int find_bus_line(int fir, int sec);// 最短路中为了查询两站之间坐什么车 返回道路序号
void find_path(int start, int end); // 寻找最短路应该怎么走
void min_dist(); // 最短距离
void ask_point(); // 站点信息询问
void ask_line(); // 线路询问
void change_point(); // 站点修改
void del_point_init(int pos); // 删点前移
void del_point(); // 删除站点
void add_point(); // 添加站点
void del_line(); // 删除线路
void add_line(); // 添加线路
void change_line(); // 改变线路
void bfs(int end_num); // 宽度优先搜索
void shortest_transfer(); // 最短换乘

void test_output(); // 测试输出
