// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#ifndef FUNCTIONLOOKUPTABLE_HPP_
#define FUNCTIONLOOKUPTABLE_HPP_

#include <sgpp/base/datatypes/DataVector.hpp>
#include <sgpp/combigrid/MultiFunction.hpp>
#include <sgpp/globaldef.hpp>

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace sgpp {
namespace combigrid {

struct FunctionLookupTableImpl;

/**
 * This class wraps a MultiFunction and stores computed values using a hashtable to avoid
 * reevaluating a function at points where it already has been evaluated. This means that only the
 * exact same parameter will allow retrieving the function value.
 */
class FunctionLookupTable {
  std::shared_ptr<FunctionLookupTableImpl> impl;

 public:
  explicit FunctionLookupTable(MultiFunction const &func);

  /**
   * Evaluates the function at the point x. If the function has already been evaluated at this
   * point, the stored result will be used.
   */
  double operator()(base::DataVector const &x);

  /**
   * Does the same as operator() (can be called from Python, for example).
   */
  double eval(base::DataVector const &x);

  /**
   * Does the same as eval(), but uses a mutex whenever the hashtable is accessed. Crucially, the
   * mutex is not locked when evaluating the function, such that multiple function evaluations can
   * be done in parallel.
   */
  double evalThreadsafe(base::DataVector const &x);

  /**
   * @returns true iff the hashtable contains a function value for the parameter x.
   */
  bool containsEntry(base::DataVector const &x);

  /**
   * Adds a function value into the storage.
   * @param x Parameter of the function.
   * @param y Result of the function evaluation.
   */
  void addEntry(base::DataVector const &x, double y);

  /**
   * Stores the stored values into a string.
   */
  std::string serialize();

  /**
   * Retrieves stored values from a string generated by serialize().
   */
  void deserialize(std::string const &value);

  /**
   * @return the number of stored function values.
   */
  size_t getNumEntries() const;
};
}  // namespace combigrid
}  // namespace sgpp

#endif /* FUNCTIONLOOKUPTABLE_HPP_ */