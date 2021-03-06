#include "pch.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>


using namespace std;

typedef struct PROCESS {
	int id;
	int startTime;
	int runTime;
	int endTime;
	int prioeity;
	int timeSlice;
	bool finish;//1:执行完成
}Pro;

Pro pro[1000];

int cmp1(const void *a, const void *b) {
	Pro *temp1 = (Pro*)a;
	Pro *temp2 = (Pro*)b;
	if (temp1->startTime == temp2->startTime)
		return temp1->id - temp2->id;
	else
		return temp1->startTime - temp2->startTime;
}
bool cmp2(const Pro &a, const Pro &b) {
	if (a.prioeity == b.prioeity) {
		if (a.startTime == b.startTime)
			return a.id < b.id;
		else
			return a.startTime < b.startTime;
	}
	else
		return a.prioeity < b.prioeity;
}

int main() {
	int flag;
	cin >> flag;
	if (flag == 1) {//先来先服务
		int id, st, rt, pri, ts, j = 1;
		while (~scanf("%d/%d/%d/%d/%d", &id, &st, &rt, &pri, &ts)) {
			pro[j].id = id;
			pro[j].startTime = st;
			pro[j].runTime = rt;
			pro[j].prioeity = pri;
			pro[j++].timeSlice = ts;
		}
		int len = j - 1;
		qsort(pro, len + 1, sizeof(Pro), cmp1);
		for (int i = 1; i <= len; i++) {
			pro[i].startTime = pro[i - 1].endTime > pro[i].startTime ?
				pro[i - 1].endTime : pro[i].startTime;
			pro[i].endTime = pro[i].startTime + pro[i].runTime;
			printf("%d/%d/%d/%d/%d\n", i, pro[i].id, pro[i].startTime, pro[i].endTime, pro[i].prioeity);
		}
	}
	else if (flag == 2) {//短作业优先
		int id, st, rt, pri, ts, j = 1;
		while (~scanf("%d/%d/%d/%d/%d", &id, &st, &rt, &pri, &ts)) {
			pro[j].id = id;
			pro[j].startTime = st;
			pro[j].runTime = rt;
			pro[j].prioeity = pri;
			pro[j++].timeSlice = ts;
		}
		int len = j - 1, loop = 1, time = 0, count = 0;
		qsort(pro, len + 1, sizeof(Pro), cmp1);
		while (count != len) {
			int temp = 1;
			for (j = 1; j <= len; j++) {
				if (pro[j].finish)
					continue;
				if (time >= pro[j].startTime)
					break;
			}
			if (j == len + 1) {
				for (int i = 1; i <= len; i++)
					if (!pro[i].finish) {
						time = pro[i].startTime;
						temp = i;
						break;
					}
			}
			else
				temp = j;
			for (int i = 1; i <= len; i++) {
				if (pro[i].finish || pro[i].startTime > time)
					continue;
				if (pro[i].runTime == pro[temp].runTime) {
					if (pro[i].startTime == pro[temp].startTime) {
						if (pro[i].id < pro[temp].id)
							temp = i;
					}
					else if (pro[i].startTime < pro[temp].startTime)
						temp = i;
				}
				else if (pro[i].runTime < pro[temp].runTime)
					temp = i;
			}
			printf("%d/%d/%d/%d/%d\n", loop++, pro[temp].id, time, time + pro[temp].runTime, pro[temp].prioeity);
			time += pro[temp].runTime;
			pro[temp].finish = true;
			count++;
		}
	}
	else if (flag == 3) {//最短剩余时间优先
		int id, st, rt, pri, ts, j = 1;
		while (~scanf("%d/%d/%d/%d/%d", &id, &st, &rt, &pri, &ts)) {
			pro[j].id = id;
			pro[j].startTime = st;
			pro[j].runTime = rt;
			pro[j].prioeity = pri;
			pro[j++].timeSlice = ts;
		}
		int len = j - 1;
		qsort(pro, len + 1, sizeof(Pro), cmp1);
		int t = -1, t1 = -1, run = 0, loop = 1;
		for (int time = pro[0].startTime; ; time++) {
			int count = 0;
			for (int i = 1; i <= len; i++) {
				if (pro[i].finish) {
					count++;
					continue;
				}
				if (pro[i].startTime <= time) {
					if (t1 == -1) {
						t1 = i;
					}
					else {
						if (pro[t1].runTime == pro[i].runTime) {
							if (pro[t1].startTime == pro[i].startTime) {
								if (pro[t1].id >= pro[i].id)
									t1 = i;
							}
							else if (pro[t1].startTime > pro[i].startTime)
								t1 = i;
						}
						else if (pro[t1].runTime > pro[i].runTime)
							t1 = i;
					}
				}
			}
			if (count == len)
				break;
			t = t == -1 ? t1 : t;
			if (t == t1) {
				pro[t].runTime--;
				run++;
				if (pro[t].runTime == 0) {
					printf("%d/%d/%d/%d/%d\n", loop++, pro[t].id, time + 1 - run, time + 1, pro[t].prioeity);
					pro[t].finish = true;
					t = -1;
					run = 0;
				}
			}
			else {
				printf("%d/%d/%d/%d/%d\n", loop++, pro[t].id, time - run, time, pro[t].prioeity);
				t = t1;
				pro[t].runTime--;
				run = 1;
				if (pro[t].runTime == 0) {
					printf("%d/%d/%d/%d/%d\n", loop++, pro[t].id, time + 1 - run, time + 1, pro[t].prioeity);
					pro[t].finish = true;
					t = -1;
					run = 0;
				}
			}
			t1 = -1;
		}
	}
	else if (flag == 4) {//时间轮转片
		int id, st, rt, pri, ts, j = 1;
		while (~scanf("%d/%d/%d/%d/%d", &id, &st, &rt, &pri, &ts)) {
			pro[j].id = id;
			pro[j].startTime = st;
			pro[j].runTime = rt;
			pro[j].prioeity = pri;
			pro[j++].timeSlice = ts;
		}
		int len = j - 1, loop = 1, time = 0;
		qsort(pro, len + 1, sizeof(Pro), cmp1);
		vector<Pro> line;
		while (1) {
			if (line.empty()) {
				for (int i = 1; i <= len; i++)
					if (!pro[i].finish) {
						time = pro[i].startTime;
						break;
					}
			}
			for (int i = 1; i <= len; i++) {
				if (!pro[i].finish && time >= pro[i].startTime) {
					line.push_back(pro[i]);
					pro[i].finish = true;
				}
			}
			Pro temp = line[0];
			line.erase(line.begin());
			if (temp.runTime <= temp.timeSlice) {
				printf("%d/%d/%d/%d/%d\n", loop++, temp.id, time, time + temp.runTime, temp.prioeity);
				time += temp.runTime;
				temp.runTime = 0;
			}
			else {
				printf("%d/%d/%d/%d/%d\n", loop++, temp.id, time, time + temp.timeSlice, temp.prioeity);
				time += temp.timeSlice;
				for (int i = 1; i <= len; i++) {
					if (!pro[i].finish && time >= pro[i].startTime) {
						line.push_back(pro[i]);
						pro[i].finish = true;
					}
				}
				temp.runTime -= temp.timeSlice;
				line.push_back(temp);
			}
			if (line.empty())
				break;
		}
	}
	else {//动态优先级
		int id, st, rt, pri, ts, j = 1;
		while (~scanf("%d/%d/%d/%d/%d", &id, &st, &rt, &pri, &ts)) {
			pro[j].id = id;
			pro[j].startTime = st;
			pro[j].runTime = rt;
			pro[j].prioeity = pri;
			pro[j++].timeSlice = ts;
		}
		int len = j - 1, loop = 1, time = 0;
		qsort(pro, len + 1, sizeof(Pro), cmp1);
		vector<Pro> line;
		while (1) {
			if (line.empty()) {
				for (j = 1; j <= len; j++) {
					if (!pro[j].finish) {
						time = pro[j].startTime;
						break;
					}
				}
				if (j == len + 1)
					break;
			}
			for (int i = 1; i <= len; i++) {
				if (!pro[i].finish && pro[i].startTime <= time) {
					line.push_back(pro[i]);
					pro[i].finish = true;
				}
			}
			sort(line.begin(), line.end(), cmp2);
			Pro temp = line[0];
			line.erase(line.begin());
			if (temp.runTime <= temp.timeSlice) {
				temp.prioeity += 3;
				printf("%d/%d/%d/%d/%d\n", loop++, temp.id, time, time + temp.runTime, temp.prioeity);
				for (vector<Pro>::iterator it = line.begin(); it != line.end(); it++) {
					it->prioeity = it->prioeity == 0 ? 0 : it->prioeity - 1;
				}
				time += temp.runTime;
				for (int i = 1; i <= len; i++) {
					if (!pro[i].finish && pro[i].startTime <= time) {
						line.push_back(pro[i]);
						pro[i].finish = true;
					}
				}
			}
			else {
				temp.prioeity += 3;
				printf("%d/%d/%d/%d/%d\n", loop++, temp.id, time, time + temp.timeSlice, temp.prioeity);
				for (vector<Pro>::iterator it = line.begin(); it != line.end(); it++) {
					it->prioeity = it->prioeity == 0 ? 0 : it->prioeity - 1;
				}
				time += temp.timeSlice;
				for (int i = 1; i <= len; i++) {
					if (!pro[i].finish && pro[i].startTime <= time) {
						line.push_back(pro[i]);
						pro[i].finish = true;
					}
				}
				temp.runTime -= temp.timeSlice;
				line.push_back(temp);
			}
			if (line.empty())
				break;
		}
	}
}