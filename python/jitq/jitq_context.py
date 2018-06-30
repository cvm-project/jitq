import jsonmerge

from jitq.rdd import CollectionSource, GeneratorSource, \
    Range, ParameterLookup


class JitqContext:
    default_conf = {}

    def __init__(self, **kwargs):
        conf = kwargs.get('conf', {})
        self.conf = jsonmerge.merge(JitqContext.default_conf, conf)
        self.serialization_cache = {}
        self.executor_cache = {}

    def read_csv(self, path, dtype=None):
        raise NotImplementedError

    def collection(self, values, add_index=False):
        return CollectionSource(self, ParameterLookup(self, values), add_index)

    def range_(self, from_, to, step=1):
        return Range(self, ParameterLookup(self, (from_, to, step)))

    def generator(self, func):
        return GeneratorSource(self, func)
