/**
 * @file Generator.hpp
 * @author aydenwong (aydenwongfs@gmail.com)
 * @brief
 * @version 0.1
 * @date 29-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <random>

#include "Matrixv2.hpp"

namespace MatMulImpl {
template <class T>
class MatrixGenerator {
   public:
    static Matrix2<T> random_fill(int m, int n) {
        std::random_device rdev;
        Matrix2<T> mat(m, n);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                mat.item(i, j) = rdev();
            }
        }
        return mat;
    }
};
template <>
class MatrixGenerator<int> {
   public:
    static Matrix2<int> random_fill(int m, int n, int minX = -10,
                                    int maxX = 10) {
        std::uniform_int_distribution<> irdist(minX, maxX);
        std::random_device rdev;
        std::mt19937 gen(rdev());
        Matrix2<int> mat(m, n);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                mat.item(i, j) = irdist(gen);
            }
        }
        return mat;
    }
};

template <>
class MatrixGenerator<double> {
   public:
    static Matrix2<double> random_fill(int m, int n) {
        std::uniform_real_distribution<> rrdist;
        std::random_device rdev;
        std::mt19937 gen(rdev());
        Matrix2<double> mat(m, n);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                mat.item(i, j) = rrdist(gen);
            }
        }
        return mat;
    }
};
}  // namespace MatMulImpl

#endif  // GENERATE_HPP
