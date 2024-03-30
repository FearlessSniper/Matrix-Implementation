#include "Generator.hpp"
#include "Matrixv2.hpp"

using namespace MatMulImpl;

int main() {
    auto a = Matrix2<double>::from(
        {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10.2, 11.9, 12}, {13, 14, 15}});
    auto b = Matrix2<double>::from({{1, 0, 0}, {0, 1, 0}, {0, 0, 1}});
    std::cout << a << "\n";
    std::cout << a.sub(0, 0, 2, 2) << '\n';
    std::cout << a.sub(1, 1, 2, 2) << '\n';
    std::cout << b << "\n";
    // std::cout << a+b << "\n";
    std::cout << a * b << "\n";
    std::cout << a * Matrix2<double>::identity(3) << '\n';
    auto c = MatrixGenerator<double>::random_fill(12, 16);
    c.print_width = 10;
    std::cout << c << '\n';
}
