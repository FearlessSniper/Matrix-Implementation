
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
   public:
    const Dim_t& dim() const;
    MatrixView<T>& sub(int i, int j, int n, int m);
    const MatrixView<T>& csub(int i, int j, int n, int m);
    Matrix<T> operator+(const MatrixBase<T>& b) const;
    Matrix<T> virtual operator*(const MatrixBase<T>& b) const;
    Matrix<T> operator*(const T& b) const;
    virtual T& at(int i, int j);

   protected:
    Dim_t _dim;
    Dim_t offset = std::make_pair(0, 0);
    std::shared_ptr<T[]> virtual get_mem() const;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const MatrixBase<T>& m);

}  // namespace MatMulImpl
