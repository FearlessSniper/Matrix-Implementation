
#pragma once
#include <iomanip>
#include <iostream>
#include <memory>
#include <utility>

#include "MatrixExceptions.hpp"

namespace MatMulImpl {
constexpr int DP_WIDTH = 6;

template <class T>
class Matrix;
template <class T>
class MatrixView;

template <class T>
class MatrixBase {
   public:
    const Dim_t& dim() const { return this->_dim; }

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
    MatrixView<T> sub(int n, int m, int off_x, int off_y) const {
        return MatrixView<T>(this->get_mem(), n, m, this->_offset.first + off_x,
                             this->_offset.second + off_y, this->_main_dim);
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
    const MatrixView<const T> csub(int n, int m, int off_x, int off_y) const {
        return MatrixView<const T>(
            this->get_mem(), n, m, this->_offset.first + off_x,
            this->_offset.second + off_y, this->_main_dim);
    }

    /**
     * @brief Add two matrices
     *
     * @tparam T
     * @param other Matrix to add
     * @return Matrix<T>
     */
    Matrix<T> operator+(const MatrixBase<T>& other) const {
        if (!(this->_dim == other.dim())) {
            throw BadDimensionException(this->_dim, other.dim());
        }

        Matrix<T> res(this->_dim.first, this->_dim.second);
        for (int i = 0; i < this->_dim.first; i++) {
            for (int j = 0; j < this->_dim.second; j++) {
                res.at(i, j) = this->at(i, j) + other.at(i, j);
            }
        }

        return res;
    }

    /**
     * @brief Multiply matrix with constant
     *
     * @tparam T
     * @param lambda constant to multiply with
     * @return Matrix<T>
     */
    Matrix<T> operator*(const T& lambda) const {
        Matrix<T> res(this->_dim.first, this->_dim.second);
        for (int i = 0; i < this->_dim.first; i++) {
            for (int j = 0; j < this->_dim.second; j++) {
                res.at(i, j) = this->at(i, j) * lambda;
            }
        }
        return res;
    }

    /**
     * @brief Multiply two matrices using the standard algorithm. This is a
     * virtual function and can be overridden to provide a more efficient
     * implementation
     *
     * @tparam T
     * @param other Matrix to multiply with
     * @return Matrix<T>
     */
    Matrix<T> virtual operator*(const MatrixBase<T>& other) const {
        if (this->_dim.second != other.dim().first) {
            throw BadDimensionException(this->_dim, other.dim());
        }

        Matrix<T> res(this->_dim.first, other.dim().second);
        for (int i = 0; i < this->_dim.first; i++) {
            for (int j = 0; j < other.dim().second; j++) {
                T sum = 0;
                for (int k = 0; k < this->_dim.second; k++) {
                    sum += this->at(i, k) * other.at(k, j);
                }
                res.at(i, j) = sum;
            }
        }
        return res;
    }

    /**
     * @brief Check if two matrices are equal
     *
     * @tparam T
     * @param other Matrix to compare with
     * @return true if equal
     * @return false if not equal
     */
    bool operator==(const MatrixBase<T>& other) const {
        if (this->_dim != other.dim()) {
            return false;
        }

        for (int i = 0; i < this->_dim.first; i++) {
            for (int j = 0; j < this->_dim.second; j++) {
                if (this->at(i, j) != other.at(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }
    /**
     * @brief Get the value at the given index
     *
     * @tparam T
     * @param i row index
     * @param j column index
     * @return T&
     */
    T& at(int r, int c) const {
        std::shared_ptr<T[]> mem = this->get_mem();

        int row = _offset.first + r, col = _offset.second + c;
        if (row < 0 || col < 0 || row >= this->_main_dim.first ||
            col >= this->_main_dim.second || r >= this->_dim.first ||
            c >= this->_dim.second) {
            throw OutOfBoundsException(r, c, this->_dim, this->_main_dim);
        }

        return mem[row * this->_main_dim.second + col];
    }

   protected:
    /**
     * @brief Construct a new Matrix Base object, to be called by views
     *
     * @param n row size
     * @param m column size
     * @param off_x offset in x direction from top left corner
     * @param off_y offset in y direction from top left corner
     * @param main_dim main matrix size
     */

    MatrixBase(int n, int m, int off_x, int off_y, Dim_t main_dim)
        : _dim(n, m), _main_dim(main_dim), _offset(off_x, off_y) {
        if (n <= 0 || m <= 0) {
            throw BadDimensionException(this->_dim);
        }
        if (this->_main_dim.first <= 0 || this->_main_dim.second <= 0) {
            throw BadDimensionException(this->_main_dim);
        }
        if (n > main_dim.first || m > main_dim.second) {
            throw BadDimensionException(this->_dim, main_dim);
        }
        if (off_x < 0 || off_y < 0 || off_x >= main_dim.first ||
            off_y >= main_dim.second) {
            throw OutOfBoundsException(off_x, off_y, this->_dim,
                                       this->_main_dim);
        }
        if (off_x + n > main_dim.first || off_y + m > main_dim.second) {
            throw OutOfBoundsException(off_x + n - 1, off_y + m - 1, this->_dim,
                                       this->_main_dim);
        }
    }

    /**
     * @brief Construct a new Matrix Base object, to be called by main matrix
     *
     * @param n row size
     * @param m column size
     */
    MatrixBase(int n, int m) : MatrixBase(n, m, 0, 0, std::make_pair(n, m)) {}

    /**
     * @brief Perform validation and return the memory pointer
     *
     * @tparam T
     * @return std::shared_ptr<T[]>
     */
    std::shared_ptr<T[]> get_mem() const {
        std::shared_ptr<T[]> mem = this->_get_mem();
        if (!mem) {
            throw NullPtrException();
        }
        return mem;
    }
    Dim_t _dim;
    Dim_t _main_dim;  // copy for keeping track of main matrix size in views
    Dim_t _offset;

   private:
    /**
     * @brief Get the memory pointer
     *
     * @tparam T
     * @return std::shared_ptr<T[]>
     */
    std::shared_ptr<T[]> virtual _get_mem() const { return nullptr; }
};

template <typename T>
Matrix<T> operator*(const T& lambda, const MatrixBase<T>& mat) {
    return mat * lambda;
}

/**
 * @brief Output the matrix to the given stream
 *
 * @tparam T
 * @param os output stream
 * @param m matrix to output
 * @return std::ostream&
 */
template <class T>
std::ostream& operator<<(std::ostream& os, const MatrixBase<T>& m) {
    for (int i = 0; i < m.dim().first; i++) {
        for (int j = 0; j < m.dim().second; j++) {
            os << std::setw(DP_WIDTH) << m.at(i, j);
        }
        os << "\n";
    }
    return os;
}

}  // namespace MatMulImpl
