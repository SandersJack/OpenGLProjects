#ifndef Vector3_HPP
#define Vector3_HPP

#include <math.h> 
#include <cassert>

class Vector3 {

    public:
        float x, y, z;

        Vector3(float _x=0, float _y=0, float _z=0): x(_x), y(_y), z(_z){}
        Vector3(const Vector3 &val): x(val.x), y(val.y), z(val.z){}
        ~Vector3() {}

        /// Vector Atributes

        float Mag(){ return sqrt(pow(x,2) + pow(y,2) + pow(z,2));}
        float Mag2(){ return (pow(x,2) + pow(y,2) + pow(z,2));}

        Vector3 cross(const Vector3 &v) const {
            return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
        }

        float dot(const Vector3 &v) const{  
            return x * v.x + y * v.y + z * v.z;
        }

        Vector3 normalize() {
            float length = Mag();
            // Check if the length is not zero to avoid division by zero
            if (length != 0.0f) {
                x /= length;
                y /= length;
                z /= length;
            }
            return *this;
        }

        /// Operators

        Vector3 operator+(const Vector3 &val) const {
            return Vector3(x + val.x, y + val.y, z + val.z);
        }

        Vector3 operator-(const Vector3 &val) const {
            return Vector3(x - val.x, y - val.y, z - val.z);
        }

        Vector3 operator*(float f) const {
            return Vector3(x * f, y * f, z * f);
        }

        /// _= operators

        Vector3 operator+=(const Vector3 &v){
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        Vector3 operator-=(const Vector3 &v){
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        // [] operator
        float &operator[](int i) {
            assert(i >= 0 && i <= 2);
            switch (i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            }
            return z;
        }

        // [] operator
        const float &operator[](int i) const {
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

#endif