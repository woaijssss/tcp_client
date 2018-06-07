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

void Random::getRandCurrent(double& p)//��������Ϊ����n��0-1����������洢������p�С�
{
    #define N 99 //��λС����
    srand(time(NULL));//������������ӣ�ʹÿ�λ�ȡ��������в�ͬ��
        p = rand()%(N+1)/(double)(N+1);//����0-1����������
        //p += rand()%(N+1) * 3;//����0-1����������
}

void Random::getRandVoltage(double& p)//��������Ϊ����n��0-1����������洢������p�С�
{
    #define N 99 //��λС����
    srand(time(NULL));//������������ӣ�ʹÿ�λ�ȡ��������в�ͬ��
        p = rand()%(N+1)/(double)(N+1);//����0-1����������
        p += rand()%(N+1) * 3;//����0-1����������
}