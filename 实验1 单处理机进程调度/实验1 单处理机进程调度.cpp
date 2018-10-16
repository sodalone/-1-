#include "pch.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>


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
		while (1) {
			int count = 0;
			for (int i = 1; i <= len; i++) {
				if (pro[i].finish)
					count++;
				else {
					if (time < pro[i].startTime) {
						for (j = 1; j <= len; j++) {
							if (pro[j].finish)
								continue;
							if (time >= pro[j].startTime)
								break;
						}
						if (j == len + 1)
							time = pro[i].startTime;
						else
							continue;
					}
					if (pro[i].runTime > pro[i].timeSlice) {
						printf("%d/%d/%d/%d/%d\n", loop++, pro[i].id, time, time + pro[i].timeSlice, pro[i].prioeity);
						time += pro[i].timeSlice;
						pro[i].runTime -= pro[i].timeSlice;
					}
					else {
						printf("%d/%d/%d/%d/%d\n", loop++, pro[i].id, time, time + pro[i].runTime, pro[i].prioeity);
						time += pro[i].runTime;
						pro[i].runTime = 0;
						pro[i].finish = true;
					}
				}
			}
			if (count == len)
				break;
		}
	}
	else {//动态优先级

	}
}
/*4
1/1/26/1/20
2/88/23/1/20
3/1/2/1/20
4/26/20/1/20
5/1/25/1/20*/

/*2
1/0/7/1/1
2/2/4/1/1
3/7/1/1/1
4/5/4/1/1
*/

/*2
1/0/7/1/1
2/2/4/1/1
3/4/1/1/1
4/5/4/1/1
*/

/*2
1/18/3/1/1
2/2/1/1/1
3/2/1/1/1
4/3/5/1/1
5/2/2/1/1*/
