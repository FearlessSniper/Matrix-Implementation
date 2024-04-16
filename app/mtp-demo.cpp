#include <chrono>
#include <iostream>

#include "Algorithms.hpp"
#include "Generator.hpp"
#include "Matrixv2.hpp"

using namespace MatMulImpl;
using Mtp = MatMulImpl::Multiplication;
const char *alg_names[] = {"naive",
                           "div_and_conquer_sq2",
                           "strassen",
                           "div_and_conquer_optimized",
                           "winograd",
                           "strassen_winograd_hybrid",
                           "div_and_conquer_winograd_hybrid"};
const std::vector<
    std::function<Matrix2<int>(const Matrix2<int> &, const Matrix2<int> &)>>
    algos({&Mtp::naive<int>, &Mtp::div_and_conquer_sq2<int>,
           &Mtp::strassen<int>, &Mtp::div_and_conquer<int>, &Mtp::winograd<int>,
           &Mtp::strassen_winograd_hybrid<int>,
           &Mtp::div_and_conquer_optimized_winograd_hybrid<int>});

int main(int argc, char const *argv[]) {
    std::cout << "Starting benchmark\n";
    std::vector<long long> total(algos.size(), 0);
    for (int j = 0; j < 10; j++) {
        for (int i = 1; i <= 128; i *= 2) {
            Matrix2<int> A(MatrixGenerator<int>::random_fill(i, i));
            Matrix2<int> B(MatrixGenerator<int>::random_fill(i, i));

            Matrix2<int> Reference = Mtp::naive(A, B);

            for (int k = 0; k < algos.size(); k++) {
                auto start = std::chrono::high_resolution_clock::now();
                auto &&C = algos[k](A, B);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration =
                    std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                          start)
                        .count();
                if (!(C == Reference)) {
                    std::cout << "Algorithm failed: " << alg_names[k] << "\n"
                              << "Size: " << i << "x" << i << "\n"
                              << "A:\n"
                              << A << "\n"
                              << "B:\n"
                              << B << "\n"
                              << "Expected:\n"
                              << Reference << "\n"
                              << "Got:\n"
                              << C << "\n";
                    return 1;
                }
                total[k] += duration;
            }
        }

        for (int k = 0; k < algos.size(); k++) {
            std::cout << "Algorithm: " << alg_names[k] << "\n"
                      << "Average time: " << total[k] / 10 << "us\n"
                      << "speedup: " << ((double)total[0] / total[k]) << "\n";
        }
        return 0;
    }
}