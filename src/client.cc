/*************************************************************************
    > File Name: client.h
    > Author: 范文瀚
    > Mail: wenhanf@163.com
    > Created Time: 2016年12月29日 星期四 10时58分36秒
 ************************************************************************/


#include "client.h"
#include "cJSON.h"

#include <pthread.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <mutex>

using namespace std;
using boost::asio::ip::tcp;
using boost::asio::ip::address;

#define MAXSIZE 1024

std::mutex _mutex;


TalkToServer::TalkToServer(boost::asio::io_service* io_service, unsigned int& num, tcp::endpoint ep, string did)
        : _sock(*io_service), _device_id(num), _ep(ep), _did(did)
{
	_flag = 0;
	//cout << "------>client name: " << client_name << endl;
	_data = new char[MAXSIZE];
	_data_pos = _data;
	_data_cpy = _data;
}

TalkToServer::~TalkToServer()
{
        cerr << "delete " << endl;
	delete _data;
}

void TalkToServer::connect(tcp::endpoint ep)  		//连接到服务器
{
	_sock.async_connect(ep, boost::bind(&TalkToServer::on_connected, shared_from_this(), boost::asio::placeholders::error));
}

void TalkToServer::on_connected(const boost::system::error_code& error)
{
	if (error == 0) {
		// 连接成功，打印消息，
		cerr << "info: connection has been established" << endl;
        regDev();
        //start_write();
	} else {
		cout << "error: failed to connect remote: " << error << endl;
		exit(-1);
	}
}

void TalkToServer::regDev()
{
        // char buff[] = {0x45, 0x54, 0x55, 0x4e, 0x47, 0x3a, 0x39,
        //                0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        //                0x30, 0x30, 0x30, 0x33, 0x39, 0x37, 0x30, 0x00};
        cerr << "_did: " << _did << endl;
        _sock.async_write_some(
                boost::asio::buffer(_did.c_str(), _did.size()),
                boost::bind(&TalkToServer::handle_write,
                            shared_from_this(),
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));

        _sock.async_read_some(boost::asio::buffer(_data,
                                                  sizeof(struct Operation)),
                              boost::bind(&TalkToServer::OnRead
                                          , shared_from_this(),
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
}

void TalkToServer::start_write()
{
        unsigned char measurement[] = {
                0x01, 0x94, 0x35, 0x77, 0xd7, 0x42, 0x01, 0x00, 0x02, 0x01, 0x05,
                0x55, 0x06, 0x00, 0x64, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x55, 0x07, 0x00, 0xe8, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x55, 0x08, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x55,
                0x09, 0x00, 0x64, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x82, 0x5a
        };
        _sock.async_write_some(
                boost::asio::buffer(measurement, sizeof(measurement)),
                boost::bind(&TalkToServer::handle_write,
                            shared_from_this(),
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));

        _sock.async_read_some(boost::asio::buffer(_data,
                                                  sizeof(struct Operation)),
                              boost::bind(&TalkToServer::OnRead
                                          , shared_from_this(),
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
}

void TalkToServer::OnRead(const boost::system::error_code& error,
                          const size_t& bytes_transferred)
{
    if (error) {
        cerr << "server error and client being closed!" << endl;
        return;
    }
    printf("OnRead: %s\n", _data);
    memset(_data, 0, MAXSIZE);
}

void TalkToServer::handle_write(const boost::system::error_code& error, const size_t& bytes_transferred)
{
	if (!error) {
		cerr << "send success" << endl;
    } else {
		cerr << "write error" << endl;
		_sock.close();
	}
}
