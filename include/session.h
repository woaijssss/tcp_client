/*
 * session.h
 *
 *  Created on: Nov 6, 2018
 *      Author: wenhan
 */

#ifndef INCLUDE_SESSION_H_
#define INCLUDE_SESSION_H_

#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <mutex>

#include "global.h"

using boost::asio::ip::tcp;
using boost::asio::deadline_timer;

class Session: public boost::enable_shared_from_this<Session> {
public:
        explicit Session(boost::asio::io_service& ioservice, const std::string& ip,
                        const int& port, std::string did, double send_seq, int num);

        ~Session();

        /* 获得连接客户端的socket */
        boost::asio::ip::tcp::socket* socket();

        void read();

        void write(GLOBAL::u8* w_buf, const GLOBAL::u32& len);

public:
        // 客户端模式
        GLOBAL::Bool connect();

        void setTimer();

private:

        /* 心跳timer触发回调 */
        void onTimer(const boost::system::error_code& error);

        /* tcp接收数据触发回调 */
        void onRead(const boost::system::error_code& error, const size_t& bytes);

        /* tcp数据发送触发回调 */
        void onWrite(const boost::system::error_code& error, const size_t& bytes);

        /* 保证线程安全的情况下，关闭socket */
        void toClose();

        /* 真正的关闭socket */
        void close();

        void reConnect();

private:
        boost::asio::io_service& _io_service;
        boost::asio::ip::tcp::socket* _sock;
        tcp::endpoint* _ep;
        boost::asio::deadline_timer _deadline;
        boost::asio::io_service::strand _strand;    // Provides serialised handler execution.

        /* socket缓存buffer，用于将socket缓存本地化 */
        GLOBAL::u8* _socketbuff_ptr;

        int _socketbuff_size;

        std::mutex _mutex;
        std::mutex _write_mutex;
        std::mutex _read_mutex;

        std::string _did;
        std::string _ip;
        int _port;
        double _send_seq;

        int _num;   // 序号
};

#endif /* INCLUDE_SESSION_H_ */
