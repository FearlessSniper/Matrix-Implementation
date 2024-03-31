#ifndef MATRIXVIEW_HPP
#define MATRIXVIEW_HPP

#include <memory>

#include "Matrix.hpp"

namespace MatMulImpl {
template <class T>
class MatrixView : public Matrix<T> {
   public:
    MatrixView(int m, int n) = delete;
    MatrixView(Matrix<T>&& mat) = delete;
    MatrixView(std::initializer_list<std::initializer_list<T>> l) = delete;

   protected:
    MatrixView(std::shared_ptr<T[]> mem, Dim_t dim, Dim_t offset);
    T& item(int i, int j) override;
    Dim_t offset;

   private:
    std::weak_ptr<T[]> mem_ref;
};

}  // namespace MatMulImpl

#endif  // MATRIXVIEW_HPP