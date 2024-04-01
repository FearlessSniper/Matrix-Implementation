
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
        return MatrixView<T>(this->get_mem(), n, m, off_x, off_y,
                             this->_main_dim);
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
    MatrixBase(int n, int m) {
        if (n <= 0 || m <= 0) {
            throw BadDimensionException(std::make_pair(n, m));
        }
        this->_dim = std::make_pair(n, m);
    }
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
    Dim_t _offset = std::make_pair(0, 0);

   private:
    /**
     * @brief Get the memory pointer
     *
     * @tparam T
     * @return std::shared_ptr<T[]>
     */
    std::shared_ptr<T[]> virtual _get_mem() const { return nullptr; }
};

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
