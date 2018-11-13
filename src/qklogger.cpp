/*
 * qklogger.cpp
 *
 *  Created on: Sep 29, 2018
 *      Author: wenhan
 */

#include <string>
#include "qklogger.h"
#include <iostream>

using namespace std;

QkLogger::QkLogger(const std::string& name, const std::string& pattern,
        LoggerLevel level)
        : log4cplus::Logger(log4cplus::Logger::getInstance("")),
                _append(new log4cplus::ConsoleAppender()),
                // _append(new log4cplus::DailyRollingFileAppender(
                //                 name, log4cplus::HOURLY, true, 1)),
                _layout(new log4cplus::PatternLayout(pattern)),
                _level(level)
{
        this->init();
}

QkLogger::~QkLogger()
{

}

void QkLogger::init()
{
        _lvl = {
                {       TRACE, log4cplus::TRACE_LOG_LEVEL},
                {       DEBUG, log4cplus::DEBUG_LOG_LEVEL},
                {       INFO, log4cplus::INFO_LOG_LEVEL},
                {       WARN, log4cplus::WARN_LOG_LEVEL},
                {       ERROR, log4cplus::ERROR_LOG_LEVEL},
                {       FATAL, log4cplus::FATAL_LOG_LEVEL},
        };
        _append->setName("log_file");
        _append->setLayout(_layout);
        // 调用父类Logger的方法设置日志相关属性
        log4cplus::Logger::addAppender(_append);
}

void QkLogger::setLogLevel(LoggerLevel lvl)
{
        std::map<int, int>::iterator it = _lvl.find(lvl);
        if (it != _lvl.end()) {
                log4cplus::Logger::setLogLevel(_lvl[lvl]);
                _level = lvl;
        }
}

void QkLogger::log(LoggerLevel lvl, const std::string& msg)
{
        if (_level > lvl) return;

        const int level = _lvl[lvl];
        switch (level) {
        case log4cplus::TRACE_LOG_LEVEL:
                LOG4CPLUS_TRACE(*this, msg);
                break;
        case log4cplus::DEBUG_LOG_LEVEL:
                LOG4CPLUS_DEBUG(*this, msg);
                break;
        case log4cplus::INFO_LOG_LEVEL:
                LOG4CPLUS_INFO(*this, msg);
                break;
        case log4cplus::WARN_LOG_LEVEL:
                LOG4CPLUS_WARN(*this, msg);
                break;
        case log4cplus::ERROR_LOG_LEVEL:
                LOG4CPLUS_ERROR(*this, msg);
                break;
        case log4cplus::FATAL_LOG_LEVEL:
                LOG4CPLUS_FATAL(*this, msg);
                break;
        }
}

static QkLogger logger("./log/ssa.log");

void setLogLevel(LoggerLevel lvl)
{
        logger.setLogLevel(lvl);
}

void QkTrace(const std::string& msg)
{
        logger.log(TRACE, msg);
}

void QkDebug(const std::string& msg)
{
        logger.log(DEBUG, msg);
}

void QkInfo(const std::string& msg)
{
        logger.log(INFO, msg);
}

void QkWarn(const std::string& msg)
{
        logger.log(WARN, msg);
}

void QkError(const std::string& msg)
{
        logger.log(ERROR, msg);
}

void QkFatal(const std::string& msg)
{
        logger.log(FATAL, msg);
}
