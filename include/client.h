/*
 * client.h
 *
 *  Created on: Nov 6, 2018
 *      Author: wenhan
 */

#ifndef INCLUDE_CLIENT_H_
#define INCLUDE_CLIENT_H_

#include <string>
#include <thread>

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;

class ThreadPool {
public:
        /* 创建size个线程 */
        explicit ThreadPool(int size);

        ~ThreadPool();

        /* 将新任务项添加到io_service池中。 */
        template<class F>
        void Enqueue(F f);

        /* 获取一个io_service */
        boost::asio::io_service& getIoService();
private:
        boost::thread_group _workers;
        boost::asio::io_service _io_service;
        boost::asio::io_service::work _work;
};

class Session;
class Client {
        typedef boost::shared_ptr<Session> sessionor;
public:
        Client(std::string ip, int port, int size, double send_seq);

        ~Client();

        void start();


private:
        ThreadPool _tp;
        int _mode;
        int _port;
        int _timeout;
        std::string _ip;
        double _send_seq;
};

#endif /* INCLUDE_CLIENT_H_ */
