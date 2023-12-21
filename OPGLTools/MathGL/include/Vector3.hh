#ifndef _Vector3_HPP
#define _Vector3_HPP

#include <math.h> 
#include <cassert>
#include <iostream>

#include "Vector2.hh"
#include "Vector4.hh"
template <typename T>
class _Vector3 {

    public:
        T x, y, z;

        _Vector3(T _x=0, T _y=0, T _z=0): x(_x), y(_y), z(_z){}
        _Vector3(const _Vector2<T> &val, T val_f): x(val.x), y(val.y), z(val_f){}
        _Vector3(const _Vector3<T> &val): x(val.x), y(val.y), z(val.z){}
        _Vector3(const _Vector4<T> &val): x(val.x), y(val.y), z(val.z){}
        ~_Vector3() {}

        /// Vector Atributes

        T Mag(){ return sqrt(pow(x,2) + pow(y,2) + pow(z,2));}
        T Mag2(){ return (pow(x,2) + pow(y,2) + pow(z,2));}

        _Vector3 cross(const _Vector3 &v) const {
            return _Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
        }

        T dot(const _Vector3 &v) const{  
            return x * v.x + y * v.y + z * v.z;
        }

        _Vector3 normalize() {
            T length = Mag();
            // Check if the length is not zero to avoid division by zero
            if (length != 0.0f) {
                x /= length;
                y /= length;
                z /= length;
            }
            return *this;
        }

        void Print() {
            std::cout << x << " " << y << " " << z << std::endl;
        }

        const T* value_ptr() const {
            return &x;
        }

        /// Operators

        _Vector3 operator+(const _Vector3 &val) const {
            return _Vector3(x + val.x, y + val.y, z + val.z);
        }

        _Vector3 operator-(const _Vector3 &val) const {
            return _Vector3(x - val.x, y - val.y, z - val.z);
        }

        _Vector3 operator*(T f) const {
            return _Vector3(x * f, y * f, z * f);
        }

        /// _= operators

        _Vector3 operator+=(const _Vector3 &v){
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        _Vector3 operator-=(const _Vector3 &v){
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        // [] operator
        T &operator[](int i) {
            assert(i >= 0 && i <= 2);
            switch (i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            }
            return z;
        }

        // [] operator
        const T &operator[](int i) const {
            assert(i >= 0 && i <= 2);
            switch (i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            }
            return z;
        }


    private:
};

using Vector3 = _Vector3<float>;
using iVector3 = _Vector3<int>;

#endif