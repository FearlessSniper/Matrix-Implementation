import csv
import matplotlib.pyplot as plt
import sys

USAGE = """
Usage: python graph.py <input_file> <output_file?>

Eg:
python graph.py data.csv graph.png
python graph.py data.csv
"""

# csv format Alg_name,Entry_type,RowA,ColA,RowB,ColB, NumberofTests, test1, test2...


def read_csv(filename: str) -> list[list[str]]:
    with open(filename, "r") as csvfile:
        csvfile.readline()  # skip header
        reader = csv.reader(csvfile)
        data = list(reader)
    return data


def format_data(data: list[list[str]]) -> dict[str, tuple[list[str], list[list[int]]]]:
    d: dict[str, tuple[list[str], list[list[int]]]] = {}
    for i, row in enumerate(data):
        name = row[0]
        matsize = row[2] + "x" + row[4]
        tests: list[int] = []
        for j, v in enumerate(row[7:]):
            try:
                if v == "":
                    continue
                tests.append(int(v))
            except ValueError:
                exit(f"Error in row={i+1} col={j+7} value='{v}'")

        if name not in d:
            d[name] = ([], [])
        d[name][0].append(matsize)
        d[name][1].append(tests)
    return d


def average_data(
    data: dict[str, tuple[list[str], list[list[int]]]]
) -> dict[str, tuple[list[str], list[float]]]:

    d: dict[str, tuple[list[str], list[float]]] = {}
    for name, (matsizes, tests) in data.items():
        d[name] = (matsizes, [])
        for test in tests:
            d[name][1].append((sum(test) / len(test)) / 1000000)
    return d


def plot_data(data: dict[str, tuple[list[str], list[int | float]]], outputfile: str):
    plt.figure(figsize=(20, 12))  # type: ignore
    for name, (matsizes, times) in data.items():
        plt.plot(matsizes, times, label=name)  # type: ignore
        print(name, matsizes, times)
    plt.legend()  # type: ignore
    plt.xlabel("Matrix Size")  # type: ignore
    plt.ylabel("Average Time (ms)")  # type: ignore
    plt.title("Matrix Multiplication Performance")  # type: ignore
    plt.savefig(outputfile)  # type: ignore
    plt.show()  # type: ignore


def main():
    if len(sys.argv) < 2:
        print(USAGE)
        sys.exit(1)
    inputfile = sys.argv[1]
    if len(sys.argv) > 2:
        outputfile = sys.argv[2]
    else:
        outputfile = inputfile.split(".")[0] + ".png"

    data = format_data(read_csv(inputfile))

    avg_data = average_data(data)

    plot_data(avg_data, outputfile)


if __name__ == "__main__":
    main()
