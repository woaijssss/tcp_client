/*************************************************************************
    > File Name: include/device.h
    > Author: 范文瀚
    > Mail: wenhanf@163.com 
    > Created Time: 2017年01月10日 星期二 13时39分32秒
 ************************************************************************/

#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "handler.h"
#include <vector>
using namespace std;

class Device
{
public:
	Device() {}

	void addTimer(SrTimer& timer);

	void loop();

private:
	typedef std::vector<SrTimer*> _Timer;
	typedef _Timer::iterator _TimerIter;
	_Timer timers;//是个vector容器
};

#endif //__DEVICE_H__

