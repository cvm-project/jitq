import pandas as pd
import numpy as np
import os
import sys

sys.path.insert(0, os.environ['BLAZEPATH'])

from blaze.benchmarks.timer import Timer
from blaze.blaze_context import BlazeContext
from numba import njit

size = 1 << 27
data = np.random.rand(size)
print("data generated ")
t = Timer()

t.start()
r = data[data > 0].sum()
t.end()

print("numpy time: " + t.diff())


#
# @njit(fastmath=False, cache=True)
# def numba(data_):
#     d = 0
#     for el in data_:
#         if el > 0:
#             d += el
#     return d
#
#
# t.start()
# numba(data)
# t.end()
#
# print("numba time: " + t.diff())
bc = BlazeContext()
t.start()
bc.collection(data).filter(lambda x: x > 0).reduce(lambda a, b: a + b)
t.end()
#
# print("blaze time: " + t.diff())

# pandas
pd_df = pd.DataFrame(data, dtype=float)
t.start()
pd_res = pd_df[pd_df > 0].sum()
t.end()
print("pandas time: " + t.diff())