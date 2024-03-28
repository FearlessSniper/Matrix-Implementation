#include <exception>
#include <string>
#include <utility>
#include <sstream>

class BadDimensionException : std::exception {
   public:
    BadDimensionException(const char* s) : explain(s) {}
    const char *what() {
        return explain.c_str();
    }
   private:
    std::string explain;
};

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
    Matrix(int m, int n) {
        mem = new T[m][n];
        dim = {m, n};
    }
    ~Matrix() { delete mem[]; }
    const std::pair<int, int>& dim() { return dim; }
    Matrix<T>& operator+(const Matrix<T>& b) {
        auto b_dim = b.dim();
        if (!(dim == b_dim)) {
            std::stringstream ss;
            ss << "The size of the two matrices does not match. LHS is " << dim.first << "x" << dim.second
               << " but RHS is " << b_dim.first << "x" << b_dim.second;
            throw BadDimensionException(ss.str().c_str());
        }
        auto c = new Matrix<T>(m, n);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                c->mem[i][j] = mem[i][j] + b.mem[i][j];
            }
        }
        return *c;
    }
};
