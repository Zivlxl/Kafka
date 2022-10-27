/**
 * @file log.cpp
 * @brief
 * @author ziv
 * @email
 * @date 22-10-26.
 * @copyright Copyright (c) 2022年 ziv All rights reserved.
 */

#include "log.h"
#include <functional>
#include <map>
#include <time.h>
#include <string.h>
#include <iostream>

KAFKA_NAMESPACE_BEGIN

const char * LogLevel::toString(LogLevel::Level level) {
    switch (level) {
#define Func(name) \
    case LogLevel::name: \
        return #name;    \
        break;
        Func(DEBUG);
        Func(INFO);
        Func(WARN);
        Func(ERROR);
        Func(FATAL);
#undef Func
        default:
            return "UNKNOWN";
    }
    return "UNKNOWN";
}

LogLevel::Level LogLevel::fromString(const std::string &str) {
#define Func(level, v)  \
    if(str == #v) {     \
        return LogLevel::level \
    }

    Func(DEBUG, debug);
    Func(INFO, info);
    Func(WARN, warn);
    Func(ERROR, error);
    Func(FATAL, fatal);

    Func(DEBUG, DEBUG);
    Func(INFO, INFO);
    Func(WARN, WARN);
    Func(ERROR, ERROR);
    Func(FATAL, FATAL);
#undef Func
    return LogLevel::UNKNOWN;
}

class MessageFormatItem : public LogFormatter::FormatItem {
public:
    MessageFormatItem(const std::string &str = "") {}
    void format(std::ostream& os,
                Logger::LoggerPtr logger,
                LogLevel::Level level,
                LogEvent::LogEventPtr event) {
        os << event->getContent();
    }
};

class LevelFormatItem : public LogFormatter::FormatItem {
public:
    LevelFormatItem(const std::string &str = "") {}
    void format(std::ostream& os,
                Logger::LoggerPtr logger,
                LogLevel::Level level,
                LogEvent::LogEventPtr event) {
        os << LogLevel::toString(level);
    }
};

class ElapseFormatItem : public LogFormatter::FormatItem {
public:
    ElapseFormatItem(const std::string &str = "") {}
    void format(std::ostream& os,
                Logger::LoggerPtr logger,
                LogLevel::Level level,
                LogEvent::LogEventPtr event) {
        os << event->getElapse();
    }
};

class NameFormatItem : public LogFormatter::FormatItem {
public:
    NameFormatItem(const std::string &str = "") {}
    void format(std::ostream& os,
                Logger::LoggerPtr logger,
                LogLevel::Level level,
                LogEvent::LogEventPtr event) {
        os << event->getThreadName();
    }
};

class ThreadIdFormatItem : public LogFormatter::FormatItem {
public:
    ThreadIdFormatItem(const std::string &str = "") {}
    void format(std::ostream& os,
                Logger::LoggerPtr logger,
                LogLevel::Level level,
                LogEvent::LogEventPtr event) {
        os << event->getThreadId();
    }
};

class NewLineFormatItem : public LogFormatter::FormatItem {
public:
    NewLineFormatItem(const std::string &str = "") {}
    void format(std::ostream& os,
                Logger::LoggerPtr logger,
                LogLevel::Level level,
                LogEvent::LogEventPtr event) {
        os << std::endl;
    }
};

class DateTimeFormatItem : public LogFormatter::FormatItem {
public:
    DateTimeFormatItem(const std::string &format = "%Y-%m-%d %H:%M:%S") : m_format(format) {
        if (m_format.empty()) {
            m_format = "%Y-%m-%d %H:%M:%S";
        }
    }
    void format(std::ostream& os,
                Logger::LoggerPtr logger,
                LogLevel::Level level,
                LogEvent::LogEventPtr event) {
        struct tm tm;
        time_t time = event->getTime();
        localtime_r(&time, &tm);
        constexpr bufferSize = m_format * 10;
        char buf[bufferSize];
        strftime(buf, sizeof(buf), m_format.c_str(), &tm);
        os << buf;
    }
private:
    std::string m_format;
};

class FilenameFormatItem : public LogFormatter::FormatItem {
public:
    FilenameFormatItem(const std::string &str = "") {}
    void format(std::ostream& os,
                Logger::LoggerPtr logger,
                LogLevel::Level level,
                LogEvent::LogEventPtr event) {
        os << event->getFile();
    }
};

class LineFormatItem : public LogFormatter::FormatItem {
public:
    LineFormatItem(const std::string &str = "") {}
    void format(std::ostream& os,
                Logger::LoggerPtr logger,
                LogLevel::Level level,
                LogEvent::LogEventPtr event) {
        os << event->getLine();
    }
};

class TabFormatItem : public LogFormatter::FormatItem {
public:
    TabFormatItem(const std::string &str = "") {}
    void format(std::ostream& os,
                Logger::LoggerPtr logger,
                LogLevel::Level level,
                LogEvent::LogEventPtr event) {
        os << "\t";
    }
};

class FiberIdFormatItem : public LogFormatter::FormatItem {
public:
    FiberIdFormatItem(const std::string &str = "") {}
    void format(std::ostream& os,
                Logger::LoggerPtr logger,
                LogLevel::Level level,
                LogEvent::LogEventPtr event) {
        os << event->getFiberId();
    }
};

class ThreadNameFormatItem : public LogFormatter::FormatItem {
public:
    ThreadNameFormatItem(const std::string &str = "") {}
    void format(std::ostream& os,
                Logger::LoggerPtr logger,
                LogLevel::Level level,
                LogEvent::LogEventPtr event) {
        os << event->getThreadName();
    }
};

class StringFormatItem : public LogFormatter::FormatItem {
public:
    StringFormatItem(const std::string &str) : m_string(str) {}
    void format(std::ostream& os,
                Logger::LoggerPtr logger,
                LogLevel::Level level,
                LogEvent::LogEventPtr event) {
        os << m_string;
    }
private:
    std::string m_string;
};

LogFormatter::LogFormatter(const std::string &pattern) : m_pattern(pattern) {
    init();
}

std::string LogFormatter::format(Logger::LoggerPtr logger, LogLevel::Level level, LogEvent::LogEventPtr event) {
    std::stringstream ss;
    for (auto & item : m_items) {
        item->format(ss, logger, level, event);
    }
    return ss.str();
}

std::ostream& LogFormatter::format(std::ostream &ofs, Logger::LoggerPtr logger, LogLevel::Level level,
                                   LogEvent::LogEventPtr event) {
    for (auto & item : m_items) {
        item->format(ofs, logger, level, event);
    }

    return ofs;
}

//%xxx or %xxx{xxx} or %%
//需要解析出以上三种格式
void LogFormatter::init() {
    //str, format, type, 当type为0时，普通字符串
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string nstr;

    for (size_t i = 0; i < m_pattern.size(); i++) {
        //第一个不是%时，为普通字符串
        if (m_pattern[i] != '%') {
            nstr.append(1, m_pattern[i]);
            continue;
        }

        //连续两个%，把第一个视作普通字符串的一部分
        if ((i + 1) < m_pattern.size()) {
            nstr.append(1, '%');
            continue;
        }

        //从一个%开始，匹配format
        size_t n = i + 1;
        int fmt_status = 0;
        size_t fmt_begin = 0;

        std::string str;
        std::string fmt;

        while (n < m_pattern.size()) {
            if (!fmt_status && (!isalpha(m_pattern[n]) && m_pattern[n] != '{' && m_pattern[n] != '}')) {
                str = m_pattern.substr(i + 1, n - i - 1);
            }

            if (!fmt_status) {
                if (m_pattern[n] == '{') {
                    str = m_pattern.substr(i + 1, n - i - 1);
                    fmt_status = 1;
                    fmt_begin = n;
                    ++n;
                    continue;
                }
            }
            else {
                if (m_pattern[n] == '}') {
                    fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                    fmt_status = 0;
                    ++n;
                    break;
                }
            }

            ++n;
            if (n == m_pattern.size()) {
                if(str.empty()) {
                    str = m_pattern.substr(i+1);
                }
            }
        }

        if(!fmt_status) {
            //普通字符串
            if (!nstr.empty()) {
                vec.push_back(std::make_tuple(nstr, std::string(), 0));
                nstr.clear();
            }
            //formatter
            vec.push_back(std::make_tuple(str, fmt, 1));

            //回到正确位置
            i = n - 1;
        }
        //error pattern format
        else {
            std::cout << "pattern parse error: " << m_pattern << "-" << m_pattern.substr(i) << std::endl;
            m_error = true;
            vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
        }
    }

    if (!nstr.empty()) {
        vec.push_back(std::make_tuple(nstr, "", 0));
    }

    static std::map<std::string, std::function<FormatItem::FormatItemPtr(const std::string& str)>>  s_format_item = {
#define Func(str, C) \
        {#str, [](const std::string &fmt){return FormatItem::FormatItemPtr(new C(fmt));}}

        Func(m, MessageFormatItem),           //m:消息
        Func(p, LevelFormatItem),             //p:日志级别
        Func(r, ElapseFormatItem),            //r:累计毫秒数
        Func(c, NameFormatItem),              //c:日志名称
        Func(t, ThreadIdFormatItem),          //t:线程id
        Func(n, NewLineFormatItem),           //n:换行
        Func(d, DateTimeFormatItem),          //d:时间
        Func(f, FilenameFormatItem),          //f:文件名
        Func(l, LineFormatItem),              //l:行号
        Func(T, TabFormatItem),               //T:Tab
        Func(F, FiberIdFormatItem),           //F:协程id
        Func(N, ThreadNameFormatItem),        //N:线程名称
#undef Func
    };

    for (auto & i : vec) {
        if (std::get<2>(i) == 0) {
            m_items.push_back(FormatItem::FormatItemPtr(new StringFormatItem(std::get<0>(i))));
        }
        else {
            auto it = s_format_item.find(std::get<0>(i));
            if (it == s_format_item.end()) {
                m_items.push_back(FormatItem::FormatItemPtr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
                m_error = true;
            }
            else {
                m_items.push_back(it->second(std::get<1>(i)));
            }
        }
    }
}

void LogAppender::setFormatter(LogFormatter::LogFormatterPtr formatter) {
    m_formatter = formatter;
    if (m_formatter) m_hasFormatter = true;
    else m_hasFormatter = false;
}

LogFormatter::LogFormatterPtr LogAppender::getFormatter() {
    return m_formatter;
}

void StdoutLogAppender::log(Logger::LoggerPtr logger, LogLevel::Level level, LogEvent::LogEventPtr event) {
    if (level >= m_level) {
        std::cout << m_formatter->format(logger, level, event);
    }
}

std::string StdoutLogAppender::toYamlString() {

}

FileLogAppender::FileLogAppender(const std::string &filename) : m_filename(filename) {

}

void FileLogAppender::log(Logger::LoggerPtr logger, LogLevel::Level level, LogEvent::LogEventPtr event) {
    if (level >= m_level) {
        m_filestream << m_formatter->format(logger, level, event);
    }
}

std::string FileLogAppender::toYamlString() {

}

bool FileLogAppender::reopen() {
    if (m_filestream) {
        m_filestream.close();
    }
    m_filestream.open(m_filename);
    return !!m_filestream;
}
KAFKA_NAMESPACE_END