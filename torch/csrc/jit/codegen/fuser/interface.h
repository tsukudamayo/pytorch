#pragma once

#include <ATen/ATen.h>
#include <torch/csrc/WindowsTorchApiMacro.h>
#include <torch/csrc/jit/ir/ir.h>
#include <ATen/core/stack.h>
#include <c10/core/DeviceType.h>

#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>

namespace torch {
namespace jit {

/*
 * NEW INTERFACE
*/

#define FUSER_DEBUG 1

class FusionBackend {
public:
  virtual bool isFusible(const Node* const node) = 0;
  virtual int fuse(const Node* const node) = 0;
  virtual void compileFusion(Node* fusion) = 0;
  virtual void callFusion(
      const Node* const fusion,
      std::vector<at::Tensor>&,
      at::ArrayRef<IValue>) = 0;

  virtual ~FusionBackend() = 0;
};

TORCH_API void registerFusionBackendEx(
    at::Device::Type backend_type,
    FusionBackend* backend);

TORCH_API bool hasFusionBackendEx(at::Device::Type backend_type);

struct TORCH_API RegisterFusionBackendEx {
  RegisterFusionBackendEx(
      at::Device::Type backend_type,
      FusionBackend* backend);
};

// Returns true iff the node is fusible
TORCH_API bool isFusible(const Node* const node);

// Creates a fusion consisting of just the given node and returns its
// corresponding key
TORCH_API int fuse(const Node* const node);

// Compiles the given fusion node
TORCH_API void compileFusion(Node* fusion);

// TODO: remove key, it can be acquired from the node
TORCH_API void callFusion(const Node* const node, Stack& stack);

<<<<<<< HEAD:torch/csrc/jit/codegen/fuser/interface.h
/*
 * OLD INTERFACE BELOW
*/


constexpr int kCPUDevice = -1;

// Assigns a "key" to the given fusion_group that it can use to run its
// fusion later (via runFusion() below).
TORCH_API int64_t registerFusion(const Node* fusion_group);

// Runs the fusion corresponding to the given key on the inputs
// found on the stack. Outputs are placed on the same stack.
// In some cases a fusion cannot be run and a fallback path where
// PyTorch's interpreter runs the graph instead is attempted.
TORCH_API void runFusion(const int64_t key, Stack& stack);

// True if the respective devices can fuse, false otherwise
TORCH_API bool canFuseOnCPU();
TORCH_API bool canFuseOnGPU();

// Sets whether fusion on the CPU is allowed (disabled by default due to
// flakiness)
TORCH_API void overrideCanFuseOnCPU(bool value);

// Sets whether fusion on the GPU is allowed (enabled by default)
TORCH_API void overrideCanFuseOnGPU(bool value);

// Treats the given graph as a fusion group and launches it on the
// specified device with the given inputs.
// Returns the outputs.
TORCH_API std::vector<at::Tensor> debugLaunchGraph(
    Graph& graph,
    at::ArrayRef<at::Tensor> inputs);

// Treats the given graph as a fusion group and returns the generated code.
TORCH_API std::string debugGetFusedKernelCode(
    Graph& graph,
    at::ArrayRef<at::Tensor> inputs);

TORCH_API size_t nCompiledKernels();

=======
>>>>>>> Removes (most) old code, updates README:torch/csrc/jit/fuser/interface.h
} // namespace jit
} // namespace torch
