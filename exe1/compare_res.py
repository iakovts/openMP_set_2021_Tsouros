import glob
import argparse

import numpy as np


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--files", "-f", nargs="+", required=False)
    args = parser.parse_args()
    files = args.files
    if not args.files:
        files = glob.glob("res*_threads.txt")
    data = [np.fromfile(i, sep=" ") for i in files]
    comp = (np.diff(np.vstack(data).reshape(len(data), -1), axis=0)==0).all()
    if comp:
        print("Results are the same for all test cases")
    else:
        print("Results are different for different number of threads")
