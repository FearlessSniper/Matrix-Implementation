#include <iostream>
#include <chrono>

#include "Algorithms.hpp"
#include "Generator.hpp"
#include "Matrixv2.hpp"

using namespace MatMulImpl;
using Mtp = MatMulImpl::Multiplication;

int main(int argc, char const *argv[]) {

    double ratio1_sum = 0;
    double ratio2_sum = 0;
    for (int j = 0; j < 10; j++) {
        for (int i = 1; i <= 128; i *= 2) {
            Matrix2<int> A(MatrixGenerator<int>::random_fill(i, i));
            Matrix2<int> B(MatrixGenerator<int>::random_fill(i, i));
            auto t_start = std::chrono::high_resolution_clock::now();


            auto &&C1 = A * B;

            auto t_end_c1 = std::chrono::high_resolution_clock::now();

            auto &&C2 = Mtp::strassen(A, B);

            auto t_end_c2 = std::chrono::high_resolution_clock::now();


            auto &&C3 = Mtp::div_and_conquer_sq2(A, B);

            auto t_end_c3 = std::chrono::high_resolution_clock::now();
            if (!(C2 == C1)) {
                std::cout << "Divide and conquer failed for " << i << "x" << i
                          << "\n";
                std::cout << "A:\n" << A;
                std::cout << "B:\n" << B;
                return 1;
            }
            if (!(C3 == C1)) {
                std::cout << "Strassen failed for " << i << "x" << i << "\n";
                std::cout << "A:\n" << A;
                std::cout << "B:\n" << B;
                return 1;
            }
            std::cout << "Test passed for " << i << "x" << i << "\n";
            auto t1 = std::chrono::duration_cast<std::chrono::microseconds>(
                t_end_c1 - t_start);
            auto t2 = std::chrono::duration_cast<std::chrono::microseconds>(
                t_end_c2 - t_end_c1);
            auto t3 = std::chrono::duration_cast<std::chrono::microseconds>(
                t_end_c3 - t_end_c2);

                double ratio1 = (double)t1.count() / t2.count();
                double ratio2 = (double)t1.count() / t3.count();

                std::cout << "Naive: " << t1.count() << "us\n";
                std::cout << "Strassen: " << t2.count() << "us\n";
                std::cout << "Divide and conquer: " << t3.count() << "us\n";
                std::cout << "Strassen speedup: " << ratio1 << "x\n";
                std::cout << "Divide and conquer speedup: " << ratio2 << "x\n";

                ratio1_sum += ratio1;
                ratio2_sum += ratio2;
        }
    }

    std::cout << "Average Strassen speedup: " << ratio1_sum / 10 << "x\n";
    std::cout << "Average Divide and conquer speedup: " << ratio2_sum / 10 << "x\n";
    // for (int i = 1; i <= 10; i++) {
    //     Matrix2<int> A(MatrixGenerator<int>::random_fill(i, i));
    //     std::cout << "A:\n" << A;
    //     Matrix2<int> I(Matrix2<int>::identity(i));
    //     std::cout << "I2:\n" << I;
    //     std::cout << "A*I2:\n" << A * I;
    //     std::cout << "A*I2(Divide and conquer):\n"
    //               << Mtp::div_and_conquer_sq2(A, I);
    //     std::cout << "A*I2(Strassen):\n" << Mtp::strassen(A, I);
    // }
    return 0;
}
