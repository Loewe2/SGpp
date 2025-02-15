// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#include <sgpp/solver/operation/hash/OperationParabolicPDESolverSystem.hpp>
#include <sgpp/base/exception/algorithm_exception.hpp>
#include <sgpp/base/operation/BaseOpFactory.hpp>

#include <sgpp/globaldef.hpp>

#include <string>

namespace sgpp {
namespace solver {

OperationParabolicPDESolverSystem::OperationParabolicPDESolverSystem() {
  this->numSumGridpointsInner = 0;
  this->numSumGridpointsComplete = 0;
  this->bnewODESolver = false;
}

OperationParabolicPDESolverSystem::~OperationParabolicPDESolverSystem() {}

sgpp::base::DataVector* OperationParabolicPDESolverSystem::getGridCoefficients() {
  return this->alpha_complete;
}

sgpp::base::Grid* OperationParabolicPDESolverSystem::getGrid() { return this->BoundGrid; }

void OperationParabolicPDESolverSystem::setODESolver(std::string ode) {
  this->tOperationMode = ode;
  this->bnewODESolver = true;
}

std::string OperationParabolicPDESolverSystem::getODESolver() { return this->tOperationMode; }

void OperationParabolicPDESolverSystem::setTimestepSize(double newTimestepSize) {
  this->TimestepSize_old = this->TimestepSize;
  this->TimestepSize = newTimestepSize;
}

void OperationParabolicPDESolverSystem::abortTimestep() {
  delete this->secondGridStorage;
  this->secondGridStorage = new sgpp::base::GridStorage(this->BoundGrid->getStorage());

  if ((this->alpha_complete)->getSize() != (this->alpha_complete_tmp)->getSize()) {
    (this->alpha_complete)->resize((this->alpha_complete_tmp)->getSize());
  }

  *(this->alpha_complete) = *(this->alpha_complete_tmp);
}

void OperationParabolicPDESolverSystem::saveAlpha() {
  delete this->oldGridStorage;
  this->oldGridStorage = new sgpp::base::GridStorage(this->BoundGrid->getStorage());

  if ((this->alpha_complete_old)->getSize() != (this->alpha_complete_tmp)->getSize())
    (this->alpha_complete_old)->resize((this->alpha_complete_tmp)->getSize());

  *(this->alpha_complete_old) = *(this->alpha_complete_tmp);

  if ((this->alpha_complete_tmp)->getSize() != (this->alpha_complete)->getSize())
    (this->alpha_complete_tmp)->resize((this->alpha_complete)->getSize());

  *(this->alpha_complete_tmp) = *(this->alpha_complete);
}

size_t OperationParabolicPDESolverSystem::getSumGridPointsComplete() {
  return this->numSumGridpointsComplete;
}

size_t OperationParabolicPDESolverSystem::getSumGridPointsInner() {
  return this->numSumGridpointsInner;
}

void OperationParabolicPDESolverSystem::getGridCoefficientsForSC(sgpp::base::DataVector& Values) {
  Values = *(this->alpha_complete);
  sgpp::op_factory::createOperationHierarchisation(*BoundGrid)->doDehierarchisation(Values);
}

sgpp::base::GridStorage* OperationParabolicPDESolverSystem::getGridStorage() {
  return &this->BoundGrid->getStorage();
}

sgpp::base::GridStorage* OperationParabolicPDESolverSystem::getOldGridStorage() {
  return oldGridStorage;
}

sgpp::base::GridStorage* OperationParabolicPDESolverSystem::getSecondGridStorage() {
  return secondGridStorage;
}

}  // namespace solver
}  // namespace sgpp
