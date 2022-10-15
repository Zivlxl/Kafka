/**
 * @file logInclude.h
 * @brief
 * @author ziv
 * @email
 * @date 2022/10/15.
 * @copyright Copyright (c) 2022年 ziv All rights reserved.
 */

#ifndef KAFKA_LOG_H
#define KAFKA_LOG_H

#include <string>
#include <stdint.h>
#include <stdarg.h>
#include <memory>
#include "../basic/basicDefine.h"

KAFKA_NAMESPACE_BEGIN

class Logger;
class LoggerManager;

/**
 * @brief log level
 */
class LogLevel {
public:
    /**
     * @brief
     */
    enum Level {UNKNOWN = 0, DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4, FATAL = 5};

    /**
     * @brief
     * @param level
     * @return
     */
    static const char * toString(LogLevel::level level);

    /**
     * @brief
     * @param str
     * @return
     */
    static LogLevel::Level fromString(const std::string & str);
};

/**
 * @brief
 */
class LogEvent {
public:
    using LogEventPtr = std::shared_ptr<LogEvent>;
    LogEvent();

    /**
     * @brief
     * @param logger
     * @param level
     * @param file
     * @param line
     * @param elapse
     * @param thread_id
     * @param fiber_id
     * @param time
     * @param thread_name
     */
    LogEvent(std::shared<Logger> logger, LogLevel::Level level,
             const char* file, int32_t line, uint32_t elapse,
             uint32_t thread_id, uint32_t fiber_id, uint64_t time,
             const std::string & thread_name);

    /**
     * @brief
     * @return
     */
    const char * getFile() const {return m_file;}

    /**
     * @brief
     * @return
     */
    int32_t getLine() const {return m_line;}

    /**
     * @brief
     * @return
     */
    uint32_t getElapse() const {return m_elapse;}

    /**
     * @brief
     * @return
     */
    uint32_t getThreadId() const {return m_threadId;}

    /**
     * @brief
     * @return
     */
    uint32_t getFiberId() const {return m_fiberId;}

    /**
     * @brief
     * @return
     */
    uint64_t getTime() const {return m_time;}

    /**
     * @brief
     * @return
     */
    const std::string& getThreadName() const {return m_threadName;}

    /**
     * @brief
     * @return
     */
    std::string getContent() const {return m_ss.str();}

    /**
     * @brief
     * @return
     */
    LogLevel::Level getLevel() const {return m_level;}

    /**
     * @brief
     * @return
     */
    std::stringstream& getSS() {return m_ss;}

    /**
     * @brief
     * @param fmt
     * @param ...
     */
    void format(const char * fmt, ...);

    /**
     * @brief
     * @param fmt
     * @param al
     */
    void format(const char * fmt, va_list al);

private:
    const char* m_file = nullptr;
    int32_t m_line = 0;
    uint32_t m_elapse = 0;
    uint32_t m_threadId = 0;
    uint32_t m_fiberId = 0;
    uint64_t m_time = 0;
    std::string m_threadName;
    std::stringstream m_ss;
    std::shared_ptr<Logger> m_logger;
    LogLevel::Level m_level;
};

class Logger {
public:
    Logger();

    /**
     * @brief
     * @param level
     * @param event
     */
    void log (Level level, const LogEvent & event);

private:

};

KAFKA_NAMESPACE_END
#endif //KAFKA_LOG_H
