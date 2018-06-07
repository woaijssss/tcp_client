#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <iconv.h>
#include <cstdlib>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <thread>
#include <pthread.h>
#include <deque>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

#include <string.h>
#include <boost/asio.hpp>

#include "client.h"
#include "device.h"

typedef boost::shared_ptr<SrTimer> timer_ptr;

int main()
{
	//创建io_service服务
	boost::asio::io_service io_service;
	//创建tcp连接节点
	tcp::endpoint ep(address::from_string("127.0.0.1"), 20001);
	//开始开启客户端
	Device device;
	Handler handler(io_service, ep);

	SrTimer handler_timer(5 * 1000, &handler);	//&tcphandler 回调指针
	device.addTimer(handler_timer);	//添加对tcphandler的定时器
	handler_timer.start();		//开始定时器

	device.loop();

	return 0;
}
