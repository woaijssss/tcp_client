/*
 * session.cpp
 *
 *  Created on: Nov 6, 2018
 *      Author: wenhan
 */

#include <string>

#include <time.h>
#include <stdlib.h>

#include "session.h"
#include "qklogger.h"
#include "util.h"

using boost::asio::ip::address;
using boost::asio::ip::tcp;
using namespace GLOBAL;

using namespace std;

extern int device_count;

static int rando()
{
        time_t tm;
        srand(time(&tm));
        int r = rand() % device_count;

        return r;
}

Session::Session(boost::asio::io_service& ioservice, const string& ip, const int& port, string did,
                double send_seq, int num)
                : _io_service(ioservice), _sock(new tcp::socket(_io_service)), _ep(
                                new tcp::endpoint(address::from_string(ip), port)), _deadline(
                                _io_service, boost::posix_time::seconds(send_seq)), _strand(
                                _io_service), _did(did), _ip(ip), _port(port), _send_seq(send_seq), _num(
                                num)
{
        _socketbuff_ptr = new u8[1024];
        _socketbuff_size = 1024;
}

Session::~Session()
{
        QkInfo(_did + " disconnect...");
        if (_socketbuff_ptr) {
                delete[] _socketbuff_ptr;
                _socketbuff_ptr = NULL;
                _deadline.cancel();
        }
}

/* 获得连接客户端的socket */
tcp::socket* Session::socket()
{
        return _sock;
}

void Session::read()
{
        _read_mutex.lock();
        _sock->async_read_some(boost::asio::buffer(_socketbuff_ptr, _socketbuff_size),
                        boost::bind(&Session::onRead, shared_from_this(),
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
        _read_mutex.unlock();
}

void Session::write(u8* w_buf, const u32& len)
{
        _write_mutex.lock();
        //QkInfo(getHexString(w_buf, len));
        boost::asio::async_write(*_sock, boost::asio::buffer(w_buf, len),
                        boost::bind(&Session::onWrite, shared_from_this(),
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
        _write_mutex.unlock();
}

Bool Session::connect()
{
        Bool ret = FALSE;
        int delay = 2;
        while (TRUE) {
                boost::system::error_code ec;
                _sock->connect(*_ep, ec);
                if (ec) {        // 连接出错处理：间隔重连
                        QkInfo("error: failed to connect remote: " + ec.message());
                        sleep(delay);
                        if (delay > 600) delay = 1;
                        delay *= 2;
                } else {
                        QkInfo("info: connection has been established");
                        ret = TRUE;
                        break;
                }
        }
        return ret;
}

void Session::setTimer()
{
        _deadline.cancel();
        // 设置recv 超时时间，此处会触发 check_deadline
        _deadline.expires_from_now(boost::posix_time::seconds(_send_seq));
        _deadline.async_wait(
                        boost::bind(&Session::onTimer, shared_from_this(),
                                        boost::asio::placeholders::error));
}

void Session::reConnect()
{
        boost::system::error_code ec;
        if (_sock->is_open()) {
                _sock->cancel(ec);
                _sock->shutdown(tcp::socket::shutdown_both);
                _sock->close();

                _sock = new tcp::socket(_io_service);
                _ep = new tcp::endpoint(address::from_string(_ip), _port);

                memset(this->_socketbuff_ptr, 0, _socketbuff_size); // 每次处理后，清空socket数据缓存

                QkDebug(_did + " reconnect");
                this->connect();
                this->setTimer();
                this->read();

                string id = "ETUNG:" + _did;
                this->write((u8*)id.c_str(), id.size() + 1);
//                unsigned char p[] = {0x01, 0x94, 0x35, 0x77, 0xe5, 0x3e, 0x01, 0x00, 0x02,
//                                0x01, 0x05, 0x55, 0x0c, 0x00, 0x01, 0x00, 0x03, 0x00, 0xb7,
//                                0xc4, 0x58, 0x23};
//                this->write(p, sizeof(p));
        }
}

/* 心跳timer触发回调 */
void Session::onTimer(const boost::system::error_code& error)
{
        _mutex.lock();
        if (!error) {
                int r = rando();
                int begin = r - 5;
                int end = r + 5;
                if (begin <= _num && _num <= end) {
                        //if (0) {
                        this->reConnect();
                } else {
                unsigned char p1[] = {
                        0x01, 0x94, 0x35, 0x77, 0xe5, 0x3e, 0x01, 0x00, 0x02, 0x01, 0x05, 0x55, 0x0c, 0x00, 0x01, 0x00, 0x03,
                        0x00, 0xb7, 0xc4, 0x58, 0x23, 0x9f, 0x55, 0x0d, 0x00, 0x01, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x55, 0x0e, 0x00, 0x01, 0x00, 0x03, 0x06, 0xf8, 0x60, 0xb2, 0xd0, 0x6a, 0x55, 0x0a, 0x00, 0x64,
                        0x00, 0x03, 0x7d, 0x13, 0xa7, 0x13, 0x66, 0x13, 0x50, 0xd4,
                        0x01, 0x94, 0x35, 0x77, 0xe5, 0x3e, 0x01, 0x00, 0x02, 0x01, 0x05, 0x55, 0x0c, 0x00, 0x01, 0x00, 0x03,
                        0xc3, 0x0e, 0x21, 0x1c, 0xdb, 0x17, 0x55, 0x0d, 0x00, 0x01, 0x00, 0x03, 0xaa, 0x04, 0x00, 0x00, 0x42,
                        0x14, 0x55, 0x0e, 0x00, 0x01, 0x00, 0x03, 0x7b, 0x0f, 0x29, 0x1f, 0x4d, 0x1f, 0x55, 0x0a, 0x00, 0x64,
                        0x00, 0x03, 0x92, 0x13, 0x95, 0x13, 0x61, 0x13, 0xb1, 0xf3,


                        0x01, 0x94, 0x35, 0x77, 0xd7, 0x42, 0x01, 0x00, 0x02, 0x01, 0x05, 0x55, 0x0c, 0x00, 0x64, 0x00, 0x03,
                        0x93, 0x71, 0x58, 0xef, 0x5c, 0x51, 0x55, 0x0d, 0x00, 0x64, 0x00, 0x03, 0x00, 0x00, 0x46, 0x01, 0x00,
                        0x00, 0x55, 0x0e, 0x00, 0x64, 0x00, 0x03, 0x60, 0x06, 0x50, 0x98, 0xed, 0x90, 0x55, 0x0a, 0x00, 0x64,
                        0x00, 0x03, 0x95, 0x13, 0xaa, 0x13, 0x44, 0x13, 0xc6, 0xf6,

                        0x01, 0x94, 0x35, 0x77, 0xd7, 0x42, 0x01, 0x00, 0x02, 0x01, 0x05, 0x55, 0x06, 0x00, 0x64, 0x00, 0x03,
                        0x04, 0x56, 0xdf, 0x56, 0x1b, 0x56, 0x55, 0x07, 0x00, 0xe8, 0x03, 0x03, 0x2c, 0x10, 0xb9, 0x07, 0x8a,
                        0x07, 0x55, 0x08, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x55, 0x09, 0x00, 0x64, 0x00, 0x04, 0x38, 0x09,
                        0xec, 0x08, 0x13, 0x09, 0xe8, 0x08, 0xc0, 0xce

                };

                this->write(p1, sizeof(p1));

                this->setTimer();
        }
}

_mutex.unlock();
}

/* tcp接收数据触发回调 */
void Session::onRead(const boost::system::error_code& error, const size_t& bytes)
{
if (error) {
        QkWarn("read error and socket close");
        //toClose();
} else {

        memset(this->_socketbuff_ptr, 0, _socketbuff_size);   // 每次处理后，清空socket数据缓存
        this->read();
}
}

/* tcp数据发送触发回调 */
void Session::onWrite(const boost::system::error_code& error, const size_t& bytes)
{
if (error) {
        QkWarn("write error and socket close");
        //this->toClose();
} else {
        QkInfo("write sucessful|" + to_string(bytes) + " bytes");
}
}

/* 保证线程安全的情况下，关闭socket */
void Session::toClose()
{
_strand.dispatch(bind(&Session::close, shared_from_this()));
}

/* 真正的关闭socket */
void Session::close()
{
_mutex.lock();
if (_sock->is_open()) {
        boost::system::error_code ec;
        _sock->cancel(ec);
        if (!ec) {
                QkInfo("socket close: " + _did);
                _sock->shutdown(tcp::socket::shutdown_both, ec);
                _sock->close(ec);
                /*
                 如果这里不取消定时器，
                 则该ioservice服务还存在异步事件(异步定时)，
                 该session指针不会被析构
                 */
                _deadline.cancel();
        }
}
_mutex.unlock();
}

