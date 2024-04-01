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
    Matrix(int m, int n) : MatrixBase<T>(m, n) {
        this->_mem = std::shared_ptr<T[]>(new T[m * n]);

        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) this->at(i, j) = 0;
    }

    Matrix(Matrix<T>&& mat) = default;
    /**
     * @brief Construct a new Matrix< T>:: Matrix object
     *
     * @tparam T
     * @param l 2d initializer list
     * @param cols number of columns
     */
    Matrix(std::initializer_list<std::initializer_list<T>> l, int cols)
        : MatrixBase<T>(l.size(), cols) {
        this->_mem =
            std::shared_ptr<T[]>(new T[this->_dim.first * this->_dim.second]);

        for (int i = 0; i < this->_dim.first; i++) {
            if (l.begin()[i].size() > this->_dim.second) {
                throw TooManyInitializersException();
            }

            for (int j = 0; j < this->_dim.second; j++) {
                if (j < l.begin()[i].size()) {
                    this->at(i, j) = l.begin()[i].begin()[j];
                } else {
                    this->at(i, j) = 0;
                }
            }
        }
    }
    /**
     * @brief Construct a new Matrix< T>:: Matrix object
     *
     * @tparam T
     * @param l 1d initializer list
     * @param cols number of columns
     */
    Matrix(std::initializer_list<T> l, int cols)
        : MatrixBase<T>((l.size() + cols - 1) / cols, cols) {
        this->_mem =
            std::shared_ptr<T[]>(new T[this->_dim.first * this->_dim.second]);

        for (int i = 0; i < this->_dim.first; i++)
            for (int j = 0; j < this->_dim.second; j++) {
                if (i * cols + j < l.size())
                    this->at(i, j) = l.begin()[i * cols + j];
                else
                    this->at(i, j) = 0;
            }
    }

    /**
     * @brief Construct a new Matrix< T>:: Matrix object
     *
     * @tparam T
     * @param l 1d initializer list
     * @param cols number of columns
     * @param rows number of rows
     */
    Matrix(std::initializer_list<T> l, int cols, int rows)
        : MatrixBase<T>(rows, cols) {
        int sz = rows * cols;
        if (l.size() > sz) throw TooManyInitializersException();

        this->_mem =
            std::shared_ptr<T[]>(new T[this->_dim.first * this->_dim.second]);

        for (int i = 0; i < this->_dim.first; i++)
            for (int j = 0; j < this->_dim.second; j++) {
                if (i * cols + j < l.size())
                    this->at(i, j) = l.begin()[i * cols + j];
                else
                    this->at(i, j) = 0;
            }
    }

   private:
    std::shared_ptr<T[]> _mem;

    /**
     * @brief Get the memory pointer
     *
     * @tparam T
     * @return std::shared_ptr<T[]>
     */
    std::shared_ptr<T[]> _get_mem() const override { return this->_mem; }
};

}  // namespace MatMulImpl
