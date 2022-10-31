/**
 * @file test.cpp
 * @brief
 * @author ziv
 * @email
 * @date 22-10-29.
 * @copyright Copyright (c) 2022年 ziv All rights reserved.
 */
#include <iostream>
#include "../src/log/logInclude.h"

int main(int argc, char **argv) {
    KAFKA::Logger::LoggerPtr logger(new KAFKA::Logger);
    logger->addAppender(KAFKA::LogAppender::LogAppenderPtr(new KAFKA::FileLogAppender("out.txt")));
    logger->addAppender(KAFKA::LogAppender::LogAppenderPtr(new KAFKA::StdoutLogAppender));

    //KAFKA::LogEvent::LogEventPtr event(new KAFKA::LogEvent(logger, KAFKA::LogLevel::DEBUG, __FILE__, __LINE__, 0, 1, 2, time(0), "main"));

//    logger->debug(event);
//    logger->fatal(event);

    KAFKA_LOG_DEBUG(logger) << "this is a main!";
    KAFKA_LOG_FATAL(logger) << "fatal error~";

    return 0;
}