from blaze.executor import execute_plan
from blaze.stage_generator import schedule


def compute_sink_rdd(rdd):
    stage = schedule(rdd)
    return execute_plan(stage)
