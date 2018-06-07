
/*************************************************************************
    > File Name: client.h
    > Author: 范文瀚
    > Mail: wenhanf@163.com
    > Created Time: 2016年12月29日 星期四 10时58分36秒
 ************************************************************************/

#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <string>
#include <stdint.h>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "timer.h"

using namespace std;
using boost::asio::ip::tcp;
using boost::asio::ip::address;

#pragma   pack(push, 1)

struct Header {
        //消息表示符：0xABCD：测量值信息(struct Body)；0xDCBA：控制信息(struct Operation)；
        uint32_t flag;
        uint32_t length;    //对应结构的整体长度
};

struct Body {
        uint8_t pos_status; //定位状态
        uint32_t lat;   //纬度
        uint32_t lng;   //经度
        uint32_t mileage;   //里程
        uint32_t battery_remain;    //剩余电量
        uint32_t rotating_speed;    //驱动电机转速
        uint32_t vehicle_speed;     //车速
        uint32_t voltage;                //电压值
};

struct Data {
        struct Header hdr;
        uint32_t device_id;    //设备id
        struct Body bd; //测量值结构
};

struct Operation {
        struct Header hdr;
        uint8_t ret;            // 0：执行失败; 1：执行成功
        uint8_t type;           // 1：喇叭; 2：车灯; 3：车锁
        uint8_t action;          // 0：关闭（不使用）；1：打开（使用）
};

#pragma pack(pop)

#define HEADER_LEN 9

class SrTimer;


class TalkToServer : public boost::enable_shared_from_this<TalkToServer>
{
public:
	/* 创建客户端的构造函数
	 */
        TalkToServer(boost::asio::io_service* io_service, unsigned int& num, tcp::endpoint ep, string did);
	/* 析构函数
	 */
	~TalkToServer();

public:
	/* 客户端连接到服务器
	 */
	void connect(tcp::endpoint ep);		//连接到服务器
	/* 判断是否连接成功
	 */
	void on_connected(const boost::system::error_code& error);

public:
	/* 开始读写操作
	 */
	void start();

	/* 发送数据操作
	 */
        void handle_write(const boost::system::error_code& error, const size_t& bytes_transferred);

	/* 异步写操作
	 */
        void regDev();
	void start_write();

    void OnRead(const boost::system::error_code& error,
                const size_t& bytes_transferred);

private:
	char* _data;		//始终指向首位置
	char* _data_pos;	//移动的指针，指向当前位置
	char* _data_cpy;	//指向拷贝的起始位置

private:
	int _flag;

	string _str;
	tcp::socket _sock;
	unsigned int _device_id;
    tcp::endpoint& _ep;
        string _did;
};

#endif //__CLIENT_H__
