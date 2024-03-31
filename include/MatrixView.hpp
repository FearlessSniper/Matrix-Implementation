#pragma once

#include <memory>

#include "MatrixBase.hpp"

namespace MatMulImpl {
template <class T>
class MatrixView : public MatrixBase<T> {
   protected:
    MatrixView(std::shared_ptr<T[]> mem, Dim_t dim, Dim_t offset);
    T& item(int i, int j) override;
    std::shared_ptr<T[]> get_mem() const override;

   private:
    std::weak_ptr<T[]> mem;
};

}  // namespace MatMulImpl
