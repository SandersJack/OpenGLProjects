#ifndef Matrix4_HPP
#define Matrix4_HPP

#include <math.h> 
#include <iostream>
#include "Vector3.hh"
#include "Vector4.hh"

class Matrix4 {

    public:

        float m[4][4];

        Matrix4() {
            SetIdentity();
        }


        ~Matrix4() {}

        Matrix4(float scale) {
            SetScalar(scale);
        }

        /// NEEED TO CONVERT TO COLUMN MAJOR

        Matrix4(float m_00, float m_10, float m_20, float m_30,
                float m_01, float m_11, float m_21, float m_31,
                float m_02, float m_12, float m_22, float m_32,
                float m_03, float m_13, float m_23, float m_33) {
            m[0][0] = m_00; m[1][0] = m_10; m[2][0] = m_20;  m[3][0] = m_30;
            m[0][1] = m_01; m[1][1] = m_11; m[2][1] = m_21;  m[3][1] = m_31;
            m[0][2] = m_02; m[1][2] = m_12; m[2][2] = m_22;  m[3][2] = m_32;
            m[0][3] = m_03; m[1][3] = m_13; m[2][3] = m_23;  m[3][3] = m_33;
        }

        explicit Matrix4(float n[4][4]) {
            m[0][0]=n[0][0]; m[1][0]=n[1][0]; m[2][0]=n[2][0]; m[3][0]=n[3][0];
            m[0][1]=n[0][1]; m[1][1]=n[1][1]; m[2][1]=n[2][1]; m[3][1]=n[3][1];
            m[0][2]=n[0][2]; m[1][2]=n[1][2]; m[2][2]=n[2][2]; m[3][2]=n[3][2];
            m[0][3]=n[0][3]; m[1][3]=n[1][3]; m[2][3]=n[2][3]; m[3][3]=n[3][3];
        }

        // Functions

         void SetIdentity() {
            // Initialize as identity matrix
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    m[i][j] = (i == j) ? 1.0f : 0.0f;
                }
            }
        }

        void SetScalar(float scalar) {
            // Initialize as a scalar matrix
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    m[i][j] = (i == j) ? scalar : 0.0f;
                }
            }
        }

        const void Print() const{
            for (int i = 0; i < 4; ++i) {
                std::cout << m[0][i] << " " << m[1][i] << " " << m[2][i] << " " << m[3][i] << std::endl;
            }
        }

        const float* value_ptr() const {
            return &m[0][0];
        }

        float determinant() const {
            float det = m[0][0] * minor(1, 2, 3, 1, 2, 3) -
                        m[1][0] * minor(1, 2, 3, 0, 2, 3) +
                        m[2][0] * minor(1, 2, 3, 0, 1, 3) -
                        m[3][0] * minor(1, 2, 3, 0, 1, 2);
            
            return det;
        }

        Matrix4 inverse() const {
            Matrix4 result;

            float det = determinant();

            // Check if the determinant is close to zero (considering floating-point precision)
            if (std::abs(det) < 1e-8f) {
                // Matrix is singular, unable to calculate the inverse
                std::cerr << "Matrix is singular, cannot calculate inverse." << std::endl;
                return result;  // Return an identity matrix or some default value
            }
            // Calculate the inverse matrix using cofactors and adjugate
            for (int col = 0; col < 4; ++col) {
                for (int row = 0; row < 4; ++row) {
                    // Calculate the cofactor
                    float cofactor = cofactorAt(row, col);

                    // Calculate the adjugate
                    result.m[col][row] = cofactor / det;
                }
            }
            return result;
        }

        Matrix4 translate(const Vector3& translation) {
            Matrix4 result = *this;
            // Update the matrix with translation
            result.m[3][0] += translation.x;
            result.m[3][1] += translation.y;
            result.m[3][2] += translation.z;

            return result;
        }

        /// Operators
        
        /// * operators
        Matrix4 operator*(float f) const {
            return Matrix4(m[0][0]*f, m[1][0]*f, m[2][0]*f,  m[3][0]*f,
                           m[0][1]*f, m[1][1]*f, m[2][1]*f,  m[3][1]*f,
                           m[0][2]*f, m[1][2]*f, m[2][2]*f,  m[3][2]*f,
                           m[0][3]*f, m[1][3]*f, m[2][3]*f,  m[3][3]*f);
        }

        
        Matrix4 operator*(const Matrix4& other) const {
            Matrix4 result(0.0f);

            for (int j = 0; j < 4; ++j) {
                for (int i = 0; i < 4; ++i) {
                    for (int k = 0; k < 4; ++k) {
                        result.m[i][j] += m[k][j] * other.m[i][k];
                    }
                }
            }

            return result;
        }
        /// [] Operator

        Vector4 operator[](int index){

            Vector4 result = Vector4(m[index][0], m[index][1], m[index][2], m[index][3]);
            return result;
        }

        const Vector4 operator[](int index) const{
            Vector4 result = Vector4(m[index][0], m[index][1], m[index][2], m[index][3]);
            return result;
        }

    private:

        // Helper method to calculate the minor of a 3x3 matrix
        float minor(int r0, int r1, int r2, int c0, int c1, int c2) const {
            return  m[c0][r0] * (m[c1][r1] * m[c2][r2] - m[c1][r2] * m[c2][r1]) -
                    m[c1][r0] * (m[c0][r1] * m[c2][r2] - m[c0][r2] * m[c2][r1]) +
                    m[c2][r0] * (m[c0][r1] * m[c1][r2] - m[c0][r2] * m[c1][r1]);
        }

        // Helper method to calculate the cofactor of a specific element
        float cofactorAt(int col, int row) const {
            int sign = ((row + col) % 2 == 0) ? 1 : -1;
            return sign * minor((row + 1) % 4, (row + 2) % 4, (row + 3) % 4,
                                (col + 1) % 4, (col + 2) % 4, (col + 3) % 4);
    }
};

#endif