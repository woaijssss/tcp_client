/*
 * client.cpp
 *
 *  Created on: Nov 6, 2018
 *      Author: wenhan
 */

#include <string>

#include "client.h"
#include "session.h"
#include "qklogger.h"
#include "global.h"

using namespace std;
using namespace GLOBAL;

extern int device_count;

ThreadPool::ThreadPool(int size)
                : _io_service(size), _work(_io_service)
{
        for (int i = 0; i < size; ++i) {
                _workers.create_thread(boost::bind(&boost::asio::io_service::run, &_io_service));
        }
}

ThreadPool::~ThreadPool()
{
        QkDebug("------");
        _io_service.stop();
        _workers.join_all();
}

//Add new work item to the pool.
template<class F>
void ThreadPool::Enqueue(F f)
{
        _io_service.post(f);
}

boost::asio::io_service& ThreadPool::getIoService()
{
        return _work.get_io_service();
}

Client::Client(string ip, int port, int size, double send_seq)
                : _tp(size), _port(port), _ip(ip), _send_seq(send_seq)
{
        this->start();
}

Client::~Client()
{

}

void Client::start()
{
        long did = 900000000000000;
        for (int i = 0; i < device_count; i++) {
                sessionor sess(
                                new Session(_tp.getIoService(), _ip, _port, to_string(did + i),
                                                _send_seq, i));
                if (!sess->connect()) {
                        QkDebug("Connect to remoter failed...");
                } else {    // 连接成功
                        QkDebug(to_string(did + i) + " connect established...");
                        sess->setTimer();
                        sess->read();

                        string id = "ETUNG:" + to_string(did+i);
                        sess->write((u8*)id.c_str(), id.size()+1);
//                        unsigned char p[] = {0x01, 0x94, 0x35, 0x77, 0xe5, 0x3e, 0x01, 0x00, 0x02,
//                                        0x01, 0x05, 0x55, 0x0c, 0x00, 0x01, 0x00, 0x03, 0x00, 0xb7,
//                                        0xc4, 0x58, 0x23};
//                        sess->write(p, sizeof(p));
                }
        }
}

