#ifndef _Matrix3_HPP
#define _Matrix3_HPP

#include <math.h> 
template <typename T>
class _Matrix3 {

    public:

        ~_Matrix3() {}

        _Matrix3(T m_00, T m_10, T m_20,
                T m_01, T m_11, T m_21, 
                T m_02, T m_12, T m_22) {
            m[0][0] = m_00; m[1][0] = m_10; m[2][0] = m_20;
            m[0][1] = m_01; m[1][1] = m_11; m[2][1] = m_21;  
            m[0][2] = m_02; m[1][2] = m_12; m[2][2] = m_22;  
        }

        explicit _Matrix3(T n[3][3]) {
            m[0][0]=n[0][0]; m[1][0]=n[1][0]; m[2][0]=n[2][0];
            m[0][1]=n[0][1]; m[1][1]=n[1][1]; m[2][1]=n[2][1];
            m[0][2]=n[0][2]; m[1][2]=n[1][2]; m[2][2]=n[2][2];
        }


    private:
        _Vector3<T> m[3];
};

using Matrix3 = _Matrix3<float>;
using iMatrix3 = _Matrix3<int>;

#endif