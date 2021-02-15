import glob
import argparse

import numpy as np


def argument_parse():
    parser = argparse.ArgumentParser()
    parser.add_argument("--files", "-f", nargs="+", required=False)
    return parser.parse_args()


if __name__ == "__main__":
    args = argument_parse()
    if not args.files:
        files = glob.glob("res*_threads.txt")
    data = [np.fromfile(i, sep=" ") for i in files]
    comp = (np.diff(np.vstack(data).reshape(len(data), -1), axis=0)==0).all()
    if comp:
        print("Results are the same for all test cases")
    else:
        print("Results are different for different number of threads")
