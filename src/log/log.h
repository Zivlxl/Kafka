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
#include <fstream>
#include <vector>
#include <memory>
#include "../basic/basicDefine.h"

KAFKA_NAMESPACE_BEGIN

class Logger;
class LoggerManager;

/**
 * @brief 日志级别
 */
class LogLevel {
public:
    /**
     * @brief 日志级别枚举
     */
    enum Level {
        UNKNOWN = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5};

    /**
     * @brief 将日志级别转换成字符串输出
     * @param level
     * @return
     */
    static const char * toString(LogLevel::level level);

    /**
     * @brief 将文本转换成日志格式级别
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
    //行号
    int32_t m_line = 0;
    //程序启动到现在的毫秒数
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
    typedef std::shared_ptr<Logger> LoggerPtr;
    Logger();

    /**
     * @brief
     * @param level
     * @param event
     */
    void log (Level level, const LogEvent & event);

private:

};

class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> LogFormatterPtr;

    LogFormatter(const std::string &pattern);

    std::string format(Logger::LoggerPtr logger, LogLevel::Level level, LogEvent::LogEventPtr event);

    std::ostream& format(std::ostream& ofs, Logger::LoggerPtr logger, LogLevel::Level level, LogEvent::LogEventPtr event);

public:
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> FormatItemPtr;

        /**
         * @brief
         */
        virtual ~FormatItem() = default;

        /**
         * @brief
         * @param os 输出流
         * @param logger 日志
         * @param level
         * @param event
         */
        virtual void format(std::ostream& os, Logger::LoggerPtr logger, LogLevel::Level level, LogEvent::LogEventPtr event) = 0;
    };

    void init();

    bool isError() const {return m_error;}

    const std::string getPattern() const {return m_pattern;}
private:
    std::string m_pattern;
    bool m_error = false;
    std::vector<FormatItem::FormatItemPtr> m_items;

};

class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> LogAppenderPtr;

    /**
     * @brief
     * @param level
     * @param event
     */
    virtual void log(Logger::LoggerPtr logger, LogLevel::level level, LogEvent::LogEventPtr event) = 0;

    /**
     * @brief
     */
    virtual ~LogAppender() = default;

    virtual std::string toYamlString() = 0;

    LogFormatter::LogFormatterPtr getFormatter() const;

    void setFormatter(LogFormatter::LogFormatterPtr formatter);

    /**
     * @brief
     * @return
     */
    LogLevel::Level getLevel() const {return m_level;}

    /**
     * @brief
     * @param level
     */
    void setLevel(LogLevel::Level level) {m_level = level;}

    /**
     * @brief
     * @return
     */
    virtual std::string toYamlString() = 0;


protected:
    LogLevel::Level m_level = LogLevel::DEBUG;
    bool m_hasFormatter = false;

    LogFormatter::LogFormatterPtr m_formatter;
};

class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> StdoutLogAppenderPtr;

    void log(Logger::LoggerPtr logger, LogLevel::Level level, LogEvent::LogEventPtr event) override;

    std::string toYamlString() override;

};

class FileLogAppender : public LogAppender{
public:
    typedef std::shared_ptr<FileLogAppender> FileLogAppender;

    /**
     * @brief
     * @param filename
     */
    FileLogAppender(const std::string &filename);

    void log(Logger::LoggerPtr logger, LogLevel::Level level, LogEvent::LogEventPtr event) override;

    std::string toYamlString() override;

    bool reopen();

private:
    std::string m_filename;
    std::ofstream m_filestream;
    uint64_t m_lastTime = 0;
};

KAFKA_NAMESPACE_END
#endif //KAFKA_LOG_H
