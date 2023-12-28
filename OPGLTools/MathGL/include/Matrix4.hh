#ifndef _Matrix4_HPP
#define _Matrix4_HPP

#include <math.h> 
#include <iostream>
#include "Vector3.hh"
#include "Vector4.hh"
template <typename T>
class _Matrix4 {

    public:

        _Matrix4() {
            SetIdentity();
        }


        ~_Matrix4() {}

        _Matrix4(T scale) {
            SetScalar(scale);
        }

        _Matrix4(T m_00, T m_10, T m_20, T m_30,
                T m_01, T m_11, T m_21, T m_31,
                T m_02, T m_12, T m_22, T m_32,
                T m_03, T m_13, T m_23, T m_33) {
            m[0][0] = m_00; m[1][0] = m_10; m[2][0] = m_20;  m[3][0] = m_30;
            m[0][1] = m_01; m[1][1] = m_11; m[2][1] = m_21;  m[3][1] = m_31;
            m[0][2] = m_02; m[1][2] = m_12; m[2][2] = m_22;  m[3][2] = m_32;
            m[0][3] = m_03; m[1][3] = m_13; m[2][3] = m_23;  m[3][3] = m_33;
        }

        explicit _Matrix4(T n[4][4]) {
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

        void SetScalar(T scalar) {
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

        const T* value_ptr() const {
            return &m[0][0];
        }

        T determinant() const {
            T det = m[0][0] * minor(1, 2, 3, 1, 2, 3) -
                        m[1][0] * minor(1, 2, 3, 0, 2, 3) +
                        m[2][0] * minor(1, 2, 3, 0, 1, 3) -
                        m[3][0] * minor(1, 2, 3, 0, 1, 2);
            
            return det;
        }

        _Matrix4 inverse() const {
            _Matrix4 result;

            T det = determinant();

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
                    T cofactor = cofactorAt(row, col);

                    // Calculate the adjugate
                    result.m[col][row] = cofactor / det;
                }
            }
            return result;
        }

        _Matrix4 translate(const _Vector3<T>& translation) {
            _Matrix4 result = *this;
            // Update the matrix with translation
            result.m[3][0] += translation.x;
            result.m[3][1] += translation.y;
            result.m[3][2] += translation.z;

            return result;
        }

        /// Operators
        
        /// * operators
        _Matrix4 operator*(T f) const {
            return _Matrix4(m[0][0]*f, m[1][0]*f, m[2][0]*f,  m[3][0]*f,
                           m[0][1]*f, m[1][1]*f, m[2][1]*f,  m[3][1]*f,
                           m[0][2]*f, m[1][2]*f, m[2][2]*f,  m[3][2]*f,
                           m[0][3]*f, m[1][3]*f, m[2][3]*f,  m[3][3]*f);
        }

        
        _Matrix4 operator*(const _Matrix4& other) const {
            _Matrix4 result(0.0f);

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

        _Vector4<T>& operator[](int index) {
            return m[index];
        }

        const _Vector4<T>& operator[](int index) const {
            return m[index];
        }

    private:

        _Vector4<T> m[4];

        // Helper method to calculate the minor of a 3x3 matrix
        T minor(int r0, int r1, int r2, int c0, int c1, int c2) const {
            return  m[c0][r0] * (m[c1][r1] * m[c2][r2] - m[c1][r2] * m[c2][r1]) -
                    m[c1][r0] * (m[c0][r1] * m[c2][r2] - m[c0][r2] * m[c2][r1]) +
                    m[c2][r0] * (m[c0][r1] * m[c1][r2] - m[c0][r2] * m[c1][r1]);
        }

        // Helper method to calculate the cofactor of a specific element
        T cofactorAt(int col, int row) const {
            int sign = ((row + col) % 2 == 0) ? 1 : -1;
            return sign * minor((row + 1) % 4, (row + 2) % 4, (row + 3) % 4,
                                (col + 1) % 4, (col + 2) % 4, (col + 3) % 4);
    }
};

using Matrix4 = _Matrix4<float>;
using iMatrix4 = _Matrix4<int>;

#endif