#pragma once

#include <memory>

#include "MatrixBase.hpp"

namespace MatMulImpl {
template <class T>
class MatrixView : public MatrixBase<T> {
    MatrixView() = delete;

   protected:
    MatrixView(std::shared_ptr<std::shared_ptr<T>[]> mem, int n, int m, int off_x, int off_y,
               Dim_t main_dim)
        : MatrixBase<T>(n, m, off_x, off_y, main_dim), _mem(mem) {}

   private:
    std::weak_ptr<std::shared_ptr<T>[]> _mem;
    /**
     * @brief Get the memory pointer
     *
     * @tparam T
     * @return std::shared_ptr<std::shared_ptr<T>[]>
     */
    std::shared_ptr<std::shared_ptr<T>[]> virtual _get_mem() const { return _mem.lock(); }

    friend class MatrixBase<T>;
};

}  // namespace MatMulImpl
