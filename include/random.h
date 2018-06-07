/*************************************************************************
    > File Name: random.h
    > Author: 范文瀚
    > Mail: wenhanf@163.com
    > Created Time: 2016年12月23日 星期五 13时30分10秒
 ************************************************************************/

#ifndef __RANDOM_H__
#define __RANDOM_H__

class Random
{
public:
	Random() {}
	void random(char* buff, int div);
	void getRandCurrent(double& p);
	void getRandVoltage(double& p);
};

#endif //__RANDOM_H__

