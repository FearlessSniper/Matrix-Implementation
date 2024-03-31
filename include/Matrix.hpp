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
class Matrix : MatrixBase<T> {
   public:
    Matrix(int m, int n);
    Matrix(Matrix<T>&& mat) = default;
    Matrix(std::initializer_list<std::initializer_list<T>> l);
    ~Matrix();

   protected:
    std::shared_ptr<T[]> get_mem() const override;

   private:
    std::shared_ptr<T[]> mem;
};

}  // namespace MatMulImpl
