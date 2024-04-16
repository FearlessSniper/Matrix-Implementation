import csv
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import sys
import uuid

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


def format_data(data: list[list[str]]) -> dict[str, tuple[list[int], list[list[int]]]]:
    d: dict[str, tuple[list[int], list[list[int]]]] = {}
    for i, row in enumerate(data):
        name = row[0]
        matsize = int(row[2])
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
    data: dict[str, tuple[list[int], list[list[int]]]]
) -> dict[str, tuple[list[int], list[float]]]:

    d: dict[str, tuple[list[int], list[float]]] = {}
    for name, (matsizes, tests) in data.items():
        d[name] = (matsizes, [])
        for test in tests:
            d[name][1].append((sum(test) / len(test)) / 1000000)
    return d


def plot_data(data: dict[str, tuple[list[int], list[int | float]]], outputfile: str):
    # plt.figure(figsize=(20, 12))  # type: ignore
    fig, ax = plt.subplots(figsize=(20, 12))  # type: ignore
    fig.tight_layout(pad=3)  # type: ignore
    fig.subplots_adjust(right=0.93)  # type: ignore
    ax.margins(x=0.009)  # type: ignore
    start = 7
    for name, (matsizes, times) in data.items():
        offset = [12, 0]
        if name == "div_and_conquer_optimized":
            offset[1] = 5 
        elif name == "naive":
            offset[1] = -5
        elif name == "div_and_conquer_sq2":
            name = "div_and_conquer_naive"
        line = ax.plot(matsizes[start:], times[start:], "o-", label=name)  # type: ignore
        ax.annotate( # type: ignore
            f"{times[-1]:.2f}ms",
            (matsizes[-1], times[-1]), # type: ignore
            textcoords="offset points",
            xytext=offset, # type: ignore
            ha="left",
            color=line[0].get_color(),  # type: ignore
            fontsize=12,
        )
        ax.set_xticks(matsizes[start:])  # type: ignore
        print(name, matsizes, times)
    ax.legend(fontsize=24)  # type: ignore
    ax.yaxis.offsetText.set_fontsize(10)
    ax.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, pos: f"{x:.0f}x{x:.0f}"))  # type: ignore
    ax.tick_params(axis='both', which='major', labelsize=10)
    ax.set_xlabel("Matrix Size", fontsize=16)  # type: ignore
    ax.set_ylabel("Average Time (ms)", fontsize=16)  # type: ignore
    ax.set_title("Matrix Multiplication Performance", fontsize=32)  # type: ignore
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
        outputfile = inputfile.split(".")[0] + "_" + str(uuid.uuid4()) + ".png"

    data = format_data(read_csv(inputfile))

    avg_data = average_data(data)

    plot_data(avg_data, outputfile)


if __name__ == "__main__":
    main()
