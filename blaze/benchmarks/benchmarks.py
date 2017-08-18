import os
import math
from functools import reduce
import numpy as np
import sys

sys.path.insert(0, "/home/sabir/projects/blaze")

from blaze.blaze_context import BlazeContext
from blaze.benchmarks.timer import timer

MAX = 1 << 18


def bench_sum():
    bc = BlazeContext()
    input_ = np.random.rand(MAX)
    dag = bc.numpy_array(input_)

    def run():
        return dag.reduce(lambda t1, t2: t1 + t2)
    return run


def bench_map_filter():
    bc = BlazeContext()
    input_ = np.random.rand(MAX)
    dag = bc.numpy_array(input_).map(lambda t: (t, t * 3 + 7)).filter(lambda t: t[0] * 100 > 50.0)

    def run():
        return dag.count()

    return run


def bench_join():
    bc = BlazeContext()

    input1 = np.random.randint(0, 1000, size=(MAX >> 1, 2))
    input2 = np.random.randint(0, 1000, size=(MAX, 2))

    inRDD1 = bc.numpy_array(input1)
    inRDD2 = bc.numpy_array(input2)
    dag = inRDD1.join(inRDD2)

    def run():
        return dag.collect()

    return run


t_sum = timer(bench_join(), 1)
print("time join " + str(t_sum))

# t_sum = timer(bench_sum(), 1)
# print("time sum " + str(t_sum))


# map_filter
# t_sum = timer(bench_map_filter())
# print("time map_filter " + str(t_sum))

# join
# t_sum = timer(bench_join())
# print("time join " + str(t_sum))
