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
#include <sstream>

#include "MatrixBase.hpp"
#include "MatrixExceptions.hpp"
#include "MatrixView.hpp"

namespace MatMulImpl {

template <class T>
class Matrix : public MatrixBase<T> {
   public:
    Matrix(int n) : MatrixBase<T>(1, 1) {
        if (n == 0) {
            std::stringstream ss;
            ss << "Not implemented! Unexpected initialization of Matrix(0)\n"
               << "Probable cause: Initialize Matrix of Matrix with missing "
                  "elements\n"
               << "Solution: Provide the missing elements\n"
               << "Example:\n"
               << "Wrong: Matrix<Matrix<T>> m({{m1}, {m3, m4}}, 2)\n"
               << "Cause: m2 is missing\n"
               << "Correct: Matrix<Matrix<T>> m({{m1, m2}, {m3, m4}}, 2)\n";

            throw NotImplementedException(ss.str().c_str());
        }

        throw NotImplementedException();
    }

    Matrix(int m, int n) : MatrixBase<T>(m, n) {
        this->_mem = std::shared_ptr<std::shared_ptr<T>[]>(
            new std::shared_ptr<T>[this->_dim.first * this->_dim.second]);

        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                this->_at(i, j) = std::shared_ptr<T>(new T(0));
    }

    Matrix(Matrix<T>&& mat)
        : MatrixBase<T>(mat.dim().first, mat.dim().second), _mem(mat._mem) {
        if (this != &mat) {
            mat._mem = nullptr;
        }
    }

    Matrix(const Matrix<T>& mat)
        : MatrixBase<T>(mat.dim().first, mat.dim().second) {
        this->_mem = std::shared_ptr<std::shared_ptr<T>[]>(
            new std::shared_ptr<T>[mat.dim().first * mat.dim().second]);

        for (int i = 0; i < mat.dim().first; i++)
            for (int j = 0; j < mat.dim().second; j++)
                this->at(i, j) = mat.at(i, j);
    }

    Matrix(std::initializer_list<std::initializer_list<T>>, int) = default;
    /**
     * @brief Construct a new Matrix< T>:: Matrix object
     *
     * @tparam T
     * @param l 2d initializer list
     * @param cols number of columns
     */
    Matrix(std::initializer_list<std::initializer_list<T>> l, int cols)
        : MatrixBase<T>(l.size(), cols) {
        this->_mem = std::shared_ptr<std::shared_ptr<T>[]>(
            new std::shared_ptr<T>[this->_dim.first * this->_dim.second]);

        for (int i = 0; i < this->_dim.first; i++) {
            if (l.begin()[i].size() > this->_dim.second) {
                throw TooManyInitializersException();
            }

            for (int j = 0; j < this->_dim.second; j++) {
                if (j < l.begin()[i].size()) {
                    this->_at(i, j) =
                        std::shared_ptr<T>(new T(l.begin()[i].begin()[j]));
                } else {
                    this->_at(i, j) = std::shared_ptr<T>(new T(0));
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
        this->_mem = std::shared_ptr<std::shared_ptr<T>[]>(
            new std::shared_ptr<T>[this->_dim.first * this->_dim.second]);

        for (int i = 0; i < this->_dim.first; i++)
            for (int j = 0; j < this->_dim.second; j++) {
                if (i * cols + j < l.size())
                    this->_at(i, j) =
                        std::shared_ptr<T>(new T(l.begin()[i * cols + j]));
                else
                    this->_at(i, j) = std::shared_ptr<T>(new T(0));
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

        this->_mem = std::shared_ptr<std::shared_ptr<T>[]>(
            new std::shared_ptr<T>[this->_dim.first * this->_dim.second]);

        for (int i = 0; i < this->_dim.first; i++)
            for (int j = 0; j < this->_dim.second; j++) {
                if (i * cols + j < l.size())
                    this->_at(i, j) =
                        std::shared_ptr<T>(new T(l.begin()[i * cols + j]));
                else
                    this->_at(i, j) = std::shared_ptr<T>(new T(0));
            }
    }

   protected:
   private:
    std::shared_ptr<std::shared_ptr<T>[]> _mem;

    /**
     * @brief Get the memory pointer
     *
     * @tparam T
     * @return std::shared_ptr<std::shared_ptr<T>[]>
     */
    std::shared_ptr<std::shared_ptr<T>[]> _get_mem() const override {
        return this->_mem;
    }
};

}  // namespace MatMulImpl
