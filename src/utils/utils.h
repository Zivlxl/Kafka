/**
 * @file utils.h
 * @brief
 * @author ziv
 * @email
 * @date 22-11-2.
 * @copyright Copyright (c) 2022年 ziv All rights reserved.
 */

#ifndef KAFKA_UTILS_H
#define KAFKA_UTILS_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "../basic/basicDefine.h"
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

KAFKA_NAMESPACE_BEGIN

pid_t getThreadId();

uint32_t getFiberId();

KAFKA_NAMESPACE_END

#endif //KAFKA_UTILS_H
