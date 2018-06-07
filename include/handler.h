/*************************************************************************
    > File Name: include/handler.h
    > Author: 范文瀚
    > Mail: wenhanf@163.com
    > Created Time: 2017年01月06日 星期五 14时44分00秒
 ************************************************************************/

#ifndef __HANDLER_H__
#define __HANDLER_H__

#include "client.h"

class TalkToServer;

class Handler : public TimerHandler
{
	typedef boost::shared_ptr<TalkToServer> session_ptr;
public:
	Handler(boost::asio::io_service& io_service, tcp::endpoint& ep);

	void CreateThread(void);

	virtual void operator()(SrTimer &timer);

private:
	boost::asio::io_service& _io_service;
	vector<session_ptr> v_ptr;
};

#endif //__HANDLER_H__
