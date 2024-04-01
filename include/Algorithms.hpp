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

#include "Matrixv2.hpp"

namespace MatMulImpl {
/**
 * @brief Determines if a number is a power of 2
 * @warning The behaviour is not defined when the number is 0
 * @param n A *positive* integer
 * @return true
 * @return false
 */
bool isP2(int n) { return n & (n - 1) == 0; }

/**
 * @brief Contains functions for matrix multiplication
 * @warning All functions have no sanity checks for matrix dimensions.
 */
class Multiplication {
   public:
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
        int n = a.n;
        Matrix2<T> c(n, n);
        Multiplication::_div_and_conquer_sq2(a, b, c, n);
        return c;
    }

   private:
    template <class T>
    static void _div_and_conquer_sq2(const Matrix2<T> &a, const Matrix2<T> &b,
                                     Matrix2<T> &c, int n) {
        if (n == 1) {
            c.item(0, 0) = a.item(0, 0) * b.item(0, 0);
        } else {
            int k = n / 2;
            _div_and_conquer_sq2(a.csub(0, 0, k, k), b.csub(0, 0, k, k),
                                 c.sub(0, 0, k, k), k);  // top left
            _div_and_conquer_sq2(a.csub(0, k, k, k), b.csub(0, k, k, k),
                                 c.sub(0, k, k, k), k);  // top right
            _div_and_conquer_sq2(a.csub(k, 0, k, k), b.csub(k, 0, k, k),
                                 c.sub(k, 0, k, k), k);
            _div_and_conquer_sq2(a.csub(k, k, k, k), b.csub(k, k, k, k),
                                 c.sub(k, k, k, k), k);
        }
    }
    template <class T>
    static void _div_and_conquer(const Matrix2<T> &a, const Matrix2<T> &b,
                                 Matrix2<T> &c) {
        // Testing: Assert c.m == a.m, c.n == b.n
        // Reference:
        // https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm#Non-square_matrices
        int sz_max = std::max({a.m, a.n, b.n});
        if (sz_max == 2) {
            c = a * b;  // problematic
        } else if (sz_max == a.m) {
            // Split A horizontally
            int p = a.m / 2;
            _div_and_conquer(a.csub(0, 0, p, a.n), b, c.sub(0, 0, p, c.n));
            _div_and_conquer(a.csub(p, 0, a.m - p, a.n), b,
                             c.sub(p, 0, a.m - p, c.n));
        } else if (sz_max == b.n) {
            int k = b.n / 2;
            _div_and_conquer(a, b.csub(0, 0, b.m, k), c.sub(0, 0, b.m, k));
            _div_and_conquer(a, b.csub(0, k, b.m, b.n - k),
                             c.sub(0, k, b.m, b.n - k));
        } else {
            int k = a.n / 2;
            Matrixv2<T> c1(a.m, b.n), c2(a.m, b.n);
            _div_and_conquer(a.csub(0, 0, a.m, p), b.csub(0, 0, p, b.n), c1);
            _div_and_conquer(a.csub(0, p, a.m, a.n - p),
                             b.csub(p, 0, a.n - p, b.n), c2);
            c = c1 + c2;  // problematic
        }
    }
};
}  // namespace MatMulImpl

#endif  // ALGORITHMS_HPP
