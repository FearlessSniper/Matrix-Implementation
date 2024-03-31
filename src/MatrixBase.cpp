#include "MatrixBase.hpp"

#include "MatrixExceptions.hpp"

namespace MatMulImpl {

template <class T>
const Dim_t& MatrixBase<T>::dim() const {
    return this->_dim;
}
/**
 * @brief Get MatrixView of size n x m starting at given offset
 *
 * @tparam T
 * @param n row size of the view
 * @param m column size of the view
 * @param off_x offset in x direction from top left corner
 * @param off_y offset in y direction from top left corner
 * @return MatrixView<T>
 */
template <class T>
MatrixView<T> MatrixBase<T>::sub(int n, int m, int off_x, int off_y) const {
    return MatrixView<T>(this->get_mem(), n, m, off_x, off_y, this->_main_dim);
}

/**
 * @brief Get MatrixView of size n x m starting at given offset
 *
 * @tparam T
 * @param n row size of the view
 * @param m column size of the view
 * @param off_x offset in x direction from top left corner
 * @param off_y offset in y direction from top left corner
 * @return MatrixView<const T>
 */
template <class T>
const MatrixView<const T> MatrixBase<T>::csub(int n, int m, int off_x,
                                              int off_y) const {
    return MatrixView<const T>(this->get_mem(), n, m, off_x, off_y,
                               this->_main_dim);
}

/**
 * @brief Add two matrices
 *
 * @tparam T
 * @param other Matrix to add
 * @return Matrix<T>
 */
template <class T>
Matrix<T> MatrixBase<T>::operator+(const MatrixBase<const T>& other) const {
    if (!(this->_dim == other.dim())) {
        throw BadDimensionException(this->_dim, other.dim());
    }

    auto res = Matrix<T>(this->_dim.first, this->_dim.second);
    for (int i = 0; i < this->_dim.first; i++) {
        for (int j = 0; j < this->_dim.second; j++) {
            res.item(i, j) = this->at(i, j) + other.at(i, j);
        }
    }
}

/**
 * @brief Multiply matrix with constant
 *
 * @tparam T
 * @param lambda constant to multiply with
 * @return Matrix<T>
 */
template <class T>
Matrix<T> MatrixBase<T>::operator*(const T& lambda) const {
    auto res = Matrix<T>(this->_dim.first, this->_dim.second);
    for (int i = 0; i < this->_dim.first; i++) {
        for (int j = 0; j < this->_dim.second; j++) {
            res.item(i, j) = this->at(i, j) * lambda;
        }
    }
    return res;
}

/**
 * @brief Multiply two matrices using the standard algorithm. This is a virtual
 * function and can be overridden to provide a more efficient implementation
 *
 * @tparam T
 * @param other Matrix to multiply with
 * @return Matrix<T>
 */
template <class T>
Matrix<T> MatrixBase<T>::operator*(const MatrixBase<const T>& other) const {
    if (this->_dim.second != other.dim().first) {
        throw BadDimensionException(this->_dim, other.dim());
    }

    auto res = Matrix<T>(this->_dim.first, other.dim().second);
    for (int i = 0; i < this->_dim.first; i++) {
        for (int j = 0; j < other.dim().second; j++) {
            T sum = 0;
            for (int k = 0; k < this->_dim.second; k++) {
                sum += this->at(i, k) * other.at(k, j);
            }
            res.item(i, j) = sum;
        }
    }
    return res;
}

/**
 * @brief Get the value at the given index
 *
 * @tparam T
 * @param i row index
 * @param j column index
 * @return T&
 */
template <class T>
T& MatrixBase<T>::at(int r, int c) const {
    std::shared_ptr<T[]> mem = this->get_mem();

    int row = _offset.first + r, col = _offset.second + c;
    if (row < 0 || col < 0 || row >= this->_main_dim.first ||
        col >= this->_main_dim.second || r >= this->_dim.first ||
        c >= this->_dim.second) {
        throw OutOfBoundsException(r, c, this->_dim, this->_main_dim);
    }

    return mem[row * this->_main_dim.second + col];
}

/**
 * @brief Perform validation and return the memory pointer
 *
 * @tparam T
 * @return std::shared_ptr<T[]>
 */
template <class T>
std::shared_ptr<T[]> MatrixBase<T>::get_mem() const {
    std::shared_ptr<T[]> mem = this->_mem();
    if (!mem) {
        throw NullPtrException();
    }
    return mem;
}

/**
 * @brief Get the memory pointer
 *
 * @tparam T
 * @return std::shared_ptr<T[]>
 */

template <class T>
std::shared_ptr<T[]> MatrixBase<T>::_mem() const {
    return nullptr;
}

}  // namespace MatMulImpl