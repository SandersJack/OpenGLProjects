#ifndef Matrix4_HPP
#define Matrix4_HPP

#include <math.h> 

class Matrix4 {

    public:

        ~Matrix4() {}

        Matrix4(float m_00=0, float m_01=0, float m_02=0, float m_03=0,
                float m_10=0, float m_11=0, float m_12=0, float m_13=0,
                float m_20=0, float m_21=0, float m_22=0, float m_23=0,
                float m_30=0, float m_31=0, float m_32=0, float m_33=0) {
            m[0][0] = m_00; m[0][1] = m_01; m[0][2] = m_02;  m[0][3] = m_03;
            m[1][0] = m_10; m[1][1] = m_11; m[1][2] = m_12;  m[1][3] = m_13;
            m[2][0] = m_20; m[2][1] = m_21; m[2][2] = m_22;  m[2][3] = m_23;
            m[3][0] = m_30; m[3][1] = m_31; m[3][2] = m_32;  m[3][3] = m_33;
        }

        Matrix4(float n[4][4]) {
            m[0][0]=n[0][0]; m[0][1]=n[0][1]; m[0][2]=n[0][2]; m[0][3]=n[0][3];
            m[1][0]=n[1][0]; m[1][1]=n[1][1]; m[1][2]=n[1][2]; m[1][3]=n[1][3];
            m[2][0]=n[2][0]; m[2][1]=n[2][1]; m[2][2]=n[2][2]; m[2][3]=n[2][3];
            m[3][0]=n[3][0]; m[3][1]=n[3][1]; m[3][2]=n[3][2]; m[3][3]=n[3][3];
        }

        // Functions

        const float* value_ptr() const {
            return &m[0][0];
        }

        /// Operators
        
        /// * operators
        Matrix4 operator*(float f) const {
            return Matrix4(m[0][0]*f, m[0][1]*f, m[0][2]*f,  m[0][3]*f,
                           m[1][0]*f, m[1][1]*f, m[1][2]*f,  m[1][3]*f,
                           m[2][0]*f, m[2][1]*f, m[2][2]*f,  m[2][3]*f,
                           m[3][0]*f, m[3][1]*f, m[3][2]*f,  m[3][3]*f);
        }

        
        Matrix4 operator*(const Matrix4 &n) const {
            Matrix4 o;
            for(int i = 0; i  < 4; i++) {
                for(int j = 0; j < 4; j++) {
                    float v = 0;
                    for(int k = 0; k < 4; k++) {
                        v += m[i][k] * n.m[k][j];
                    }
                    o.m[i][j] = v;
                }
            }
            return o;
        }


        /// [] Operator

        float* operator[](int index){
            return m[index];
        }

        const float* operator[](int index) const{
            return m[index];
        }

    private:
        float m[4][4];
};

#endif