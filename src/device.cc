#include "device.h"

void Device::addTimer(SrTimer& timer)
{
	timers.push_back(&timer);
}

void Device::loop()
{
	const timespec ts = {5/1000, (5%1000) * 1000000};
	while (true) {
		clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL);
		for (auto &i: timers) {
			timespec now;
			clock_gettime(CLOCK_MONOTONIC, &now);
			if (i->isActive() && i->fireTime() <= now) {
				i->run();		//这里触发各个类中的()重载函数
				if (i->isActive())
					i->start();
			}
		}
	}
}
