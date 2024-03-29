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

#include <exception>
#include <initializer_list>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace MatMulImpl {
using Dim_t = std::pair<int, int>;
class BadDimensionException : std::exception {
   public:
    BadDimensionException();
    BadDimensionException(const char* s);
    const char* what();

   private:
    std::string explain;
};
template <class T>
class MatrixView {
   public:
    MatrixView() = delete;
    const Dim_t& dim() const;
    virtual T& item(int i, int j);
    virtual const T& citem(int i, int j) const;
    MatrixView<T> operator+(const MatrixView<T>& b) const;
    MatrixView<T> operator*(const MatrixView<T>& b) const;

   protected:
    T* mem;
    int mem_row_size;
    Dim_t _dim;
    MatrixView(T* mem, int m, int n, int row_size);
    static MatrixView<T> build(T* mem, int m, int n, int row_size);
};

template <class T>
class Matrix : public MatrixView<T> {
   public:
    Matrix(int m, int n);
    Matrix(Matrix<T>&& mat) = default;
    static Matrix<T> from(std::initializer_list<std::initializer_list<T>> l);
    ~Matrix();
    MatrixView<T>& sub(int i, int j, int n, int m);
    const MatrixView<T>& csub(int i, int j, int n, int m);
    Matrix<T>& operator+=(const MatrixView<T>& b);

   private:
    std::vector<MatrixView<T>> views;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const MatrixView<T>& m);
}  // namespace MatMulImpl

#endif  // MATRIX_HPP
