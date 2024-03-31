#include "MatrixExceptions.hpp"

#include <sstream>

namespace MatMulImpl {
BadDimensionException::BadDimensionException() : explain("") {}
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

}  // namespace MatMulImpl