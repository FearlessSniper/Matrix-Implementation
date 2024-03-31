/**
 * @file Matrix.cpp
 * @author aydenwong (aydenwongfs@gmail.com)
 * @brief
 * @version 0.1
 * @date 28-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "Matrix.hpp"

#include <iomanip>
#include <sstream>

#include "MatrixExceptions.hpp"

namespace MatMulImpl {

template <class T>
Matrix<T>::Matrix(int m, int n) {
    this->_dim = std::make_pair(m, n);
    this->_mem = std::shared_ptr<T[]>(new T[m * n]);
}

/**
 * @brief Construct a new Matrix< T>:: Matrix object
 *
 * @tparam T
 * @param l 2d initializer list
 * @param cols number of columns
 */
template <class T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> l, int cols) {
    this->_dim = std::make_pair(l.size(), cols);
    this->_main_dim = this->_dim;
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
template <class T>
Matrix<T>::Matrix(std::initializer_list<T> l, int cols) {
    this->_dim = std::make_pair((l.size() + cols - 1) / cols, cols);
    this->_main_dim = this->_dim;
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
 * @brief Get the memory pointer
 *
 * @tparam T
 * @return std::shared_ptr<T[]>
 */
template <class T>
std::shared_ptr<T[]> Matrix<T>::_get_mem() const {
    return this->_mem;
}

}  // namespace MatMulImpl
