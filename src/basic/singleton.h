/**
 * @file singleton.h
 * @brief
 * @author ziv
 * @email
 * @date 22-11-1.
 * @copyright Copyright (c) 2022年 ziv All rights reserved.
 */

#ifndef KAFKA_SINGLETON_H
#define KAFKA_SINGLETON_H

#include "basicDefine.h"
#include <memory>

KAFKA_NAMESPACE_BEGIN

template<class T, class X = void, int N = 0>
T& GetInstance() {
    static T val;
    return &val;
}

template<class T, class X = void, int N = 0>
std::shared_ptr<T> GetInstance() {
    static std::shared_ptr<T> val(new T);
    return val;
}

/**
 * @brief 单例模式
 * @tparam T 类型
 * @tparam X 为了创建多个实例对应的Tag
 * @tparam N 同一个Tag创造多个实例索引
 */
template<class T, class X = void, int N = 0>
class Singleton {
public:
    static T* GetInstance() {
        static T val;
        return &val;
    }
};

template<class T, class X = void, int N = 0>
class SingletonPtr {
public:
    static std::shared_ptr<T> GetInstance() {
        static std::shared_ptr<T> val(new T);
        return val;
    }
};

KAFKA_NAMESPACE_END
#endif //KAFKA_SINGLETON_H
