#ifndef CCANNY_INCLUDE_VECTOR_H
#define CCANNY_INCLUDE_VECTOR_H

#include "basic_types.h"
#include <iostream>
#include <string>

namespace ccanny {

template<size_t Length>
class Vector;

// 先声明成模板函数，然后在Vector类中才能够把它绑定到Vector类上
template<size_t Length>
std::ostream& operator<<(std::ostream &s, const Vector<Length>& vector);

template<size_t Length>
class Vector {
private:
    typedef Vector<Length> Vector_;
    float_t data_[Length];

public:
    Vector() = default;
    
    Vector(const float_t x) {
        for (size_t i = 0; i < Length; i++) {
            data_[i] = x;
        }
    }

    // Vector_ ? Vector<Length> ?
    // Why you choose to consider so much ?
    // Why you need to consider so much ?
    // -v-
    Vector(const Vector_& other) {
        for (size_t i = 0; i < Length; i++) {
            data_[i] = other.data_[i];
        }
    }

    Vector_& operator=(const Vector_& other) {
        for (size_t i = 0; i < Length; i++) {
            data_[i] = other.data_[i];
        }
        return *this;
    }

    // Move constructor or move assign operator ?
    // They are useless under any circumstance. (I think)

    Vector_ operator+(const Vector_& other) const {
        Vector_ res;
        for (size_t i = 0; i < Length; i++) {
            res.data_[i] = data_[i] + other.data_[i];
        }
        return res;
    }

    Vector_ operator-(const Vector_& other) const {
        Vector_ res;
        for (size_t i = 0; i < Length; i++) {
            res.data_[i] = data_[i] - other.data_[i];
        }
        return res;
    }

    Vector_ operator*(const Vector_& other) const {
        Vector_ res;
        for (size_t i = 0; i < Length; i++) {
            res.data_[i] = data_[i] * other.data_[i];
        }
        return res;
    }

    Vector_ operator/(const Vector_& other) const {
        Vector_ res;
        for (size_t i = 0; i < Length; i++) {
            res.data_[i] = data_[i] / other.data_[i];
        }
        return res;
    }

    // TODO: why I can't add const to this function ?
    // float_t& operator[](const size_t i) const {
    float_t& operator[](const size_t i) {
        return data_[i];
    }

    /* TODO: other operations if necessary */
    // 每次Vector类特化之后，都会对对应参数的友元函数产生特化需求
    // 编译器会自动对友元函数进行特化
    friend std::ostream& operator<<<Length>(std::ostream &s, const Vector_& vector);
};

template<size_t Length>
std::ostream& operator<<(std::ostream &s, const Vector<Length>& vector) {
    s << "(";
    for (size_t i = 0; i < Length; i++) {
        if (!i) {
            s << vector.data_[i];
        } else {
            s << ", " << vector.data_[i];
        }
    }
    s << ")";
    return s;
}

} // namespace ccanny

#endif // CCANNY_INCLUDE_VECTOR_H