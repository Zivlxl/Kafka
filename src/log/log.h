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
#include <unordered_map>
#include <memory>
#include <list>
#include <sstream>
#include "../basic/basicDefine.h"
#include "../basic/singleton.h"

/**
 * @brief
 */
#define KAFKA_LOG_LEVEL(logger, level) \
    if (logger->getLevel() <= level)  \
        KAFKA::LogEventWrap(KAFKA::LogEvent::LogEventPtr(new KAFKA::LogEvent(logger, level, \
                            __FILE__, __LINE__, 0, 1, \
                            2, time(0), "main"))).getSS()

/**
 * @brief
 */
#define KAFKA_LOG_DEBUG(logger) KAFKA_LOG_LEVEL(logger, KAFKA::LogLevel::DEBUG)

/**
 * @brief
 */
#define KAFKA_LOG_INFO(logger) KAFKA_LOG_LEVEL(logger, KAFKA::LogLevel::INFO)

/**
 * @brief
 */
#define KAFKA_LOG_WARN(logger) KAFKA_LOG_LEVEL(logger, KAFKA::LogLevel::WARN)

/**
 * @brief
 */
#define KAFKA_LOG_ERROR(logger) KAFKA_LOG_LEVEL(logger, KAFKA::LogLevel::ERROR)

/**
 * @brief
 */
#define KAFKA_LOG_FATAL(logger) KAFKA_LOG_LEVEL(logger, KAFKA::LogLevel::FATAL)

/**
 * @brief 使用格式化方式将日志级别level的日志写入到logger中
 */

#define KAFKA_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if (logger->getLevel() <= level)  \
        KAFKA::LogEventWrap(KAFKA::LogEvent::LogEventPtr(new KAFKA::LogEvent(logger, level, \
                            __FILE__, __LINE__, 0, 1, \
                            2, time(0), "main"))).getEvent()->format(fmt, __VA_ARGS__)

/**
 * @brief
 */
#define KAFKA_LOG_FMT_DEBUG(logger, fmt, ...) KAFKA_LOG_FMT_LEVEL(logger, KAFKA::LogLevel::DEBUG, fmt, __VA_ARGS__)

/**
 * @brief
 */
#define KAFKA_LOG_FMT_INFO(logger, fmt, ...) KAFKA_LOG_FMT_LEVEL(logger, KAFKA::LogLevel::INFO, fmt, __VA_ARGS__)

/**
 * @brief
 */
#define KAFKA_LOG_FMT_WARN(logger, fmt, ...) KAFKA_LOG_FMT_LEVEL(logger, KAFKA::LogLevel::WARN, fmt, __VA_ARGS__)

/**
 * @brief
 */
#define KAFKA_LOG_FMT_ERROR(logger, fmt, ...) KAFKA_LOG_FMT_LEVEL(logger, KAFKA::LogLevel::ERROR, fmt, __VA_ARGS__)

/**
 * @brief
 */
#define KAFKA_LOG_FMT_FATAL(logger, fmt, ...) KAFKA_LOG_FMT_LEVEL(logger, KAFKA::LogLevel::FATAL, fmt, __VA_ARGS__)


KAFKA_NAMESPACE_BEGIN

class Logger;
class LogAppender;
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
    static const char * toString(LogLevel::Level level);

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
    LogEvent() = default;

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
    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
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
    std::shared_ptr<Logger> getLogger() const {return m_logger;}

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
    //线程ID
    uint32_t m_threadId = 0;
    //协程ID
    uint32_t m_fiberId = 0;
    //时间戳
    uint64_t m_time = 0;
    //线程名称
    std::string m_threadName;
    //字符串流
    std::stringstream m_ss;
    //日志器
    std::shared_ptr<Logger> m_logger;
    //日志等级
    LogLevel::Level m_level;
};

class LogEventWrap {
public:
    /**
     * @brief
     * @param event
     */
    explicit LogEventWrap(LogEvent::LogEventPtr event);

    /**
     * @brief
     */
    ~LogEventWrap();

    /**
     * @brief
     * @return
     */
    LogEvent::LogEventPtr getEvent() const {return m_event;}

    /**
     * @brief
     * @return
     */
    std::stringstream& getSS();
private:
    LogEvent::LogEventPtr m_event;
};

class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> LogFormatterPtr;

    explicit LogFormatter(const std::string &pattern);

    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::LogEventPtr event);

    std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::LogEventPtr event);

    /**
     * @brief generate format
     */
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
        virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::LogEventPtr event) = 0;
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
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::LogEventPtr event) = 0;

    /**
     * @brief
     */
    virtual ~LogAppender() = default;

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

public:
    bool m_hasFormatter = false;

    LogLevel::Level m_level = LogLevel::DEBUG;

    LogFormatter::LogFormatterPtr m_formatter;
};

class Logger : public std::enable_shared_from_this<Logger> {
public:
    typedef std::shared_ptr<Logger> LoggerPtr;

    /**
     * @brief
     * @param name
     */
    explicit Logger(const std::string &name = "root");

    /**
     * @brief
     * @param level
     * @param event
     */
    void log (LogLevel::Level level, const LogEvent::LogEventPtr & event);

    /**
     * @brief
     * @param event
     */
    void debug(LogEvent::LogEventPtr event);

    /**
     * @brief
     * @param event
     */
    void info(LogEvent::LogEventPtr event);

    /**
     * @brief
     * @param event
     */
    void warn(LogEvent::LogEventPtr event);

    /**
     * @brief
     * @param event
     */
    void error(LogEvent::LogEventPtr event);

    /**
     * @brief
     * @param event
     */
    void fatal(LogEvent::LogEventPtr event);

    /**
     * @brief
     * @param appender
     */
    void addAppender(LogAppender::LogAppenderPtr appender);

    /**
     * @brief
     * @param appender
     */
    void delAppender(LogAppender::LogAppenderPtr appender);

    /**
     * @brief
     */
    void cleanAppender();

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
    const std::string& getName() const {return m_name;}

    /**
     * @brief
     * @param formatter
     */
    void setFormatter(LogFormatter::LogFormatterPtr formatter);

    /**
     * @brief
     * @param str
     */
    void setFormatter(const std::string& str);

    /**
     * @brief
     * @return
     */
    LogFormatter::LogFormatterPtr getFormatter();

    /**
     * @brief
     * @return
     */
    Logger::LoggerPtr getRootLogger() const {return m_root;}

    /**
     * @brief
     * @param root
     */
    void setRootLogger(const Logger::LoggerPtr root) {m_root = root;}

    /**
     * @brief
     * @return
     */
    std::string toYamlString();
private:
    //日志级别
    LogLevel::Level m_level;
    //日志名称
    std::string m_name;
    //日志输出目标集合
    std::list<LogAppender::LogAppenderPtr> m_appenders;
    //主日志器
    Logger::LoggerPtr m_root;
    //
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
    typedef std::shared_ptr<FileLogAppender> FileLogAppenderPtr;

    /**
     * @brief
     * @param filename
     */
    explicit FileLogAppender(const std::string &filename);

    ~FileLogAppender() override;

    void log(Logger::LoggerPtr logger, LogLevel::Level level, LogEvent::LogEventPtr event) override;

    std::string toYamlString() override;

    bool reopen();

private:
    std::string m_filename;
    std::ofstream m_filestream;
    uint64_t m_lastTime = 0;
};

class LoggerManager {
public:
    /**
     * @brief
     */
    LoggerManager();

    /**
     * @brief
     * @param name
     * @return
     */
    Logger::LoggerPtr getLogger(const std::string name);

    /**
     * @brief
     */
    void init();

    /**
     * @brief
     * @return
     */
    Logger::LoggerPtr getRoot() const {return m_root;}

    /**
     * @brief
     */
     std::string toYamlString();

private:
    std::unordered_map<std::string, Logger::LoggerPtr> m_loggers;
    Logger::LoggerPtr m_root;
};

//日志管理类的单例模式
typedef KAFKA::Singleton<LoggerManager> LoggerMgr;

KAFKA_NAMESPACE_END
#endif //KAFKA_LOG_H
