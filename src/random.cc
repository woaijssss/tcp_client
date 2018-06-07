#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include "random.h"

void Random::random(char* buff, int div)
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	unsigned long us = (tv.tv_sec%div + tv.tv_usec) % div;
	sprintf(buff, "%ld", us);
}

void Random::getRandCurrent(double& p)//函数功能为产生n个0-1的随机数，存储于数组p中。
{
    #define N 99 //三位小数。
    srand(time(NULL));//设置随机数种子，使每次获取的随机序列不同。
        p = rand()%(N+1)/(double)(N+1);//生成0-1间的随机数。
        //p += rand()%(N+1) * 3;//生成0-1间的随机数。
}

void Random::getRandVoltage(double& p)//函数功能为产生n个0-1的随机数，存储于数组p中。
{
    #define N 99 //三位小数。
    srand(time(NULL));//设置随机数种子，使每次获取的随机序列不同。
        p = rand()%(N+1)/(double)(N+1);//生成0-1间的随机数。
        p += rand()%(N+1) * 3;//生成0-1间的随机数。
}