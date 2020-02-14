#include <torch/csrc/jit/fuser/common/fusion.h>
#include <torch/csrc/jit/fuser/common/ir.h>
#include <torch/csrc/jit/fuser/common/tensor.h>
#include <torch/csrc/jit/fuser/common/iriostream.h>

#include <iostream>

namespace torch {
namespace jit {
namespace fuser {

namespace {

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& data) {
  os << "(";
  for (auto i = data.begin(); i != data.end(); i++) {
    os << (*i);
    os << " ";
  }
  return os << ")";
}

}

std::ostream& operator<<(std::ostream& os, const Fusion* const fusion) {
  std::cout<<"Fusion has "<< fusion->exprs().size() << "exprs" << std::endl;
  for (const Expr* expr : fusion->exprs()){
    os << expr << std::endl;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const Statement* const stmt) {
  if (stmt->isVal())
    return os << static_cast<const Val*>(stmt);
  else if (stmt->isExpr())
    return os << static_cast<const Expr*>(stmt);
  throw std::runtime_error("Unkown statment type found in os << Statement.");
}

std::ostream& operator<<(std::ostream& os, const Val* const val) {
  switch (*(val->getValType())) {
    case ValType::TensorDomain:
      return os << static_cast<const TensorDomain* const>(val);
    case ValType::TensorView:
      return os << static_cast<const TensorView* const>(val);
    case ValType::IterDomain:
      return os << static_cast<const IterDomain* const>(val);      
    case ValType::Tensor:
      return os << static_cast<const Tensor* const>(val);

    case ValType::Scalar:
      switch (*(val->getDataType())){
        case DataType::Float:
          return os << static_cast<const Float* const>(val);
        case DataType::Int:
          return os << static_cast<const Int* const>(val);
        default:
          break;
      }

    default:
      break;
  }
  throw std::runtime_error("Unknown ValType in os << Val.");
}

std::ostream& operator<<(std::ostream& os, const Expr* const expr) {
  switch (*(expr->getExprType())) {
    case ExprType::UnaryOp:
      return os << static_cast<const UnaryOp*>(expr);
    case ExprType::BinaryOp:
      return os << static_cast<const BinaryOp*>(expr);
    case ExprType::Split:
      return os << static_cast<const Split*>(expr);
    case ExprType::Merge:
      return os << static_cast<const Merge*>(expr);
    case ExprType::Reorder:
      return os << static_cast<const Reorder*>(expr);
  }
  throw std::runtime_error("Unknown ExprType in os << Expr.");
}

TORCH_API std::ostream& operator<<(std::ostream& os, const TensorDomain* const td){
  os << "[ ";
  for(std::vector<const IterDomain*>::size_type i = 0; i<td->size(); i++){
    os<<td->axis(i);
    if(i!=td->size()-1)
      os<<", ";
  }
  return os<<" ]";
}

TORCH_API std::ostream& operator<<(std::ostream& os, const TensorView* const tv){
  assert(tv->domain() != nullptr);
  return os << tv->tensor() << " -> "<<tv->domain();
}

TORCH_API std::ostream& operator<<(std::ostream& os, const IterDomain* const id){
  if(id->isReduction())
    os << "r";
  else
    os << "i";
  switch(id->parallel_method()){
    case(ParallelType::Vectorize):
      os <<"V";
      break;
    case(ParallelType::Unroll):
      os << "U";
      break;
    case(ParallelType::Serial):
      os << "S";
      break;
    default:
      os << id->parallel_method();
  }
  return os << "{" << id->size() << "}";
}

std::ostream& operator<<(std::ostream& os, const Tensor* const t) {
  os << "%T" << t->name(); 
  if(t->getDataType().has_value())
    os << " scalar_type: " << *(t->getDataType());
  if(t->domain() != nullptr)
    os << " " << t->domain();
  if(t->hasContiguityInfo())
    os << " " << &t->getContiguityInfo().value();
  return os;
}

std::ostream& operator<<(
    std::ostream& os,
    const TensorContiguity* const t) {
  return os << "format_tag: " << t->getContiguityTag();
}

std::ostream& operator<<(std::ostream& os, const Float* const f) {
  os << "%f";
  if (f->isSymbolic()) {
    return os << f->name();
  } else {
    return os << f->name() << "{" << *(f->value()) << "}";
  }
}

std::ostream& operator<<(std::ostream& os, const Int* const i) {
  os << "%i";
  if (i->isSymbolic()) {
    return os << i->name();
  } else {
    return os << i->name() << "{" << *(i->value()) << "}";
  }
}

std::ostream& operator<<(std::ostream& os, const UnaryOp* const uop) {
  return os << uop->out() << " = " << uop->type() << "(" << uop->in() << ")";
}

std::ostream& operator<<(std::ostream& os, const BinaryOp* const bop) {
  return os << bop->out() << " = " << bop->type() << "(" << bop->lhs() << ", " << bop->rhs() << ")";
}

std::ostream& operator<<(std::ostream& os, const Fusion& f) {
  return os << &f;
}

/*
 * Avoid using lowercase instances of expressions, as they will be used for
 * arith version of the expressions. For example
 * Split is an IR node
 * split is a function that generates an IR node and returns a const TensorView*.
 */

TORCH_API std::ostream& operator<<(std::ostream& os, const Split* const s){
  return os << "Split: " << s->in() << " axis " << s->axis() << " by factor " << s->factor() << " -> " << s->out();
}

TORCH_API std::ostream& operator<<(std::ostream& os, const Merge* const m){
  return os << "Merge: " << m->in() << " axis " << m->axis() << " with the following -> " << m->out();
}

TORCH_API std::ostream& operator<<(std::ostream& os, const Reorder* const ro){
  return os << "Reorder: " << ro->in() << " -> " << ro->out();
}

} // namespace fuser
} // namespace jit
} // namespace torch
