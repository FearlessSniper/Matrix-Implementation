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
#include <numeric>
#include <string>

#include "Algorithms.hpp"
#include "Generator.hpp"


using namespace MatMulImpl;
using Mtp = MatMulImpl::Multiplication;
using tick_type = std::chrono::high_resolution_clock::duration::rep;

const char *help_msg =
    "mtp-bechmark [-h|--help] [-a|--alg algName] [csvOut]\n"
    "-h | --help\n"
    "Prints this help message and exits.\n"
    "-a | --alg\n"
    "Only runs test with the named algorithm. Available algorithm: naive, "
    "div_and_conquer, strassen."
    "csvOut\n"
    "Default: alg-runtimes<current datetime>.csv\n"
    "The output path of the csvFile\n";

constexpr long init_batch_size = 0x400000;  // = 4^11

const char *alg_names[] = {"naive", "div_and_conquer", "strassen"};

int main(int argc, char const *argv[]) {
    std::string csvOut("");
    std::vector<bool> used_algs = {true, true, true};  // refactor!!
    for (int i = 1; i < argc; i++) {
        std::string s(argv[i]);
        if (s == "--help" || s == "-h") {
            std::cout << help_msg << std::endl;
            return 0;
        } else if (s == "-a" || s == "--alg") {
            used_algs = {false, false, false};
            std::string alg_name(argv[i + 1]);
            if (alg_name == "naive")
                used_algs[0] = true;
            else if (alg_name == "div_and_conquer")
                used_algs[1] = true;
            else if (alg_name == "strassen")
                used_algs[2] = true;  // refactor later!!
            else {
                std::cout << "Unsupported algorithm: " << alg_name << std::endl;
                return -1;
            }
        } else if (s.find('-') != -1) {
            std::cout << "Unrecognized argument: " << help_msg << std::endl;
            return -1;
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
    std::cout << "Running algorithms: ";
    for (int i = 0; i < 3; i++) {
        if (used_algs[i]) {
            std::cout << alg_names[i] << ' ';
        }
    }
    std::cout << '\n';
    std::cout << "Timing using std::chrono::high_resolution::clock\nResolution:"
              << std::chrono::high_resolution_clock::period::den
              << " ticks in 1 second" << std::endl;
    int i_alg = 0;
    int i_batch = 1;
    for (auto &&f : std::initializer_list<std::function<Matrix2<double>(
             const Matrix2<double> &, const Matrix2<double> &)> >{
             &Mtp::naive<double>, &Mtp::div_and_conquer_sq2<double>,
             &Mtp::strassen<double>}) {
        if (!used_algs[i_alg]) {
            i_alg++;
            continue;
        }
        int matrix_size = 2;
        for (long batch_size = init_batch_size; batch_size > 0;
             batch_size /= 4, i_batch++, matrix_size *= 2) {
            std::cout << "Batch #" << i_batch << ": Multiplying " << matrix_size
                      << "x" << matrix_size << " matrices with "
                      << alg_names[i_alg] << ", batch size " << batch_size
                      << std::endl;
            std::vector<tick_type> unit_times;
            unit_times.reserve(batch_size);
            for (long i = 0; i < batch_size; i++) {
                // prepare matrices
                auto m1 = MatrixGenerator<double>::random_fill(matrix_size,
                                                               matrix_size),
                     m2 = MatrixGenerator<double>::random_fill(matrix_size,
                                                               matrix_size);
                auto t_start = std::chrono::high_resolution_clock::now();
                f(m1, m2);
                auto t_end = std::chrono::high_resolution_clock::now();
                auto t_delta = t_end - t_start;
                unit_times.push_back(t_delta.count());
            }
            std::cout << "Batch #" << i_batch << " done, average ticks "
                      << std::accumulate(unit_times.begin(), unit_times.end(),
                                         0) /
                             unit_times.size()
                      << '\n';
            out_file << alg_names[i_alg] << ',' << "int" << ',' << matrix_size
                     << ',' << matrix_size << ',' << matrix_size << ','
                     << matrix_size << ',' << batch_size << ',';
            for (auto &&t : unit_times) {
                out_file << t << ',';
            }
            out_file << std::endl;
        }
        i_alg++;
    }
    out_file.close();
    return 0;
}
