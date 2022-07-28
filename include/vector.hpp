#ifndef CCANNY_INCLUDE_VECTOR_H
#define CCANNY_INCLUDE_VECTOR_H

#include <iostream>
#include <string>
#include <cstring>

namespace ccanny {

template<int Length>
class Vector;

// 先声明成模板函数，然后在Vector类中才能够把它绑定到Vector类上
template<int Length>
std::ostream& operator<<(std::ostream &s, const Vector<Length>& vector);

template<int Length>
class Vector {
private:
    typedef Vector<Length> Vector_;
    float data_[Length];

public:
    Vector() = default;
    
    Vector(const float x) {
        for (int i = 0; i < Length; i++) {
            data_[i] = x;
        }
    }

    // Vector_ ? Vector<Length> ?
    // Why you choose to consider so much ?
    // Why you need to consider so much ?
    // -v-
    Vector(const Vector_& other) {
        std::memcpy(data_, other.data_, sizeof(float) * Length);
    }

    Vector_& operator=(const Vector_& other) {
        std::memcpy(data_, other.data_, sizeof(float) * Length);
        return *this;
    }

    // Move constructor or move assign operator ?
    // They are useless under any circumstance. (I think)

    Vector_ operator+(const Vector_& other) const {
        Vector_ res;
        for (int i = 0; i < Length; i++) {
            res.data_[i] = data_[i] + other.data_[i];
        }
        return res;
    }

    Vector_ operator-(const Vector_& other) const {
        Vector_ res;
        for (int i = 0; i < Length; i++) {
            res.data_[i] = data_[i] - other.data_[i];
        }
        return res;
    }

    Vector_ operator*(const Vector_& other) const {
        Vector_ res;
        for (int i = 0; i < Length; i++) {
            res.data_[i] = data_[i] * other.data_[i];
        }
        return res;
    }

    Vector_ operator/(const Vector_& other) const {
        Vector_ res;
        for (int i = 0; i < Length; i++) {
            res.data_[i] = data_[i] / other.data_[i];
        }
        return res;
    }

    Vector_ operator/(const float x) const {
        Vector_ res;
        for (int i = 0; i < Length; i++) {
            res.data_[i] = data_[i] / x;
        }
        return res;
    }

    Vector_& operator+=(const Vector_& other) {
        for (int i = 0; i < Length; i++) {
            data_[i] += other.data_[i];
        }
        return *this;
    }

    // why I can't add const to this function ?
    // 因为data位于结构体内部，而不在堆上，所以加上const之后返回值的引用也必须是const
    // 而如果data的内存在堆上，就不会有这个问题了，此时const只保证指针不变，不保证堆的内容不变
    // float& operator[](const int i) const {
    float& operator[](const int i) {
        return data_[i];
    }

    float sum() const {
        float s = 0.f;
        for (int i = 0; i < Length; i++) {
            s += data_[i];
        }
        return s;
    }

    /* TODO: other operations if necessary */
    // 每次Vector类特化之后，都会对对应参数的友元函数产生特化需求
    // 编译器会自动对友元函数进行特化
    friend std::ostream& operator<<<Length>(std::ostream &s, const Vector_& vector);
};

template<int Length>
std::ostream& operator<<(std::ostream &s, const Vector<Length>& vector) {
    s << "(";
    for (int i = 0; i < Length; i++) {
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