import unittest

from blaze.blaze_context import BlazeContext
from functools import reduce


class TestJoin(unittest.TestCase):
    def test_overlap(self):
        bc = BlazeContext()
        input1 = [(r, r * 10) for r in range(10)]
        input2 = [(r, r * 10) for r in range(5, 15)]
        data1 = bc.collection(input1)
        data2 = bc.collection(input2)

        joined = data1.join(data2)
        res = joined.collect()
        self.assertEqual(res, [(r, (r * 10, r * 10)) for r in range(5, 10)])


class TestFilter(unittest.TestCase):
    def test_filter_collect(self):
        bc = BlazeContext()
        d = bc.collection(range(0, 10)).filter(lambda w: w % 2 == 0)
        res = d.collect()
        self.assertListEqual(list(res), list(filter(lambda w: w % 2 == 0, range(0, 10))))

    def test_filter_count(self):
        bc = BlazeContext()
        d = bc.collection(range(0, 10)).filter(lambda w: w % 2 == 0)
        res = d.count()
        self.assertEqual(res, len(list(filter(lambda w: w % 2 == 0, range(0, 10)))))


class TestMap(unittest.TestCase):
    def test_map_collect(self):
        bc = BlazeContext
        d = bc.collection(range(0, 10)).map(lambda t: (t, t * 10))
        res = d.collect()
        self.assertListEqual([(t[0], t[1]) for t in res], [(t, t * 10) for t in range(0, 10)])

    def test_map_count(self):
        bc = BlazeContext
        d = bc.collection(range(0, 10)).map(lambda t: (t, t * 10))
        res = d.count()
        self.assertEqual(res, 10)


class TestFlatMap(unittest.TestCase):
    def test_flat_map(self):
        bc = BlazeContext()
        d = bc.collection(range(0, 10)).flat_map(lambda w: [0])
        res = d.collect()
        self.assertEqual(res, [0 for i in range(0, 10)])


class TestReduce(unittest.TestCase):
    def test_reduce_sum(self):
        bc = BlazeContext()
        input = range(0, 10)
        d = bc.collection(input).reduce(lambda t1, t2: t1 + t2)
        self.assertEqual(d, sum(input))

    def test_reduce_tuple(self):
        bc = BlazeContext()
        input = [(r, r * 10) for r in range(0, 10)]
        res = bc.collection(input).reduce(lambda t1, t2: (t1[0] + t2[0], t1[1] + t2[1]))
        self.assertTupleEqual(res, (sum(map(lambda t: t[0], input)), sum(map(lambda t: t[1], input))))


class TestIntegration(unittest.TestCase):
    def test_map_filter_map(self):
        bc = BlazeContext()
        input_ = range(0, 10)
        mapF1 = lambda w: (w, w * 3)
        filtF1 = lambda w: w[0] % 2 == 0
        mapF2 = lambda w: w[0]
        # d = bc.collection(range(0, 10)).map(mapF1).map(mapF2)
        d = bc.collection(input_).map(mapF1).filter(filtF1).map(mapF2)
        res = d.collect()
        self.assertListEqual(list(res), list(map(mapF2, filter(filtF1, map(mapF1, input_)))))

    def test_map_filter_flat_map_filter_flat_map(self):
        bc = BlazeContext()
        d = bc.collection(range(0, 10)).map(lambda w: w * 3).filter(lambda w: w % 2 == 0).flat_map(
            lambda w: [0]).filter(lambda w: w % 3).flat_map(lambda w: [0])
        d.collect()


if __name__ == '__main__':
    unittest.main()
