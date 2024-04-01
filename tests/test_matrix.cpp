#include <gtest/gtest.h>

#include "Matrix.hpp"
#include "MatrixExceptions.hpp"

// add classes to test here
using MatrixTypes =
    ::testing::Types<MatMulImpl::Matrix<int>, MatMulImpl::Matrix<float>>;

// setup
template <typename T>
class MATRIX_BASE : public ::testing::Test {
   protected:
    using MatrixType = T;
};
template <typename T>
class MATRIX : public MATRIX_BASE<T> {};
TYPED_TEST_CASE(MATRIX_BASE, MatrixTypes);
TYPED_TEST_CASE(MATRIX, MatrixTypes);

// tests
TYPED_TEST(MATRIX_BASE, DIM) {
    typename TestFixture::MatrixType m(3, 3);
    EXPECT_EQ(m.dim(), std::make_pair(3, 3));
}

TYPED_TEST(MATRIX_BASE, AT) {
    typename TestFixture::MatrixType m(3, 3);
    m.at(1, 1) = 5;
    EXPECT_EQ(m.at(1, 1), 5);

    EXPECT_EQ(m.at(0, 0), 0);

    EXPECT_THROW(m.at(3, 3), MatMulImpl::OutOfBoundsException);
}

TYPED_TEST(MATRIX_BASE, EQUALITY) {
    typename TestFixture::MatrixType m1({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 3),
        m2({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 3),
        m3({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 4),
        m4({{1, 2, 3}, {4, 5, 6}, {7, 8, 10}}, 3);

    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 == m3);
    EXPECT_FALSE(m1 == m4);
}

TYPED_TEST(MATRIX_BASE, ADD) {
    typename TestFixture::MatrixType m1({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 3),
        m2({{9, 8, 7}, {6, 5, 4}, {3, 2, 1}}, 3),
        m3({{10, 10, 10}, {10, 10, 10}, {10, 10, 10}}, 3),
        m4({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 4);

    EXPECT_EQ(m1 + m2, m3);

    EXPECT_THROW(m1 + m4, MatMulImpl::BadDimensionException);
}

TYPED_TEST(MATRIX_BASE, CONSTANT_MUL) {
    typename TestFixture::MatrixType m1({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 3),
        m2({{2, 4, 6}, {8, 10, 12}, {14, 16, 18}}, 3);

    EXPECT_EQ(m1 * 2, m2);
}

TYPED_TEST(MATRIX_BASE, MATRIX_MUL) {
    typename TestFixture::MatrixType m1({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 3),
        m2({{9, 8, 7}, {6, 5, 4}, {3, 2, 1}}, 3),
        m3({{30, 24, 18}, {84, 69, 54}, {138, 114, 90}}, 3);

    EXPECT_EQ(m1 * m2, m3);

    typename TestFixture::MatrixType m4({{1, 2, 3}, {4, 5, 6}}, 3),
        m5({{9, 8, 7}, {6, 5, 4}, {3, 2, 1}}, 3),
        m6({{30, 24, 18}, {84, 69, 54}}, 3);

    EXPECT_EQ(m4 * m5, m6);

    typename TestFixture::MatrixType m7({{1, 2, 3}, {4, 5, 6}}, 3),
        m8({{9, 8, 7, 11}, {6, 5, 4, 12}, {3, 2, 1, 13}}, 4),
        m9({{30, 24, 18, 74}, {84, 69, 54, 182}}, 4);

    EXPECT_EQ(m7 * m8, m9);

    typename TestFixture::MatrixType m10({{1, 2, 3}, {4, 5, 6}}, 3),
        m11({{9, 8, 7, 11}, {6, 5, 4, 12}, {3, 2, 1, 13}, {1, 2, 3, 4}}, 4);

    EXPECT_THROW(m10 * m11, MatMulImpl::BadDimensionException);
}

TYPED_TEST(MATRIX, CONSTRUCTOR_M_N) {
    typename TestFixture::MatrixType m(3, 3);
    EXPECT_EQ(m.dim(), std::make_pair(3, 3));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(m.at(i, j), 0);
        }
    }

    typename TestFixture::MatrixType m2(3, 4);
    EXPECT_EQ(m2.dim(), std::make_pair(3, 4));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_EQ(m2.at(i, j), 0);
        }
    }

    EXPECT_THROW(typename TestFixture::MatrixType(0, 0),
                 MatMulImpl::BadDimensionException);
    EXPECT_THROW(typename TestFixture::MatrixType(1, 0),
                 MatMulImpl::BadDimensionException);
    EXPECT_THROW(typename TestFixture::MatrixType(-1, 10),
                 MatMulImpl::BadDimensionException);
}

TYPED_TEST(MATRIX, CONSTRUCTOR_2D_LIST) {
    typename TestFixture::MatrixType m({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 3);
    EXPECT_EQ(m.dim(), std::make_pair(3, 3));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(m.at(i, j), i * 3 + j + 1);
        }
    }

    typename TestFixture::MatrixType m2({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 4);
    EXPECT_EQ(m2.dim(), std::make_pair(3, 4));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            if (j < 3) {
                EXPECT_EQ(m2.at(i, j), i * 3 + j + 1);
            } else {
                EXPECT_EQ(m2.at(i, j), 0);
            }
        }
    }

    typename TestFixture::MatrixType m3({{0, 0}, {}, {}}, 4);
    EXPECT_EQ(m3.dim(), std::make_pair(3, 4));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_EQ(m3.at(i, j), 0);
        }
    }

    EXPECT_THROW(typename TestFixture::MatrixType({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 2),
                 MatMulImpl::TooManyInitializersException);
}

TYPED_TEST(MATRIX, CONSTRUCTOR_1D_LIST) {
    typename TestFixture::MatrixType m({1, 2, 3, 4, 5, 6, 7, 8, 9}, 3);
    EXPECT_EQ(m.dim(), std::make_pair(3, 3));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(m.at(i, j), i * 3 + j + 1);
        }
    }

    typename TestFixture::MatrixType m2({1, 2, 3, 4, 5, 6, 7, 8, 9}, 4);
    EXPECT_EQ(m2.dim(), std::make_pair(3, 4));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            if (i * 4 + j < 9) {
                EXPECT_EQ(m2.at(i, j), i * 4 + j + 1);
            } else {
                EXPECT_EQ(m2.at(i, j), 0);
            }
        }
    }

    typename TestFixture::MatrixType m3({0}, 4);
    EXPECT_EQ(m3.dim(), std::make_pair(1, 4));

    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_EQ(m3.at(i, j), 0);
        }
    }

    typename TestFixture::MatrixType m4({0}, 4, 3);
    EXPECT_EQ(m4.dim(), std::make_pair(3, 4));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_EQ(m4.at(i, j), 0);
        }
    }

    EXPECT_THROW(typename TestFixture::MatrixType({1, 2, 3, 4, 5, 6, 7, 8, 9}, 2, 2),
                 MatMulImpl::TooManyInitializersException);
}