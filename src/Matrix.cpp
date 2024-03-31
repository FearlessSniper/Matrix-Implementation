/**
 * @file Matrix.cpp
 * @author aydenwong (aydenwongfs@gmail.com)
 * @brief
 * @version 0.1
 * @date 28-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "Matrix.hpp"

#include <iomanip>
#include <sstream>

namespace MatMulImpl {

constexpr int DP_WIDTH = 6;

BadDimensionException::BadDimensionException() : explain("") {}
BadDimensionException::BadDimensionException(const char* s) : explain(s) {}
const char* BadDimensionException::what() const noexcept {
    return explain.c_str();
}

/**
 * @brief A lightweight view object providing matrix-like operations. Intended
 * to be used for submatrices.
 *
 * @tparam T
 */
template <class T>
MatrixView<T>::MatrixView(T* mem, int m, int n, int row_size)
    : mem(mem), mem_row_size(row_size), _dim({m, n}) {}

template <class T>
MatrixView<T> MatMulImpl::MatrixView<T>::build(T* mem, int m, int n,
                                               int row_size) {
    return MatrixView<T>(mem, m, n, row_size);  // idk why i'm doing this
}

template <class T>
const Dim_t& MatrixView<T>::dim() const {
    return _dim;
}

template <class T>
inline T& MatrixView<T>::item(int i, int j) {
    return mem[i * mem_row_size + j];
}
template <class T>
inline const T& MatrixView<T>::citem(int i, int j) const {
    return mem[i * mem_row_size + j];
}
template <class T>
MatrixView<T> MatrixView<T>::operator+(const MatrixView<T>& b) const {
    if (!(_dim == b.dim())) {
        std::stringstream ss;
        ss << "The size of the two matrices does not match. LHS is "
           << _dim.first << "x" << _dim.second << " but RHS is "
           << b.dim().first << "x" << b.dim().second;
        throw BadDimensionException(ss.str().c_str());
    }
    auto c = Matrix<T>(_dim.first, _dim.second);
    for (int i = 0; i < _dim.first; i++) {
        for (int j = 0; j < _dim.second; j++) {
            c.item(i, j) = citem(i, j) + b.citem(i, j);
        }
    }
    return std::move(c);
}
template <class T>
MatrixView<T> MatrixView<T>::operator*(const MatrixView<T>& b) const {
    if (_dim.second != b.dim().first) {
        std::stringstream ss;
        ss << "Matrix A is an " << _dim.first << "x" << _dim.second
           << " matrix. Matrix B has to have " << _dim.second
           << " rows, but instead have " << b.dim().first;
        throw BadDimensionException(ss.str().c_str());
    }
    auto c = Matrix<T>(_dim.first, b.dim().second);
    for (int i = 0; i < _dim.first; i++) {
        for (int j = 0; j < b.dim().second; j++) {
            c.item(i, j) = citem(i, 0) * b.citem(0, j);
            for (int k = 1; k < b.dim().second; k++) {
                c.item(i, j) += citem(i, k) * b.citem(k, j);  // Traditional
            }
        }
    }
    return std::move(c);
}

/**
 * @brief Construct a new Matrix object of size m x n
 *
 * @param m Number of rows of the matrix
 * @param n Number of columns of the matrix
 */
template <class T>
Matrix<T>::Matrix(int m, int n)
    : MatrixView<T>::MatrixView(new T[m * n], m, n, n) {
}  // Note: any matrix elements accessed immediately after this is undefined
template <class T>
Matrix<T> Matrix<T>::from(std::initializer_list<std::initializer_list<T>> l) {
    int n_rows = l.size();
    int n_cols = l.begin()->size();
    for (auto it = l.begin() + 1; it != l.end(); it++) {
        if (it->size() > n_cols) {
            n_cols = it->size();
        }
    }
    auto mat = Matrix<T>(n_rows, n_cols);
    auto row_it = l.begin();
    for (int i = 0; i < mat.dim().first; i++) {
        auto col_it = row_it->begin();
        for (int j = 0; j < mat.dim().second; j++) {
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
template <class T>
Matrix<T>::~Matrix() {
    delete[] this->mem;
}
template <class T>
MatrixView<T>& Matrix<T>::sub(int i, int j, int n, int m) {
    views.push_back(MatrixView<T>::build(this->mem + i * this->mem_row_size + j,
                                         n, m, this->mem_row_size));
    return views.back();
}
template <class T>
const MatrixView<T>& Matrix<T>::csub(int i, int j, int n, int m) {
    return sub(i, j, n, m);
}
template <class T>
Matrix<T>& Matrix<T>::operator+=(const MatrixView<T>& b) {
    if (!(this->_dim == b.dim())) {
        std::stringstream ss;
        ss << "The size of the two matrices does not match. LHS is "
           << this->_dim.first << "x" << this->_dim.second << " but RHS is "
           << b.dim().first << "x" << b.dim().second;
        throw BadDimensionException(ss.str().c_str());
    }
    for (int i = 0; i < this->_dim.first; i++) {
        for (int j = 0; j < this->_dim.second; j++) {
            this->item(i, j) += b.citem(i, j);
        }
    }
    return *this;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const MatrixView<T>& m) {
    for (int i = 0; i < m.dim().first; i++) {
        for (int j = 0; j < m.dim().second; j++) {
            os << std::setw(DP_WIDTH) << m.citem(i, j);
        }
        os << "\n";
    }
    return os;
}
template class MatrixView<int>;
template class Matrix<int>;
template std::ostream& operator<<(std::ostream& os, const MatrixView<int>& m);
}  // namespace MatMulImpl
