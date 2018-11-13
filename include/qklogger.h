/*
 * qklogger.h
 *
 *  Created on: Sep 29, 2018
 *      Author: wenhan
 */

#ifndef QKLOGGER_H_
#define QKLOGGER_H_

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>   // 日志输出宏
#include <log4cplus/consoleappender.h> // 将日志输出到控制台
#include <log4cplus/fileappender.h> // 将日志输出到文件
#include <log4cplus/layout.h>       // 输出格式控制
#include <log4cplus/loglevel.h>
#include <iomanip>
#include <unistd.h>
#include <memory>
#include <iostream>
#include <map>
enum LoggerLevel {
        TRACE = 0,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
};
class QkLogger : public log4cplus::Logger {
public:
        QkLogger(const std::string& name,
                  const std::string& pattern="%D{20%y-%m-%d %H:%M:%S} %p: %m%n",
                  LoggerLevel level = WARN);
        virtual ~QkLogger();
public:
        void init();

        /* 设置日志级别，可由用户调用设置_level值；
         * _level值默认为 WARN 级别。
         */
        void setLogLevel(LoggerLevel lvl);

        /* 日志输出
         * 若lvl值小于_level值，则日志不会输出。
         */
        void log(LoggerLevel lvl, const std::string& msg);
private:
        std::string _pattern;
        log4cplus::helpers::SharedObjectPtr<log4cplus::Appender> _append;
        std::auto_ptr<log4cplus::Layout> _layout;
        LoggerLevel _level;
        std::map<int, int> _lvl;
};
void setLogLevel(LoggerLevel lvl);
void QkTrace(const std::string& msg);
void QkDebug(const std::string& msg);
void QkInfo(const std::string& msg);
void QkWarn(const std::string& msg);
void QkError(const std::string& msg);
void QkFatal(const std::string& msg);

#endif /* QKLOGGER_H_ */
