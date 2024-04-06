/**
 * @file mtp-benchmark.cpp
 * @author aydenwong (aydenwongfs@gmail.com)
 * @brief Benchmarks the performance of different matrix multiplication
 * algorithms and outputs a CSV for further processing
 * @version 0.1
 * @date 06-04-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

/**
 * CSV format:
 * Row 1 (Table header)||Alg_name|Entry_type|RowA|ColA|RowB|ColB|Tests|T1|...
 * Row 2 and onwards   || Naive    |int     | 2  | 2  | 2  | 2|
 * | 100   |... |...
 * ...
 */
#include <chrono>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

#include "Algorithms.hpp"
#include "Generator.hpp"

using namespace MatMulImpl;
using Mtp = MatMulImpl::Multiplication;
using tick_type = std::chrono::high_resolution_clock::duration::rep;

const char *help_msg =
    "mtp-bechmark [-h|--help] [csvOut]\n"
    "-h | --help\n"
    "Prints this help message and exits.\n"
    "csvOut\n"
    "Default: alg-runtimes<current datetime>.csv\n"
    "The output path of the csvFile\n";

constexpr long init_batch_size = 0x200000000;  // Put a number that is 2^n here

int main(int argc, char const *argv[]) {
    std::string csvOut("");
    for (int i = 1; i < argc; i++) {
        std::string s(argv[i]);
        if (s == "--help" || s == "-h") {
            std::cout << help_msg << std::endl;
            return 0;
        } else if (s.find('-') != -1) {
            std::cout << "Unrecognized argument: " << help_msg << std::endl;
            return 0;
        } else {
            csvOut.assign(argv[i]);
        }
    }
    if (csvOut == "") {
        char dt_str[13];
        auto now_epoch = time(nullptr);
        strftime(dt_str, 13, "%Y%m%d%H%M", localtime(&now_epoch));
        ((csvOut += "alg-runtimes") += dt_str) += ".csv";
    }
    std::fstream out_file;
    out_file.open(csvOut, std::ios::out);
    out_file << "Alg_name,Entry_type,RowA,ColA,RowB,ColB,Tests\n";
    // auto f = Mtp::naive;
    // for (std::function < Matrix2<int> (const Matrix2<int>&, const
    // Matrix2<int>&) > &&f: {Mtp::naive, Mtp::div_and_conquer_sq2,
    // Mtp::strassen}; std::function < Matrix2<int>(const)
    const char *alg_names[] = {"naive", "div_and_conquer", "strassen"};
    std::cout << "Running algorithms: ";
    for (auto &&s : alg_names) {
        std::cout << s << ' ';
    }
    std::cout << '\n';
    std::cout << "Timing using std::chrono::high_resolution::clock\nResolution:"
              << std::chrono::high_resolution_clock::period::den
              << " ticks in 1 second" << std::endl;
    int i_alg = 0;
    int i_batch = 1;
    for (auto &&f : {&Mtp::naive<int>, &Mtp::div_and_conquer_sq2<int>,
                     &Mtp::strassen<int>}) {
        int matrix_size = 2;
        for (long batch_size = init_batch_size; batch_size > 0; batch_size /= 8, i_batch++, matrix_size*=2) {
            std::cout << "Batch #" << i_batch << ": Multiplying " << matrix_size << "x" << matrix_size << " matrices with " << alg_names[i_alg] << ", batch size " << batch_size << std::endl;
            std::vector<tick_type> unit_times(batch_size);
            for (long i = 0; i < batch_size; i++) {
                // prepare matrices
                auto m1 = MatrixGenerator<int>::random_fill(matrix_size,
                                                            matrix_size),
                     m2 = MatrixGenerator<int>::random_fill(matrix_size,
                                                            matrix_size);
                auto t_start = std::chrono::high_resolution_clock::now();
                f(m1, m2);
                auto t_end = std::chrono::high_resolution_clock::now();
                auto t_delta = t_end - t_start;
                unit_times.push_back(t_delta.count());
            }
            std::cout << "Batch #" << i_batch << " done, average ticks " << std::reduce(unit_times.begin(), unit_times.end()) / unit_times.size() << '\n';
            out_file << alg_names[i_alg] << ',' << "int" << ',' << matrix_size << ',' << matrix_size << ',' << matrix_size << ',' << matrix_size << ',' << batch_size << ',';
            for (auto &&t : unit_times)
            {
                out_file << t << ',';
            }
            out_file << std::endl;
        }
        i_alg++;
    }
    out_file.close();
    return 0;
}
