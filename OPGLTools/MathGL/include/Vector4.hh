#ifndef Vector4_HPP
#define Vector4_HPP

#include <math.h>
#include <cassert> 

class Vector4 {

    public:
        float x, y, z, w;

        Vector4(float r[4]): x(r[0]), y(r[1]), z(r[2]), w(r[3]){}
        Vector4(float _x=0, float _y=0, float _z=0, float _w=0): x(_x), y(_y), z(_z), w(_w){}
        Vector4(const Vector4 &val): x(val.x), y(val.y), z(val.z), w(val.w){}
        ~Vector4() {}

        /// Vector Atributes

        float Mag(){ return sqrt(pow(x,2) + pow(y,2) + pow(z,2) + pow(w,2));}

        const float* value_ptr() const {
            return &x;
        }

        /// Operators

        Vector4 operator+(const Vector4 &val) const {
            return Vector4(x + val.x, y + val.y, z+val.z, w+val.w);
        }

        Vector4 operator*(float f) const {
            return Vector4(x * f, y * f, z * f, w * f);
        }

        // [] operator
        float &operator[](int i) {
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
        const float &operator[](int i) const {
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

#endif