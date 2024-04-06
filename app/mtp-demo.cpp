#include <iostream>
#include "Matrixv2.hpp"
#include "Algorithms.hpp"
#include "Generator.hpp"

using namespace MatMulImpl;
using Mtp = MatMulImpl::Multiplication;

int main(int argc, char const *argv[])
{
    for (int i = 1; i <= 10; i++) {
        Matrix2<int> A(MatrixGenerator<int>::random_fill(i, i));
        std::cout << "A:\n" << A;
        Matrix2<int> I(Matrix2<int>::identity(i));
        std::cout << "I2:\n" << I;
        std::cout << "A*I2:\n" << A*I;
        std::cout << "A*I2(Divide and conquer):\n" << Mtp::div_and_conquer_sq2(A, I);
        std::cout << "A*I2(Strassen):\n" << Mtp::strassen(A, I);
    }
    return 0;
}

