/**
 * @file Matrixv2.hpp
 * @author aydenwong (aydenwongfs@gmail.com)
 * @brief A matrix implementation.
 * @version 0.1
 * @date 29-03-2024
 *
 * Note: Implementation is put here because template substitution type
 * has to be known for the code to be compiled. This means the template code
 * has to be in the same compilation unit as the caller for it to be compiled,
 * and to do that we put the source in the header file.
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MATRIXV2_HPP
#define MATRIXV2_HPP

#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

namespace MatMulImpl {
using Dim_t = std::pair<int, int>;
class BadDimensionException : std::exception {
   public:
    BadDimensionException();
    BadDimensionException(const char* s);
    const char* what() const noexcept override;

   private:
    std::string explain;
};
inline BadDimensionException::BadDimensionException() : explain("") {}
inline BadDimensionException::BadDimensionException(const char* s) : explain(s) {}
inline const char* BadDimensionException::what() const noexcept { return explain.c_str(); }
template <class T>
class Matrix2 {
   public:
    Matrix2() = delete;
    Matrix2(int m, int n) : mem(new T[m * n]), m(m), n(n), mem_row_sz(n) {}
    Matrix2(Matrix2<T>& m) = delete;   // no copying for now: not implemented
    ~Matrix2() {
        if (!is_view) {
            delete[] mem;
        }
    }
    static Matrix2<T> from(std::initializer_list<std::initializer_list<T> > l) {
        int n_rows = l.size();
        int n_cols = l.begin()->size();
        for (auto it = l.begin() + 1; it != l.end(); it++) {
            if (it->size() > n_cols) {
                n_cols = it->size();
            }
        }
        auto mat = Matrix2<T>(n_rows, n_cols);
        auto row_it = l.begin();
        for (int i = 0; i < n_rows; i++) {
            auto col_it = row_it->begin();
            for (int j = 0; j < n_cols; j++) {
                if (col_it != row_it->end()) {
                    mat.item(i, j) = *col_it;
                    col_it++;
                } else {
                    mat.item(i, j) = 0;
                }
            }
            row_it++;
        }
        return mat;
    }
    static Matrix2<T> identity(int n) {
        Matrix2<T> mat(n, n);
        for (int i = 0; i < n; i++) {
            mat.item(i, i) = 1;
        }
        return mat;
    }
    inline T& item(int i, int j) { return mem[i * mem_row_sz + j]; }
    inline const T& citem(int i, int j) const {
        return mem[i * mem_row_sz + j];
    }
    Dim_t dim() const { return Dim_t({m, n}); }
    Matrix2<T>& sub(int i, int j, int M, int N) {
        // We don't have to keep track of the submatrices; we'll trust the
        // caller (ourselves!) that we know what we are doing. I have no
        // way of telling whether a submatrix is done being used or not.
        // Besides, the sub matrix object should go out-of-scope with
        // the parent matrix, right?
        return Matrix2<T>(mem + i * mem_row_sz + j, M, N, mem_row_sz);
    }
    const Matrix2<T>& csub(int i, int j, int M, int N) {
        return sub(i, j, M, N);
    }
    Matrix2<T> operator+(const Matrix2<T>& b) const {
        if (!(Dim_t{m, n} == b.dim())) {
            std::stringstream ss;
            ss << "The size of the two matrices does not match. LHS is " << m
               << "x" << n << " but RHS is " << b.dim().first << "x"
               << b.dim().second;
            throw BadDimensionException(ss.str().c_str());
        }
        Matrix2<T> c(m, n);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                c.item(i, j) = citem(i, j) + b.citem(i, j);
            }
        }
        return c;
    }
    Matrix2<T> operator-(const Matrix2<T>& b) const {
        if (!(Dim_t{m, n} == b.dim())) {
            std::stringstream ss;
            ss << "The size of the two matrices does not match. LHS is " << m
               << "x" << n << " but RHS is " << b.dim().first << "x"
               << b.dim().second;
            throw BadDimensionException(ss.str().c_str());
        }
        Matrix2<T> c(m, n);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                c.item(i, j) = citem(i, j) - b.citem(i, j);
            }
        }
        return c;
    }
    Matrix2<T> operator*(const Matrix2<T>& b) const {
        if (n != b.dim().first) {
            std::stringstream ss;
            ss << "Matrix A is an " << m << "x" << n
               << " matrix. Matrix B has to have " << n
               << " rows, but instead have " << b.dim().first;
            throw BadDimensionException(ss.str().c_str());
        }
        Matrix2<T> c(m, n);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                c.item(i, j) = citem(i, 0) * b.citem(0, j);
                for (int k = 1; k < n; k++) {
                    c.item(i, j) += citem(i, k) * b.citem(k, j);  // Traditional
                }
            }
        }
        return c;
    }
    int print_width = 6;
    const int m, n;  // mxn matrix; it's constant so why not public

   private:
    T* mem;
    const int mem_row_sz;  // Number of items in a row in the actual matrix
    bool is_view = false;
    Matrix2(T* mem, int m, int n, int mem_row_sz)
        : mem(mem), m(m), n(n), mem_row_sz(mem_row_sz), is_view(true) {}
};
template <class T1, class T2>
Matrix2<T2> operator*(T1 k, const Matrix2<T2>& mat) {
    auto dim = mat.dim();
    int m = dim.first, n = dim.second;
    Matrix2<T2> c(m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            c.item(m, n) = k * mat.item(i, j);
        }
    }
    return c;
}
template <class T>
std::ostream& operator<<(std::ostream& os, const Matrix2<T>& m) {
    for (int i = 0; i < m.dim().first; i++) {
        for (int j = 0; j < m.dim().second; j++) {
            os << std::setw(m.print_width) << m.citem(i, j);
        }
        os << '\n';
    }
    return os;
}
}  // namespace MatMulImpl

#endif  // MATRIXV2_HPP
