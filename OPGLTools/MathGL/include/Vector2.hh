#ifndef Vector2_HPP
#define Vector2_HPP

#include <math.h> 

class Vector2 {

    public:
        int x, y;

        Vector2(int _x=0, int _y=0, int _z=0): x(_x), y(_y){}

        Vector2(const Vector2 &val): x(val.x), y(val.y){}
        ~Vector2() {}

        /// Vector Atributes

        float Mag(){ return sqrt(pow(x,2) + pow(y,2));}
        float Mag2(){ return (pow(x,2) + pow(y,2));}

        /// Operators

        Vector2 operator+(const Vector2 &val) const {
            return Vector2(x + val.x, y + val.y);
        }

    private:
};

#endif