#ifndef _Vector4_HPP
#define _Vector4_HPP

#include <math.h>
#include <cassert> 
#include <iostream>
template <typename T>
class _Vector4 {

    public:
        T x, y, z, w;

        _Vector4(T r[4]): x(r[0]), y(r[1]), z(r[2]), w(r[3]){}
        _Vector4(T _x=0, T _y=0, T _z=0, T _w=0): x(_x), y(_y), z(_z), w(_w){}
        _Vector4(const _Vector4 &val): x(val.x), y(val.y), z(val.z), w(val.w){}
        ~_Vector4() {}

        /// Vector Atributes

        T Mag(){ return sqrt(pow(x,2) + pow(y,2) + pow(z,2) + pow(w,2));}

        const T* value_ptr() const {
            return &x;
        }

        void Print() {
            std::cout << x << " " << y << " " << z << " " << w << std::endl;
        }

        /// Operators

        _Vector4 operator+(const _Vector4 &val) const {
            return _Vector4(x + val.x, y + val.y, z+val.z, w+val.w);
        }

        _Vector4 operator*(T f) const {
            return _Vector4(x * f, y * f, z * f, w * f);
        }

        // [] operator
        T &operator[](int i) {
            assert(i >= 0 && i <= 3);
            switch (i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            }
            return w;
        }

        // [] operator
        const T &operator[](int i) const {
            assert(i >= 0 && i <= 3);
            switch (i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            }
            return w;
        }

    private:

};

using Vector4 = _Vector4<float>;
using iVector4 = _Vector4<int>;

#endif