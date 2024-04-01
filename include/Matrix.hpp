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

#pragma once

#include <initializer_list>
#include <memory>

#include "MatrixBase.hpp"

namespace MatMulImpl {

template <class T>
class Matrix : public MatrixBase<T> {
   public:
    Matrix(int m, int n);
    Matrix(Matrix<T>&& mat) = default;
    Matrix(std::initializer_list<std::initializer_list<T>> l, int cols);
    Matrix(std::initializer_list<T> l, int cols);
    Matrix(std::initializer_list<T> l, int cols, int rows);


   private:
    std::shared_ptr<T[]> _mem;
    std::shared_ptr<T[]> _get_mem() const override;
};

}  // namespace MatMulImpl
