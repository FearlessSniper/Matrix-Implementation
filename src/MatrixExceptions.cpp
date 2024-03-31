#include "MatrixExceptions.hpp"

#include <sstream>

namespace MatMulImpl {
BadDimensionException::BadDimensionException() : explain("Bad dimension") {}
BadDimensionException::BadDimensionException(const char* s) : explain(s) {}
BadDimensionException::BadDimensionException(const Dim_t& a, const Dim_t& b) {
    std::stringstream ss;
    ss << "Bad dimension: (" << a.first << ", " << a.second << ") vs ("
       << b.first << ", " << b.second << ")";
    explain = ss.str();
}
const char* BadDimensionException::what() const noexcept {
    return explain.c_str();
}

OutOfBoundsException::OutOfBoundsException() : explain("Out of bounds") {}
OutOfBoundsException::OutOfBoundsException(const char* s) : explain(s) {}
OutOfBoundsException::OutOfBoundsException(int i, int j, Dim_t& dim,
                                           Dim_t& main_dim) {
    std::stringstream ss;
    ss << "Out of bounds: " << std::endl
       << "Index: (" << i << ", " << j << ")" << std::endl
       << "View dimension: (" << dim.first << ", " << dim.second << ")"
       << std::endl
       << "Matrix dimension: (" << main_dim.first << ", " << main_dim.second
       << ")";
    explain = ss.str();
}
const char* OutOfBoundsException::what() const noexcept {
    return explain.c_str();
}

NullPtrException::NullPtrException()
    : explain("Null Pointer: Matrix no longer exists") {}
NullPtrException::NullPtrException(const char* s) : explain(s) {}
const char* NullPtrException::what() const noexcept { return explain.c_str(); }

}  // namespace MatMulImpl