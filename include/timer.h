#ifndef TIMER_H
#define TIMER_H
#include <utility>
#include <time.h>
#include "srtypes.h"

class SrTimer;


bool operator<=(const timespec &l, const timespec &r);


/**
 *  \class TimerHandler
 *  \brief Virtual abstract functor for the SrTimer callback interface.
 */
class TimerHandler
{
public:
        virtual ~TimerHandler() {}
        /**
         *  \brief SrTimer callback interface.
         *  \param timer reference to the SrTimer which fires the callback.
         *  \param agent reference to the SrAgent instance.
         */
        virtual void operator()(SrTimer &timer) = 0;
};

typedef TimerHandler AbstractTimerFunctor;


/**
 *  \class SrTimer
	简单的周期性毫秒计时器分辨率。
 *  \brief A periodical timer with millisecond resolution.
 */
class SrTimer
{
public:
	/**
	 *  \brief SrTimer constructor.
	 *
			首先构造时注意计时器是不活跃的。
			param毫秒计时器时间,以毫秒为单位。
			参数调函子当定时器触发执行。
			millisec:毫秒
			callback:回调
	 *  \note the timer is inactive when first constructed.
	 *  \param millisec timer period in milliseconds.
	 *  \param callback functor to be executed when the timer fires.
	 */
	SrTimer(int millisec, TimerHandler *callback = NULL):
		cb(callback), val(millisec), active(false) {}
	virtual ~SrTimer() {}

	/**
			检查计时器是否活跃。
	 *  \brief Check if the timer is active.
	 *  \return true if active, false otherwise.
	 */
	bool isActive() const {
		return active;
	}
	/**
	 *  \brief Get the current period of the timer.
	 *  \return the current timer period in milliseconds.
	 */
	int interval() const {
		return val;
	}
	/**
	 *  \brief Get the schedule time of the timer.
	 *  \return the schedule time, undefined if timer is inactive.
	 */
	const timespec &shedTime() const {
		return beg;
	}
	/**
	 *  \brief Get the fire time of the timer.
	 *  \return the schedule time, undefined if timer is inactive.
	 */
	const timespec &fireTime() const {
		return end;
	}
	/**
	 *  \brief Run the connected callback if not NULL.
			param代理SrAgent实例的引用。
	 *  \param agent reference to the SrAgent instance.
	 */
	//在这里，实现了调用CPUMeasurement类中的对括号重载的()函数
	void run() {
		if (cb) (*cb)(*this);
	}
	/**
	 *  \brief Set the period to millisec for the timer.
	 *
	 *  \note This function does not activate the timer.
	 *  \note Set a negative interval causes undefined behavior.
	 *
	 *  \param millisec the new period in milliseconds.
	 */
	void setInterval(int millisec) {
		val = millisec;
	}
	/**
	 *  \brief Connect callback functor to the timer.	简单回调函子连接到计时器。
	 *
			函子叫做当定时器触发。旧的回调是覆盖。调用这个函数与零重置回调。
	 *  The functor is called when the timer is fired. The old callback is
	 *  overwritten if any. Calling this function with NULL resets the
	 *  callback.
	 *
	 *  \param functor A subclass of TimerHandler.
	 */
	void connect(TimerHandler *functor) {
		cb = functor;
	}
	/**
	 *  \brief Start the timer.
	 *
	 *  This function activates the timer, sets the schedule time to now,
	 *  and the fire time according to the current period.
	 */
	void start() {
		clock_gettime(CLOCK_MONOTONIC, &beg);
		end.tv_sec = beg.tv_sec + val / 1000;
		end.tv_nsec = beg.tv_nsec + (val % 1000) * 1000000;
		active = true;
	}
	/**
	 *  \brief Stop the timer. Sets the timer to inactive.
	 */
	void stop() {
		active = false;
	}

private:
	TimerHandler *cb;
	timespec beg;
	timespec end;
	int val;
	bool active;
};

#endif /* SRTIMER_H */
