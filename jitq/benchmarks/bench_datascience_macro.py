import pandas as pd
import dask.dataframe as dd
import sys
import numpy as np

from dask import threaded

sys.path.insert(0, "/home/sabir/projects/jitq")

from jitq.jitq_context import JitqContext

from jitq.benchmarks.timer import timer, Timer

from multiprocessing.pool import ThreadPool
import dask
from subprocess import call

dask.set_options(pool=ThreadPool(1))

dtype = {'grid_id': int,
         'event': int,
         'holiday': int,
         'temp': float,
         'hum': float,
         'discomf': float,
         'daylight': float,
         'moon': float,
         'prior1d': float,
         'prior3d': float,
         'prior7d': float,
         'prior14d': float,
         'offence': float,
         'probability': float,
         'pred_class': int,
         'probability_norm': float,
         'pred_class_norm': float,
         'cen_year': int,
         'dow': int}

dtype_list = [('grid_id', int),
              ('event', int),
              ('holiday', int),
              ('temp', float),
              ('hum', float),
              ('discomf', float),
              ('daylight', float),
              ('moon', float),
              ('prior1d', float),
              ('prior3d', float),
              ('prior7d', float),
              ('prior14d', float),
              ('offence', float),
              ('probability', float),
              ('pred_class', int),
              ('probability_norm', float),
              ('pred_class_norm', float),
              ('cen_year', int),
              ('dow', int)]


def clear_file_cache():
    call(["sudo -- sh -c 'sync ; echo 3 > /proc/sys/vm/drop_caches'"])


def with_pandas():
    print('loading csv files...')
    t = Timer()
    t.start()
    static_feat = pd.read_csv('crime_data/static_feat.csv', dtype=float)
    census = pd.read_csv('crime_data/semi_static_feat.csv', dtype=float)
    features_raw = pd.read_csv('crime_data/main_feat_scaled.csv', dtype=float)

    print("loading files " + t.diff())

    print('merging dataframes step 1...')
    features_raw = pd.merge(features_raw, census, on=['grid_id'], sort=False)
    # print(features_raw.shape)

    print('merging dataframes step 2...')
    features_raw = pd.merge(features_raw, static_feat, on=['grid_id'], sort=False)
    #
    print('filtering...')
    features_raw = features_raw[features_raw['land_no_use'] < 1]
    print(features_raw.shape)


def with_dask():
    static_feat = dd.read_csv('crime_data/static_feat.csv', dtype=float)
    census = dd.read_csv('crime_data/semi_static_feat.csv', dtype=float)
    features_raw = dd.read_csv('crime_data/main_feat_scaled2.csv', dtype=float)

    features_raw['temp'] = features_raw['temp'] * 1.8 + 32
    features_raw['weather1'] = 3.2 * features_raw['temp'] ** 3 + 7.5 * features_raw['hum'] ** 3 + \
                               2.3 * features_raw['discomf'] ** 3 + 5.3 * \
                                features_raw['daylight'] ** 3 + 8.6 * \
                                features_raw['moon'] ** 3

    features_raw['weather2'] = 3.2 * features_raw['temp'] ** 2 + 7.5 * features_raw['hum'] ** 2 + \
                               2.3 * features_raw['discomf'] ** 2 + 5.3 * \
                            features_raw['daylight'] ** 2 + 8.6 * \
                            features_raw['moon'] ** 2

    features_raw['weather3'] = 3.2 * features_raw['temp'] ** 4 + 7.5 * features_raw['hum'] ** 4 + \
                               2.3 * features_raw['discomf'] ** 4 + 5.3 * \
                                features_raw['daylight'] ** 4 + 8.6 * \
                                features_raw['moon'] ** 4

    features_raw = dd.merge(features_raw, static_feat, on=['grid_id'])
    features_raw = dd.merge(features_raw, census, on=['grid_id'])
    features_raw = features_raw[features_raw['land_no_use'] < 1]

    result = features_raw.compute()


def with_our():
    bc = JitqContext()
    ti = Timer()
    ti.start()
    static_feat = pd.read_csv('crime_data/static_feat.csv', dtype=float)
    static_feat = bc.collection(static_feat)
    census = pd.read_csv('crime_data/semi_static_feat.csv', dtype=float)
    census = bc.collection(census)
    features_raw = pd.read_csv('crime_data/main_feat_scaled2.csv', dtype=float)
    features_raw = bc.collection(features_raw)
    ti.end()
    print("loading files " + ti.diff())

    features_raw = features_raw.map(lambda t: t[:3] + (t[3] * 1.8 + 32,) + t[4:])
    features_raw = features_raw.map(
        lambda t: t + (3.2 * t[3] ** 3 + 7.5 * t[4] ** 3 + 2.3 * t[5] ** 3 + 5.3 * t[6] ** 3 + 8.6 * t[7] ** 3,
                       3.2 * t[3] ** 2 + 7.5 * t[4] ** 2 + 2.3 * t[5] ** 2 + 5.3 * t[6] ** 2 + 8.6 * t[7] ** 2,
                       3.2 * t[3] ** 4 + 7.5 * t[4] ** 4 + 2.3 * t[5] ** 4 + 5.3 * t[6] ** 4 + 8.6 * t[7] ** 4,))

    features_raw = static_feat.join(features_raw)
    features_raw = census.join(features_raw)

    features_raw = features_raw.filter(lambda t: t[1][37] < 1)  # 0.4 selectivity
    res = features_raw.collect()


# print("pandas time: " + str(timer(with_pandas, 1)))
print("dask time: " + str(timer(with_dask, 1)))
print("jitq time: " + str(timer(with_our, 1)))
