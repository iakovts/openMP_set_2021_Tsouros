import glob

import numpy as np

files = glob.glob("res*_threads.txt")

data = [np.fromfile(i, sep=" ") for i in files]

comp = (np.diff(np.vstack(data).reshape(len(data), -1), axis=0)==0).all()

if comp:
    print("Output from all threads {1..4} is the same")
else:
    print("Outputs are different for different number of threads")
