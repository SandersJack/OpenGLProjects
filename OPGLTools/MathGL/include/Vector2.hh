#ifndef _Vector2_HPP
#define _Vector2_HPP

#include <math.h> 

template <typename T>
class _Vector2 {

    public:
        T x, y;

        _Vector2(T _x=0, T _y=0, T _z=0): x(_x), y(_y){}

        _Vector2(const _Vector2 &val): x(val.x), y(val.y){}
        ~_Vector2() {}

        /// Vector Atributes

        T Mag(){ return sqrt(pow(x,2) + pow(y,2));}
        T Mag2(){ return (pow(x,2) + pow(y,2));}

        float dot(const _Vector2 &v) const{  
            return x * v.x + y * v.y;
        }

        _Vector2 normalize() {
            T length = Mag();
            // Check if the length is not zero to avoid division by zero
            if (length != 0.0f) {
                x /= length;
                y /= length;
            }
            return *this;
        }

        void Print() {
            std::cout << x << " " << y << std::endl;
        }

        const T* value_ptr() const {
            return &x;
        }

        /// Operators

        _Vector2 operator+(const _Vector2 &val) const {
            return _Vector2(x + val.x, y + val.y);
        }

        _Vector2 operator-(const _Vector2 &val) const {
            return _Vector2(x - val.x, y - val.y);
        }

        _Vector2 operator*(float f) const {
            return _Vector2(x * f, y * f);
        }

        /// _= operators

        _Vector2 operator+=(const _Vector2 &v){
            x += v.x;
            y += v.y;
            return *this;
        }

        _Vector2 operator-=(const _Vector2 &v){
            x -= v.x;
            y -= v.y;
            return *this;
        }

        // [] operator
        T &operator[](int i) {
            assert(i >= 0 && i <= 1);
            switch (i) {
            case 0: return x;
            case 1: return y;
            }
            return y;
        }

        // [] operator
        const T &operator[](int i) const {
            assert(i >= 0 && i <= 1);
            switch (i) {
            case 0: return x;
            case 1: return y;
            }
            return y;
        }

    private:
};

using Vector2 = _Vector2<float>;
using iVector2 = _Vector2<int>;


#endif