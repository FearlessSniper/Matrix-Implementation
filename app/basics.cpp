#include "Matrix.hpp"

using namespace MatMulImpl;

int main() {
    auto a = Matrix<int>::from({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    auto b = Matrix<int>::from({{1, 0, 0}, {0, 1, 0}, {0, 0, 1}});
    auto c1 = a+b;
    auto c2 = a*b;
    std::cout << a << "\n";
    std::cout << b << "\n";
    std::cout << c1 << "\n";
    std::cout << c2 << "\n";

    
}
