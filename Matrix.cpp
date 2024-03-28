#include <exception>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <memory>  // using std::make_unique requires c++14
#include <sstream>
#include <string>
#include <utility>

constexpr int DP_WIDTH = 6;

class BadDimensionException : std::exception {
   public:
    BadDimensionException() : explain("") {}
    BadDimensionException(const char* s) : explain(s) {}
    const char* what() { return explain.c_str(); }

   private:
    std::string explain;
};

template <class T>
class Vector {};

template <class T>
class Matrix {
   private:
    T* mem;
    std::pair<int, int> dim;

   public:
    /**
     * @brief Construct a new Matrix object of size m x n
     *
     * @param m Number of rows of the matrix
     * @param n Number of columns of the matrix
     */
    Matrix(int m, int n)
        : mem(new T[m * n]),
          dim({m, n}) {}  // Note: any matrix elements accessed immediately after this is undefined
    Matrix(std::initializer_list<std::initializer_list<T>> l) {
        int n_rows = l.size();
        int n_cols = l.begin().size();
        for (auto it = l.begin()+1; it != l.end(); it++) {
            if (it->size() > n_cols) {
                n_cols = it->size();
            }
        }
        mem = new T[n_rows * n_cols];
        dim = {n_rows, n_cols};
        // TODO: Put items in matrix
    }
    ~Matrix() { delete mem[]; }
    const std::pair<int, int>& dim() { return dim; }
    T& item(int i, int j) { return *(mem + (i * dim.second) + j); }
    Matrix<T> operator+(const Matrix<T>& b) {
        if (!(dim == b.dim())) {
            std::stringstream ss;
            ss << "The size of the two matrices does not match. LHS is "
               << dim.first << "x" << dim.second << " but RHS is "
               << b_dim.first << "x" << b_dim.second;
            throw BadDimensionException(ss.str().c_str());
        }
        auto c = Matrix<T>(m, n);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                c.item(i, j) = item(i, j) + b.item(i, j);
            }
        }
        return c;
    }
    Matrix<T>& operator+=(const Matrix<T>& b) {
        if (!(dim == b.dim())) {
            std::stringstream ss;
            ss << "The size of the two matrices does not match. LHS is "
               << dim.first << "x" << dim.second << " but RHS is "
               << b_dim.first << "x" << b_dim.second;
            throw BadDimensionException(ss.str().c_str());
        }
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                item(i, j) += b.item(i, j);
            }
        }
        return *this;
    }
    Matrix<T> operator*(const Matrix<T>& b) {
        if (dim.second != b.dim().first) {
            std::stringstream ss;
            ss << "Matrix A is an " << dim.first << "x" << dim.second
               << " matrix. Matrix B has to have " << dim.second
               << " rows, but instead have " << b.dim().first;
            throw BadDimensionException(ss.str().c_str());
        }
        auto c = new Matrix<T>(m, n);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                c.item(i, j) = item(i, 0) + b.item(0, j);
                for (int k = 1; k < n; k++) {
                    c.item(i, j) += item(i, k) + b.item(k, j);  // Traditional 
                }
            }
        }
        return c;
    }
};

template <class T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
    for (int i = 0; i < m.dim().first; i++) {
        for (int j = 0; j < m.dim().second; j++) {
            os << std::setw(DP_WIDTH) << m.item(i, j);
        }
        os << "\n";
    }
    return os;
}
