// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#include <cstring>
#include <iterator>

#include <sgpp/globaldef.hpp>

#include <sgpp/optimization/gridgen/IterativeGridGeneratorSOO.hpp>
#include <sgpp/optimization/gridgen/HashRefinementMultiple.hpp>
#include <sgpp/optimization/tools/Printer.hpp>
#include <sgpp/base/grid/generation/functors/SurplusRefinementFunctor.hpp>

namespace SGPP {
  namespace optimization {

    IterativeGridGeneratorSOO::IterativeGridGeneratorSOO(
      ObjectiveFunction& f, base::Grid& grid, size_t N,
      float_t adaptivity) :
      IterativeGridGenerator(f, grid, N) {
      setAdaptivity(adaptivity);
    }

    IterativeGridGeneratorSOO::AdaptivityFunction
    IterativeGridGeneratorSOO::getAdaptivity() const {
      return hMax;
    }

    void IterativeGridGeneratorSOO::setAdaptivity(float_t adaptivity) {
      hMax = [adaptivity](size_t n) {
        return std::pow(n, adaptivity);
      };
    }

    void IterativeGridGeneratorSOO::setAdaptivity(
      IterativeGridGeneratorSOO::AdaptivityFunction adaptivity) {
      hMax = adaptivity;
    }

    bool IterativeGridGeneratorSOO::generate() {
      printer.printStatusBegin("Adaptive grid generation (SOO)...");

      bool result = true;
      base::GridIndex::PointDistribution distr = base::GridIndex::Normal;
      base::GridStorage& gridStorage = *grid.getStorage();
      const size_t d = f.getDimension();

      HashRefinementMultiple refinement;

      if ((std::strcmp(grid.getType(), "bsplineClenshawCurtis") == 0) ||
          (std::strcmp(grid.getType(), "modBsplineClenshawCurtis") == 0) ||
          (std::strcmp(grid.getType(), "linearClenshawCurtis") == 0)) {
        // Clenshaw-Curtis grid
        distr = base::GridIndex::ClenshawCurtis;
      }

      // generate initial grid
      {
        base::GridIndex gp(d);

        for (size_t t = 0; t < d; t++) {
          gp.set(t, 1, 1);
        }

        gridStorage.insert(gp);
      }

      size_t currentN = 1;

      // depthMap[depth] is a list of indices j with delta(x[j]) = depth
      std::vector<std::vector<size_t>> depthMap{{0}};
      // if the i-th grid point is refinable
      std::vector<bool> refinable(N, true);

      // abbreviation (functionValues is a member variable of
      // IterativeGridGenerator)
      base::DataVector& fX = functionValues;
      fX.resize(N);

      {
        base::GridIndex& gp = *gridStorage.get(0);
        base::DataVector x(d);

        for (size_t t = 0; t < d; t++) {
          x[t] = gp.getCoord(t);
        }

        fX[0] = f.eval(x);
      }

      base::DataVector refinementAlpha(1, 0.0);

      size_t depthBoundOffset = 0;
      size_t n = 0;
      bool breakLoop = false;

      // iteration counter
      size_t k = 0;

      while (!breakLoop) {
        // status printing
        {
          char str[10];
          snprintf(str, 10, "%.1f%%",
                   static_cast<float_t>(currentN) /
                   static_cast<float_t>(N) * 100.0);
          printer.printStatusUpdate(std::string(str) +
                                    " (N = " + std::to_string(currentN) +
                                    ", k = " + std::to_string(k) + ")");
        }

        const size_t curDepthBound =
          std::min(depthMap.size() - 1,
                   static_cast<size_t>(hMax(n)) + depthBoundOffset);
        float_t nuMin = INFINITY;

        for (size_t depth = 0; depth <= curDepthBound; depth++) {
          float_t fBest = INFINITY;
          size_t iBest = 0;

          for (size_t i : depthMap[depth]) {
            if (refinable[i] && (fX[i] < fBest)) {
              fBest = fX[i];
              iBest = i;
            }
          }

          if (fBest < nuMin) {
            refinementAlpha[iBest] = 1.0;
            base::SurplusRefinementFunctor refineFunc(&refinementAlpha, 1);
            refinement.free_refine(&gridStorage, &refineFunc);

            // new grid size
            const size_t newN = gridStorage.size();

            if (newN == currentN) {
              // size unchanged ==> point not refined (should not happen)
              printer.printStatusEnd(
                "error: size unchanged in IterativeGridGeneratorSOO");
              result = false;
              breakLoop = true;
              break;
            }

            if (newN > N) {
              // too many new points ==> undo refinement and exit
              undoRefinement(currentN);
              breakLoop = true;
              break;
            }

            // resize refinement vector and set to all zeros
            // (in the following loop)
            refinementAlpha.resize(newN);
            refinementAlpha[iBest] = 0.0;

            for (size_t i = currentN; i < newN; i++) {
              base::GridIndex& gp = *gridStorage.get(i);
              // set point distribution accordingly to
              // normal/Clenshaw-Curtis grids
              gp.setPointDistribution(distr);
              refinementAlpha[i] = 0.0;
              size_t depth = 0;

              // calculate sum of levels
              for (size_t t = 0; t < d; t++) {
                depth += gp.getLevel(t);
              }

              depth -= d;

              for (size_t depth2 = depthMap.size(); depth2 <= depth; depth2++) {
                depthMap.push_back(std::vector<size_t>());
              }

              depthMap[depth].push_back(i);
            }

            // evaluation of f in the new grid points
            evalFunction(currentN);

            refinable[iBest] = false;
            n++;
            nuMin = fBest;

            currentN = newN;
          }
        }

        if (std::isinf(nuMin)) {
          depthBoundOffset++;
        }

        k++;
      }

      // delete superfluous entries in fX
      fX.resize(currentN);

      if (result) {
        printer.printStatusUpdate("100.0% (N = " + std::to_string(currentN) +
                                  ", k = " + std::to_string(k) + ")");
        printer.printStatusEnd();
        return true;
      } else {
        return false;
      }
    }

  }
}