#ifndef DAG_DAGOPERATORS_H
#define DAG_DAGOPERATORS_H

#include "DAGAssertCorrectOpenNextClose.h"
#include "DAGCartesian.h"
#include "DAGColumnScan.h"
#include "DAGConstantTuple.h"
#include "DAGEnsureSingleTuple.h"
#include "DAGExpandPattern.h"
#include "DAGFilter.h"
#include "DAGGroupBy.h"
#include "DAGJoin.h"
#include "DAGMap.h"
#include "DAGMaterializeColumnChunks.h"
#include "DAGMaterializeParquet.h"
#include "DAGMaterializeRowVector.h"
#include "DAGParallelMap.h"
#include "DAGParameterLookup.h"
#include "DAGParquetScan.h"
#include "DAGPartition.h"
#include "DAGPipeline.h"
#include "DAGProjection.h"
#include "DAGRange.h"
#include "DAGReduce.h"
#include "DAGReduceByKey.h"
#include "DAGReduceByKeyGrouped.h"
#include "DAGRowScan.h"
#include "DAGSplitColumnData.h"
#include "DAGSplitRange.h"
#include "DAGSplitRowData.h"

#endif
