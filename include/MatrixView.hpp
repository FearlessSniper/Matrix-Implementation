#pragma once

#include <memory>

#include "MatrixBase.hpp"

namespace MatMulImpl {
template <class T>
class MatrixView : public MatrixBase<T> {
    MatrixView() = delete;

   protected:
    MatrixView(std::shared_ptr<T[]> mem, int n, int m, int off_x, int off_y, Dim_t orig_dim);
    T& item(int i, int j) override;
    std::shared_ptr<T[]> get_mem() const override;

   private:
    std::weak_ptr<T[]> mem;
};

}  // namespace MatMulImpl
