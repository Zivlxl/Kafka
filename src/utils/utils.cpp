/**
 * @file utils.cpp
 * @brief
 * @author ziv
 * @email
 * @date 22-11-2.
 * @copyright Copyright (c) 2022年 ziv All rights reserved.
 */
#include "utils.h"

KAFKA_NAMESPACE_BEGIN

pid_t getThreadId() {
    return syscall(SYS_gettid);
}

uint32_t getFiberId() {
    return 0;
}

KAFKA_NAMESPACE_END