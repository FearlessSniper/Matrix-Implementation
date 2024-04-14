/**
 * @file Algorithms.hpp
 * @author aydenwong (aydenwongfs@gmail.com)
 * @brief Implementation of several matrix multiplication algorithms in C++.
 * @version 0.1
 * @date 01-04-2024
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <algorithm>
#include <tuple>

#include "Matrixv2.hpp"

namespace MatMulImpl {
/**
 * @brief Determines if a number is a power of 2
 * @warning The behaviour is not defined when the number is 0
 * @param n A *positive* integer
 * @return true
 * @return false
 */
inline bool isP2(int n) { return (n & (n - 1)) == 0; }

template <class T>
using SqSlices = std::tuple<Matrix2<T>, Matrix2<T>, Matrix2<T>, Matrix2<T>>;

/**
 * @brief Slices a square matrix into pieces of 4.
 *
 * @tparam T Type of element of Matrix
 * @param m Square matrix to slice
 * @return SqSlices<T> {M11, M12, M21, M22}
 */
template <class T>
SqSlices<T> square_slice(Matrix2<T> &m) {
    int k = m.m / 2;
    return std::make_tuple(m.sub(0, 0, k, k), m.sub(0, k, k, k),
                           m.sub(k, 0, k, k), m.sub(k, k, k, k));
}

/**
 * @brief Contains functions for matrix multiplication
 * @warning Some functions have no sanity checks for matrix dimensions.
 */
class Multiplication {
   public:
    template <class T>
    static Matrix2<T> naive(const Matrix2<T> &a, const Matrix2<T> &b) {
        return a * b;  // Only here because we want a similar function signature
    }
    template <class T>
    static Matrix2<T> div_and_conquer(const Matrix2<T> &a,
                                      const Matrix2<T> &b) {
        Matrix2<T> c(a.m, b.n);
        _div_and_conquer(a, b, c);
        return c;
    }
    /**
     * @brief Multiplies two matrices with the divide and conquer method
     * @warning Assumes matrix a and b are both nxn in size, and n is a power
     * of 2.
     * @tparam T Type of element of matrix
     * @param a Left operand matrix
     * @param b Right operand matrix
     * @return Matrix2<T> The result matrix
     */
    template <class T>
    static Matrix2<T> div_and_conquer_sq2(const Matrix2<T> &a,
                                          const Matrix2<T> &b) {
        // Reference: Introduction to algorithms 3rd-edition, Ch 4.2, p.77,
        // SQUARE-MATRIX-MULTIPLY-RECURSIVE
        // if (a.dim() != b.dim() && !isP2(a.n) && !isP2(a.m))
        //     throw BadDimensionException(
        //         "div_and_conquer_sq2: Matrix is either not square or size is
        //         " "not a power of 2.");
        if (a.m == 1 && b.m == 1)
            return Matrix2<T>::from({{a.citem(0, 0) * b.citem(0, 0)}});
        Matrix2<T> c(a.n, a.n);
        int k = a.n / 2;
        auto a11 = a.csub(0, 0, k, k), a12 = a.csub(0, k, k, k),
             a21 = a.csub(k, 0, k, k), a22 = a.csub(k, k, k, k);
        auto b11 = b.csub(0, 0, k, k), b12 = b.csub(0, k, k, k),
             b21 = b.csub(k, 0, k, k), b22 = b.csub(k, k, k, k);
        auto &&[c11, c12, c21, c22] = square_slice(c);  // requires C++17
        c11.sum_from(div_and_conquer_sq2(a11, b11),
                     div_and_conquer_sq2(a12, b21));
        c12.sum_from(div_and_conquer_sq2(a11, b12),
                     div_and_conquer_sq2(a12, b22));
        c21.sum_from(div_and_conquer_sq2(a21, b11),
                     div_and_conquer_sq2(a22, b21));
        c22.sum_from(div_and_conquer_sq2(a21, b12),
                     div_and_conquer_sq2(a22, b22));
        return c;
    }
    /**
     * @brief Implementation of the Strassen fast matrix multiplication
     * algorithm
     * This is a simple implementation; It only supports matrices of size
     * that is a power of two, but any matrix can be padded (i.e. adding
     * rows are columns) to make the size a power of 2. For simplicity,
     * We will not do that here.
     * @warning a, b must have size of power 2
     * @tparam T type of entries in a and b
     * @param a Left operand matrix a
     * @param b Right operand matrix b
     * @return Matrix2<T> Resultant matrix
     */
    template <class T>
    static Matrix2<T> strassen(const Matrix2<T> &a, const Matrix2<T> &b) {
        if (a.m == 1 && b.m == 1)
            return Matrix2<T>::from({{a.citem(0, 0) * b.citem(0, 0)}});
        auto &&a11 = a.csub(0, 0, a.m / 2, a.n / 2),
             &&a12 = a.csub(0, a.n / 2, a.m / 2, a.n / 2);
        auto &&a21 = a.csub(a.m / 2, 0, a.m / 2, a.n / 2),
             &&a22 = a.csub(a.m / 2, a.n / 2, a.m / 2, a.n / 2);
        auto &&b11 = b.csub(0, 0, b.m / 2, b.n / 2),
             &&b12 = b.csub(0, b.n / 2, b.m / 2, b.n / 2);
        auto &&b21 = b.csub(b.m / 2, 0, b.m / 2, b.n / 2),
             &&b22 = b.csub(b.m / 2, b.n / 2, b.m / 2, b.n / 2);
        auto &&m1 = strassen(a11 + a22, b11 + b22);
        auto &&m2 = strassen(a21 + a22, b11);
        auto &&m3 = strassen(a11, b12 - b22);
        auto &&m4 = strassen(a22, b21 - b11);
        auto &&m5 = strassen(a11 + a12, b22);
        auto &&m6 = strassen(a21 - a11, b11 + b12);
        auto &&m7 = strassen(a12 - a22, b21 + b22);
        Matrix2<T> c(a.m, b.n);
        c.sub(0, 0, c.m / 2, c.n / 2).sum_from(m1 + m4 - m5, m7);
        c.sub(0, c.n / 2, c.m / 2, c.n / 2).sum_from(m3, m5);
        c.sub(c.m / 2, 0, c.m / 2, c.n / 2).sum_from(m2, m4);
        c.sub(c.m / 2, c.n / 2, c.m / 2, c.n / 2).sum_from(m1 - m2 + m3, m6);
        return c;
    }

   private:
    // template <class T>
    // static void _div_and_conquer_sq2(const Matrix2<T> &a, const Matrix2<T>
    // &b,
    //                                  Matrix2<T> &c, int n) {
    //     if (n == 1) {
    //         c.item(0, 0) = a.citem(0, 0) * b.citem(0, 0);
    //     } else {
    //         int k = n / 2;
    //         Matrix2<T> c11(c.sub(0, 0, k, k));
    //         Matrix2<T> c12(c.sub(0, k, k, k));
    //         Matrix2<T> c21(c.sub(k, 0, k, k));
    //         Matrix2<T> c22(c.sub(k, k, k, k));
    //         _div_and_conquer_sq2(a.csub(0, 0, k, k), b.csub(0, 0, k, k), c1,
    //                              k);  // top left
    //         _div_and_conquer_sq2(a.csub(0, k, k, k), b.csub(0, k, k, k), c2,
    //                              k);  // top right
    //         _div_and_conquer_sq2(a.csub(k, 0, k, k), b.csub(k, 0, k, k), c3,
    //         k); _div_and_conquer_sq2(a.csub(k, k, k, k), b.csub(k, k, k, k),
    //         c4, k);
    //     }
    // }
    template <class T>
    static void _div_and_conquer(const Matrix2<T> &a, const Matrix2<T> &b,
                                 Matrix2<T> &c) {
        // Testing: Assert c.m == a.m, c.n == b.n
        // Reference:
        // https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm#Non-square_matrices
        int sz_max = std::max({a.m, a.n, b.n});

        if (sz_max <= 2) {
            c.product_from(a, b);
        } else if (sz_max == a.m) {
            // Split A horizontally
            int p = a.m / 2;
            auto a1 = a.csub(0, 0, p, a.n), a2 = a.csub(p, 0, a.m - p, a.n);
            auto c1 = c.sub(0, 0, p, b.n), c2 = c.sub(p, 0, a.m - p, b.n);
            _div_and_conquer(a1, b, c1);
            _div_and_conquer(a2, b, c2);
        } else if (sz_max == b.n) {
            int k = b.n / 2;
            auto b1 = b.csub(0, 0, b.m, k), b2 = b.csub(0, k, b.m, b.n - k);
            auto c1 = c.sub(0, 0, a.m, k), c2 = c.sub(0, k, a.m, b.n - k);
            _div_and_conquer(a, b1, c1);
            _div_and_conquer(a, b2, c2);
        } else {
            int k = a.n / 2;
            Matrix2<T> c1(a.m, b.n), c2(a.m, b.n);
            auto a1 = a.csub(0, 0, a.m, k), a2 = a.csub(0, k, a.m, a.n - k);
            auto b1 = b.csub(0, 0, k, b.n), b2 = b.csub(k, 0, a.n - k, b.n);

            _div_and_conquer(a1, b1, c1);
            _div_and_conquer(a2, b2, c2);
            
            c.sum_from(c1, c2);
        }
    }
};
}  // namespace MatMulImpl

#endif  // ALGORITHMS_HPP
