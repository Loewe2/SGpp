// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#ifndef SGPP_OPTIMIZATION_FUNCTION_TEST_HARTMAN6_HPP
#define SGPP_OPTIMIZATION_FUNCTION_TEST_HARTMAN6_HPP

#include <sgpp/globaldef.hpp>

#include <sgpp/optimization/function/test/TestFunction.hpp>

#include <cmath>

namespace SGPP {
  namespace optimization {
    namespace test_functions {

      /**
       * Hartman6 test function.
       *
       * Definition:
       * \f$f(\vec{x}) := -\sum_{i=1}^4 a_i
       * \exp\!\left(-\sum_{t=1}^6 b_{i,t} (x_t - c_{i,t})^2\right)\f$,
       * \f$\vec{a} = \begin{pmatrix}1\\1.2\\3\\3.2\end{pmatrix}\f$,
       * \f$B :=
       *      \begin{pmatrix}
       *          10 & 3 & 17 & 3.5 & 1.7 & 8\\
       *          0.05 & 10 & 17 & 0.1 & 8 & 14\\
       *          3 & 3.5 & 1.7 & 10 & 17 & 8\\
       *          17 & 8 & 0.05 & 10 & 0.1 & 14
       *      \end{pmatrix}\f$,
       * \f$C :=
       *      \begin{pmatrix}
       *          0.1312 & 0.1696 & 0.5569 & 0.0124 & 0.8283 & 0.5886\\
       *          0.2329 & 0.4135 & 0.8307 & 0.3736 & 0.1004 & 0.9991\\
       *          0.2348 & 0.1451 & 0.3522 & 0.2883 & 0.3047 & 0.6650\\
       *          0.4047 & 0.8828 & 0.8732 & 0.5743 & 0.1091 & 0.0381
       *      \end{pmatrix}\f$,
       * \f$\vec{x} \in [0, 1]^6\f$,
       * \f$\vec{x}_{\text{opt}} = (0.20169, 0.150011, 0.476874,
       * 0.275332, 0.311652, 0.6573)^{\mathrm{T}}\f$,
       * \f$f_{\text{opt}} = -3.322368\f$
       */
      class Hartman6 : public TestFunction {
        public:
          /**
           * Constructor.
           */
          Hartman6() : TestFunction(6) {
          }

          /**
           * Evaluates the test function.
           *
           * @param x     point \f$\vec{x} \in [0, 1]^6\f$
           * @return      \f$f(\vec{x})\f$
           */
          float_t evalUndisplaced(const base::DataVector& x) {
            return -1.0 * std::exp(-10.0 * (x.get(0) - 0.1312) * (x.get(0) - 0.1312) -
                                   3.0 * (x.get(1) - 0.1696) * (x.get(1) - 0.1696) -
                                   17.0 * (x.get(2) - 0.5569) * (x.get(2) - 0.5569) -
                                   3.5 * (x.get(3) - 0.0124) * (x.get(3) - 0.0124) -
                                   1.7 * (x.get(4) - 0.8283) * (x.get(4) - 0.8283) -
                                   8.0 * (x.get(5) - 0.5886) * (x.get(5) - 0.5886)) -
                   1.2 * std::exp(-0.05 * (x.get(0) - 0.2329) * (x.get(0) - 0.2329) -
                                  10.0 * (x.get(1) - 0.4135) * (x.get(1) - 0.4135) -
                                  17.0 * (x.get(2) - 0.8307) * (x.get(2) - 0.8307) -
                                  0.1 * (x.get(3) - 0.3736) * (x.get(3) - 0.3736) -
                                  8.0 * (x.get(4) - 0.1004) * (x.get(4) - 0.1004) -
                                  14.0 * (x.get(5) - 0.9991) * (x.get(5) - 0.9991)) -
                   3.0 * std::exp(-3.0 * (x.get(0) - 0.2348) * (x.get(0) - 0.2348) -
                                  3.5 * (x.get(1) - 0.1451) * (x.get(1) - 0.1451) -
                                  1.7 * (x.get(2) - 0.3522) * (x.get(2) - 0.3522) -
                                  10.0 * (x.get(3) - 0.2883) * (x.get(3) - 0.2883) -
                                  17.0 * (x.get(4) - 0.3047) * (x.get(4) - 0.3047) -
                                  8.0 * (x.get(5) - 0.6650) * (x.get(5) - 0.6650)) -
                   3.2 * std::exp(-17.0 * (x.get(0) - 0.4047) * (x.get(0) - 0.4047) -
                                  8.0 * (x.get(1) - 0.8828) * (x.get(1) - 0.8828) -
                                  0.05 * (x.get(2) - 0.8732) * (x.get(2) - 0.8732) -
                                  10.0 * (x.get(3) - 0.5743) * (x.get(3) - 0.5743) -
                                  0.1 * (x.get(4) - 0.1091) * (x.get(4) - 0.1091) -
                                  14.0 * (x.get(5) - 0.0381) * (x.get(5) - 0.0381));
          }

          /**
           * Returns minimal point and function value of the test function.
           *
           * @param[out] x minimal point
           *               \f$\vec{x}_{\text{opt}} \in [0, 1]^6\f$
           * @return       minimal function value
           *               \f$f_{\text{opt}} = f(\vec{x}_{\text{opt}})\f$
           */
          float_t getOptimalPointUndisplaced(base::DataVector& x) {
            x.resize(6);
            x[0] = 0.20169;
            x[1] = 0.150011;
            x[2] = 0.476874;
            x[3] = 0.275332;
            x[4] = 0.311652;
            x[5] = 0.6573;
            return evalUndisplaced(x);
          }

          /**
           * @param[out] clone pointer to cloned object
           */
          virtual void clone(std::unique_ptr<ObjectiveFunction>& clone) const {
            clone = std::unique_ptr<ObjectiveFunction>(new Hartman6(*this));
          }
      };

    }
  }
}

#endif /* SGPP_OPTIMIZATION_FUNCTION_TEST_HARTMAN6_HPP */