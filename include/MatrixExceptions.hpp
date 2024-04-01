#ifndef MATRIXEXCEPTIONS_HPP
#define MATRIXEXCEPTIONS_HPP

#include <exception>
#include <string>
#include "Matrix_types.hpp"


namespace MatMulImpl {

class BadDimensionException : std::exception {
   public:
    BadDimensionException();
    BadDimensionException(const char* s);
    BadDimensionException(const Dim_t& a);
    BadDimensionException(const Dim_t& a, const Dim_t& b);
    const char* what() const noexcept override;

   private:
    std::string explain;
};

class OutOfBoundsException : std::exception {
   public:
    OutOfBoundsException();
    OutOfBoundsException(const char* s);
    OutOfBoundsException(int i, int j, const Dim_t& dim, const Dim_t& main_dim);
    const char* what() const noexcept override;

   private:
    std::string explain;
};

class NullPtrException : std::exception {
   public:
    NullPtrException();
    NullPtrException(const char* s);
    const char* what() const noexcept override;

   private:
    std::string explain;
};

class TooManyInitializersException : std::exception {
   public:
    TooManyInitializersException();
    TooManyInitializersException(const char* s);
    const char* what() const noexcept override;

   private:
    std::string explain;
};

}  // namespace MatMulImpl

#endif  // MATRIXEXCEPTIONS_HPP