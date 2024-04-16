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

std::string help_msg =
    "mtp-bechmark [-h|--help] [-a|--alg algName] [csvOut]\n"
    "-h | --help\n"
    "Prints this help message and exits.\n"
    "-a | --alg\n"
    "Only runs test with the named algorithm. Available algorithm: "
    "<ALGO_NAMES>\n"  // <ALGO_NAMES> will be replaced with the actual names
    "csvOut\n"
    "Default: alg-runtimes<current datetime>.csv\n"
    "The output path of the csvFile\n";

constexpr long init_batch_size = 0x400000;  // = 4^11

std::string alg_names[] = {"naive",
                           "div_and_conquer_sq2",
                           "strassen",
                           "winograd",
                           "div_and_conquer_optimized",
                           "strassen_winograd_hybrid",
                           "div_and_conquer_optimized_winograd_hybrid"};
const std::vector<
    std::function<void(const Matrix2<double> &, const Matrix2<double> &)>>
    algos({&Mtp::naive<double>, &Mtp::div_and_conquer_sq2<double>,
           &Mtp::strassen<double>, &Mtp::winograd<double>,
           &Mtp::div_and_conquer<double>,
           &Mtp::strassen_winograd_hybrid<double>,
           &Mtp::div_and_conquer_optimized_winograd_hybrid<double>});

const int alg_cnt = algos.size();

int main(int argc, char const *argv[]) {
    std::string algo_name_list("");
    for (int i = 0; i < alg_cnt; i++) {
        algo_name_list += alg_names[i];
        if (i != alg_cnt - 1) {
            algo_name_list += ", ";
        }
    }
    help_msg.replace(help_msg.find("<ALGO_NAMES>"), 12, algo_name_list);
    std::string csvOut("");
    std::vector<bool> used_algs(alg_cnt, true);
    for (int i = 1; i < argc; i++) {
        std::string s(argv[i]);
        if (s == "--help" || s == "-h") {
            std::cout << help_msg << std::endl;
            return 0;
        } else if (s == "-a" || s == "--alg") {
            used_algs.assign(alg_cnt, false);
            std::string alg_name(argv[i + 1]);
            bool flag = false;
            for (int j = 0; j < alg_cnt; j++) {
                if (alg_name == alg_names[j]) {
                    used_algs[j] = true;
                    flag = true;
                    break;
                }
            }
            if (!flag) {
                std::cout << "Unsupported algorithm: " << alg_name << std::endl;
                return -1;
            }
        } else if (s.find('-') != -1) {
            std::cout << "Unrecognized argument: " << help_msg << std::endl;
            return -1;
        } else {
            char dt_str[13];
            auto now_epoch = time(nullptr);
            strftime(dt_str, 13, "%Y%m%d%H%M", localtime(&now_epoch));
            (((csvOut += "alg-runtimes") += argv[i]) += dt_str) += ".csv";
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
    for (int i = 0; i < alg_cnt; i++) {
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
    for (auto &&f : algos) {
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
                                         0LL) /
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
