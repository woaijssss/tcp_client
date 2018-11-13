/*
 * main.cpp
 *
 *  Created on: Nov 6, 2018
 *      Author: wenhan
 */

#include <iostream>

#include "client.h"
#include "qklogger.h"

using namespace std;

int device_count = 10000;

int main(void)
{
        setLogLevel(DEBUG);

        double send_seq = 1.0;   // 发送频率
        string ip = "192.168.0.132";    // 连接ip
        int port = 20001;
        int thread_poll_size = 200;
        Client c(ip, port, thread_poll_size, send_seq);

        while (true) {

        }

        return 0;
}

