/**
 * @file Matrix.hpp
 * @author aydenwong (aydenwongfs@gmail.com)
 * @brief
 * @version 0.1
 * @date 28-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <initializer_list>
#include <iostream>
#include <memory>
#include <utility>

namespace MatMulImpl {
using Dim_t = std::pair<int, int>;

template <class T>
class MatrixView;

template <class T>
class Matrix {
   public:
    Matrix(int m, int n);
    Matrix(Matrix<T>&& mat) = default;
    Matrix(std::initializer_list<std::initializer_list<T>> l);
    ~Matrix();
    const Dim_t& dim() const;
    MatrixView<T>& sub(int i, int j, int n, int m);
    const MatrixView<T>& csub(int i, int j, int n, int m);
    Matrix<T> operator+(const MatrixView<T>& b) const;
    Matrix<T> virtual operator*(const MatrixView<T>& b) const;
    Matrix<T> operator*(const T& b) const;
    virtual T& at(int i, int j);

   protected:
    Dim_t _dim;
    virtual T& item(int i, int j);

   private:
    std::shared_ptr<T[]> mem;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m);

}  // namespace MatMulImpl

#endif  // MATRIX_HPP
