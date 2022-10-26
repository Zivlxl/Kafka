/**
 * @file noncopyable.h
 * @brief
 * @author ziv
 * @email
 * @date 22-10-26.
 * @copyright Copyright (c) 2022年 ziv All rights reserved.
 */

#ifndef KAFKA_NONCOPYABLE_H
#define KAFKA_NONCOPYABLE_H

#include "basicDefine.h"

KAFKA_NAMESPACE_BEGIN

class noncopyable {
protected:
    noncopyable() = default;
    ~noncopyable() = default;
    noncopyable(const noncopyable&) = delete;
    const noncopyable& operator=(const noncopyable&) = delete;
};

KAFKA_NAMESPACE_END

#endif //KAFKA_NONCOPYABLE_H
