#include "handler.h"
#include <iostream>
#include <string.h>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>

using namespace std;
using boost::asio::ip::tcp;
using boost::asio::ip::address;

Handler::Handler(boost::asio::io_service& io_service, tcp::endpoint& ep)
        :_io_service(io_service)
{
        try {
                char buff[] = {0x45, 0x54, 0x55, 0x4e, 0x47, 0x3a, 0x39,
                               0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
                               0x30, 0x30, 0x30, 0x33, 0x39, 0x37, 0x30, 0x00
                };
                printf("%s\n", buff);
                string d = "9000000000010000";
                for (unsigned int i = 0; i < 1; ++i) {
                        d = to_string(atol(d.c_str()) + 1);
                        d = "0000000000000000";
                        string did = "ETUNG:" + d;

                        string device_id = "";
                        for (int j = 0; j < did.size(); j++){
                                char temp[4] = {0};
                                snprintf(temp, 2, "%c", did[j]);
                                device_id += string(temp);
                        }


                        cerr << "did: " << device_id << endl;
                        usleep(100);
                        unsigned int n = i+1;
                        session_ptr new_session(new TalkToServer(&_io_service, n, ep, device_id));
                        new_session->connect(ep);
                        //new_session->start();
                        //new_session->start_write();
                        v_ptr.push_back(new_session);
                }
                boost::thread thrd(boost::bind(&Handler::CreateThread, this));
                thrd.timed_join(boost::posix_time::seconds(0));		//主线程以非阻塞方式等待线程
        } catch (boost::system::system_error& err) {
                cout << "client terminated" << endl;
                exit(-1);
        }
}

void Handler::CreateThread(void)
{
        _io_service.run();
}


void Handler::operator()(SrTimer &timer)
{
        vector<session_ptr>::iterator it;
        for (it = v_ptr.begin(); it != v_ptr.end(); ++it) {
                (*it)->start_write();
        }
}
