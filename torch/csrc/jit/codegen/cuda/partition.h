#pragma once

#include <torch/csrc/jit/ir.h>

/*
 * API for query node-compatibility in CudaCodeGen
 *
 * It is used in the optimization passes, where the graph is traversed and parts
 * that could be handled by CudaCodegen is partitioned and stuffed in
 * `attr::Subgraph` of `prim::CudaFusionGroup`.
 *
 * Logic right now is very simple. On top of device placement, we consider a
 * `Node` compatible when we have a parsing rule for it in our parser.
 */

namespace torch {
namespace jit {
namespace fuser {
namespace cuda {

TORCH_API bool isFusibleCudaFusionGroup(const Node* const node);

// consider if `node` could be fused into `fusion`
TORCH_API bool isFusibleCudaFusionGroup(
    const Node* const fusion,
    const Node* const node);

}}}}
