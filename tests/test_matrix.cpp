#include <gtest/gtest.h>

#include "Matrix.hpp"
#include "MatrixExceptions.hpp"

template <typename T>
using MatrixClass = MatMulImpl::Matrix<T>;  // add class to test here

using MatrixTypes = ::testing::Types<int>;

// setup
template <typename T>
class MATRIX_BASE : public ::testing::Test {
   protected:
    using MatrixType = T;
};

template <typename T>
class MATRIX : public MATRIX_BASE<T> {};
template <typename T>
class MATRIX_VIEW : public MATRIX_BASE<T> {};

TYPED_TEST_CASE(MATRIX_BASE, MatrixTypes);
TYPED_TEST_CASE(MATRIX, MatrixTypes);
TYPED_TEST_CASE(MATRIX_VIEW, MatrixTypes);

// tests
TYPED_TEST(MATRIX_BASE, DIM) {
    MatrixClass<TypeParam> m(3, 3);
    EXPECT_EQ(m.dim(), std::make_pair(3, 3));
}

TYPED_TEST(MATRIX_BASE, AT) {
    MatrixClass<TypeParam> m(3, 3);
    m.at(1, 1) = 5;
    EXPECT_EQ(m.at(1, 1), 5);

    EXPECT_EQ(m.at(0, 0), 0);

    EXPECT_THROW(m.at(3, 3), MatMulImpl::OutOfBoundsException);
}

TYPED_TEST(MATRIX_BASE, EQUALITY) {
    MatrixClass<TypeParam> m1({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 3),
        m2({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 3),
        m3({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 4),
        m4({{1, 2, 3}, {4, 5, 6}, {7, 8, 10}}, 3);

    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 == m3);
    EXPECT_FALSE(m1 == m4);
}

TYPED_TEST(MATRIX_BASE, ADD) {
    MatrixClass<TypeParam> m1({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 3),
        m2({{9, 8, 7}, {6, 5, 4}, {3, 2, 1}}, 3),
        m3({{10, 10, 10}, {10, 10, 10}, {10, 10, 10}}, 3),
        m4({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 4);

    EXPECT_EQ(m1 + m2, m3);

    EXPECT_THROW(m1 + m4, MatMulImpl::BadDimensionException);
}

TYPED_TEST(MATRIX_BASE, CONSTANT_MUL) {
    MatrixClass<TypeParam> m1({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 3),
        m2({{2, 4, 6}, {8, 10, 12}, {14, 16, 18}}, 3);

    EXPECT_EQ(m1 * (TypeParam)2, m2);
    EXPECT_EQ((TypeParam)2 * m1, m2);
}

TYPED_TEST(MATRIX_BASE, MATRIX_MUL) {
    MatrixClass<TypeParam> m1({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 3),
        m2({{9, 8, 7}, {6, 5, 4}, {3, 2, 1}}, 3),
        m3({{30, 24, 18}, {84, 69, 54}, {138, 114, 90}}, 3);

    EXPECT_EQ(m1 * m2, m3);

    MatrixClass<TypeParam> m4({{1, 2, 3}, {4, 5, 6}}, 3),
        m5({{9, 8, 7}, {6, 5, 4}, {3, 2, 1}}, 3),
        m6({{30, 24, 18}, {84, 69, 54}}, 3);

    EXPECT_EQ(m4 * m5, m6);

    MatrixClass<TypeParam> m7({{1, 2, 3}, {4, 5, 6}}, 3),
        m8({{9, 8, 7, 11}, {6, 5, 4, 12}, {3, 2, 1, 13}}, 4),
        m9({{30, 24, 18, 74}, {84, 69, 54, 182}}, 4);

    EXPECT_EQ(m7 * m8, m9);

    MatrixClass<TypeParam> m10({{1, 2, 3}, {4, 5, 6}}, 3),
        m11({{9, 8, 7, 11}, {6, 5, 4, 12}, {3, 2, 1, 13}, {1, 2, 3, 4}}, 4);

    EXPECT_THROW(m10 * m11, MatMulImpl::BadDimensionException);
}

TYPED_TEST(MATRIX_BASE, MATRIX_FLATTEN) {
    MatMulImpl::Matrix<int> m1({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 3);
    MatMulImpl::Matrix<MatMulImpl::Matrix<int>> m2({{m1, m1}, {m1, m1}}, 2);

    auto m3 = MatMulImpl::Matrix<int>::flatten(m2);

    EXPECT_EQ(m3.dim(), std::make_pair(6, 6));

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            EXPECT_EQ(m3.at(i, j), m1.at(i % 3, j % 3));
        }
    }
}

TYPED_TEST(MATRIX, CONSTRUCTOR_M_N) {
    MatrixClass<TypeParam> m(3, 3);
    EXPECT_EQ(m.dim(), std::make_pair(3, 3));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(m.at(i, j), 0);
        }
    }

    MatrixClass<TypeParam> m2(3, 4);
    EXPECT_EQ(m2.dim(), std::make_pair(3, 4));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_EQ(m2.at(i, j), 0);
        }
    }

    EXPECT_THROW(MatrixClass<TypeParam>(0, 0),
                 MatMulImpl::BadDimensionException);
    EXPECT_THROW(MatrixClass<TypeParam>(1, 0),
                 MatMulImpl::BadDimensionException);
    EXPECT_THROW(MatrixClass<TypeParam>(-1, 10),
                 MatMulImpl::BadDimensionException);
}

TYPED_TEST(MATRIX, CONSTRUCTOR_2D_LIST) {
    MatrixClass<TypeParam> m({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 3);
    EXPECT_EQ(m.dim(), std::make_pair(3, 3));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(m.at(i, j), i * 3 + j + 1);
        }
    }

    MatrixClass<TypeParam> m2({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 4);
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

    MatrixClass<TypeParam> m3({{0, 0}, {}, {}}, 4);
    EXPECT_EQ(m3.dim(), std::make_pair(3, 4));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_EQ(m3.at(i, j), 0);
        }
    }

    EXPECT_THROW(MatrixClass<TypeParam>({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 2),
                 MatMulImpl::TooManyInitializersException);
}

TYPED_TEST(MATRIX, CONSTRUCTOR_1D_LIST) {
    MatrixClass<TypeParam> m({1, 2, 3, 4, 5, 6, 7, 8, 9}, 3);
    EXPECT_EQ(m.dim(), std::make_pair(3, 3));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(m.at(i, j), i * 3 + j + 1);
        }
    }

    MatrixClass<TypeParam> m2({1, 2, 3, 4, 5, 6, 7, 8, 9}, 4);
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

    MatrixClass<TypeParam> m3({0}, 4);
    EXPECT_EQ(m3.dim(), std::make_pair(1, 4));

    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_EQ(m3.at(i, j), 0);
        }
    }

    MatrixClass<TypeParam> m4({0}, 4, 3);
    EXPECT_EQ(m4.dim(), std::make_pair(3, 4));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_EQ(m4.at(i, j), 0);
        }
    }

    EXPECT_THROW(MatrixClass<TypeParam>({1, 2, 3, 4, 5, 6, 7, 8, 9}, 2, 2),
                 MatMulImpl::TooManyInitializersException);
}

TYPED_TEST(MATRIX_VIEW, SUBMATRIX) {
    MatrixClass<TypeParam> m({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 3);
    auto m2 = m.sub(1, 1, 2, 2);

    EXPECT_EQ(m2.dim(), std::make_pair(1, 1));

    EXPECT_EQ(m2.at(0, 0), 9);

    EXPECT_THROW(m2.at(1, 1), MatMulImpl::OutOfBoundsException);

    EXPECT_THROW(m.sub(1, 1, 3, 3), MatMulImpl::OutOfBoundsException);
    EXPECT_THROW(m.sub(2, 2, 2, 2), MatMulImpl::OutOfBoundsException);
    EXPECT_THROW(m.sub(-1, 1, 1, 1), MatMulImpl::BadDimensionException);

    auto m3 = m.sub(2, 2, 1, 1);

    EXPECT_EQ(m3.dim(), std::make_pair(2, 2));

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            EXPECT_EQ(m.at(i + 1, j + 1), m3.at(i, j));
        }
    }

    MatrixClass<TypeParam> m4(
        {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
         13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
        5);

    auto m5 = m4.sub(3, 3, 2, 2);

    EXPECT_EQ(m5.dim(), std::make_pair(3, 3));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(m4.at(i + 2, j + 2), m5.at(i, j));
        }
    }

    EXPECT_THROW(m4.sub(4, 4, 2, 2), MatMulImpl::OutOfBoundsException);

    {
        MatrixClass<TypeParam> m6({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 3);
        m5 = m6.sub(1, 1, 2, 2);

        EXPECT_EQ(m5.dim(), std::make_pair(1, 1));

        EXPECT_EQ(m5.at(0, 0), 9);
    }

    EXPECT_THROW(m5.at(0, 0), MatMulImpl::NullPtrException);
}

TYPED_TEST(MATRIX_VIEW, SUBMATRIX_OF_SUBMATRIX) {
    MatrixClass<TypeParam> m(
        {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
         13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
        5);

    auto m2 = m.sub(4, 4, 1, 1);

    EXPECT_EQ(m2.dim(), std::make_pair(4, 4));

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_EQ(m.at(i + 1, j + 1), m2.at(i, j));
        }
    }

    auto m3 = m2.sub(3, 3, 1, 1);
    EXPECT_EQ(m3.dim(), std::make_pair(3, 3));

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(m2.at(i + 1, j + 1), m3.at(i, j));
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(m.at(i + 2, j + 2), m3.at(i, j));
        }
    }

    EXPECT_THROW(m3.sub(2, 2, 2, 2), MatMulImpl::OutOfBoundsException);
    EXPECT_THROW(m3.sub(3, 0, 2, 2), MatMulImpl::BadDimensionException);

    auto m4 = m3.sub(2, 2, 1, 1);

    EXPECT_EQ(m4.dim(), std::make_pair(2, 2));

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            EXPECT_EQ(m3.at(i + 1, j + 1), m4.at(i, j));
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            EXPECT_EQ(m.at(i + 3, j + 3), m4.at(i, j));
        }
    }

    EXPECT_THROW(m4.at(0, 3), MatMulImpl::OutOfBoundsException);

    {
        MatrixClass<TypeParam> m5(
            {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
             13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
            5);

        m2 = m5.sub(3, 4, 2, 1);

        EXPECT_EQ(m2.dim(), std::make_pair(3, 4));

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                EXPECT_EQ(m5.at(i + 2, j + 1), m2.at(i, j));
            }
        }

        m3 = m2.sub(2, 2, 1, 2);

        EXPECT_EQ(m3.dim(), std::make_pair(2, 2));

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                EXPECT_EQ(m2.at(i + 1, j + 2), m3.at(i, j));
            }
        }

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                EXPECT_EQ(m5.at(i + 3, j + 3), m3.at(i, j));
            }
        }
    }

    EXPECT_THROW(m2.at(0, 0), MatMulImpl::NullPtrException);
    EXPECT_THROW(m3.at(0, 0), MatMulImpl::NullPtrException);
}

TYPED_TEST(MATRIX_VIEW, SUBMATRIX_ARITHMETIC) {
    MatrixClass<TypeParam> m(
        {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
         13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
        5);

    auto m2 = m.sub(3, 3, 1, 1);
    auto m3 = m.sub(3, 3, 2, 2), m4 = m2.sub(2, 2, 1, 1),
         m5 = m3.sub(2, 2, 0, 0);

    MatrixClass<TypeParam> m6({{18, 20, 22}, {28, 30, 32}, {38, 40, 42}}, 3),
        m7({{24, 26}, {34, 36}}, 2);

    EXPECT_EQ(m2.dim(), std::make_pair(3, 3));
    EXPECT_EQ(m3.dim(), std::make_pair(3, 3));
    EXPECT_EQ(m4.dim(), std::make_pair(2, 2));
    EXPECT_EQ(m5.dim(), std::make_pair(2, 2));

    EXPECT_EQ(m2 + m3, m6);
    EXPECT_EQ(m4 + m5, m7);
    EXPECT_EQ(m4 + m5, (TypeParam)2 * m4);

    EXPECT_THROW(m2 + m4, MatMulImpl::BadDimensionException);
    EXPECT_THROW(m3 + m5, MatMulImpl::BadDimensionException);

    MatrixClass<TypeParam> m8(
        {{367, 388, 409}, {622, 658, 694}, {877, 928, 979}}, 3),
        m9({{365, 390}, {510, 545}}, 2);

    EXPECT_EQ(m2 * m3, m8);
    EXPECT_EQ(m4 * m5, m9);
    EXPECT_EQ(m4 * m5, m4 * m4);

    EXPECT_THROW(m2 * m4, MatMulImpl::BadDimensionException);
    EXPECT_THROW(m3 * m5, MatMulImpl::BadDimensionException);
}