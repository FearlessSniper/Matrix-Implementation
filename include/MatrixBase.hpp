
#pragma once

#include <iostream>
#include <memory>
#include <utility>

namespace MatMulImpl {
using Dim_t = std::pair<int, int>;

template <class T>
class Matrix;
template <class T>
class MatrixView;

template <class T>
class MatrixBase {
    MatrixBase() = delete;

   public:
    const Dim_t& dim() const;
    MatrixView<T> sub(int n, int m, int off_x, int off_y) const;
    const MatrixView<const T> csub(int n, int m, int off_x, int off_y) const;
    Matrix<T> operator+(const MatrixBase<const T>& other) const;
    Matrix<T> operator*(const T& lambda) const;
    Matrix<T> virtual operator*(const MatrixBase<const T>& other) const;
    T& at(int r, int c) const;

   protected:
    std::shared_ptr<T[]> get_mem() const;

   private:
    Dim_t _dim;
    Dim_t _offset = std::make_pair(0, 0);
    Dim_t _main_dim;  // copy for keeping track of main matrix size in views
    std::shared_ptr<T[]> virtual _mem() const;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const MatrixBase<T>& m);

}  // namespace MatMulImpl
