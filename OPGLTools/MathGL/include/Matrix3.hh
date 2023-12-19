#ifndef Matrix3_HPP
#define Matrix3_HPP

#include <math.h> 

class Matrix3 {

    public:
        float m[3][3];

        ~Matrix3() {}

        Matrix3(float m_00=0, float m_01=0, float m_02=0,
                float m_10=0, float m_11=0, float m_12=0,
                float m_20=0, float m_21=0, float m_22=0) {
            m[0][0] = m_00; m[0][1] = m_01; m[0][2] = m_02;  
            m[1][0] = m_10; m[1][1] = m_11; m[1][2] = m_12;  
            m[2][0] = m_20; m[2][1] = m_21; m[2][2] = m_22;  
        }

        Matrix3(float n[4][4]) {
            m[0][0]=n[0][0]; m[0][1]=n[0][1]; m[0][2]=n[0][2];
            m[1][0]=n[1][0]; m[1][1]=n[1][1]; m[1][2]=n[1][2]; 
            m[2][0]=n[2][0]; m[2][1]=n[2][1]; m[2][2]=n[2][2]; 
        }


    private:
};

#endif