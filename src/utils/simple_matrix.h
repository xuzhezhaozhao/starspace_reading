/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#pragma once

#include <cstdint>
#include <istream>
#include <ostream>
#include <vector>

#include "../model.h"

namespace starspace {

class SimpleVector;

class SimpleMatrix {
  public:
    Real* data_;
    int64_t m_;
    int64_t n_;

    SimpleMatrix();
    SimpleMatrix(int64_t, int64_t);
    SimpleMatrix(const SimpleMatrix&);
    SimpleMatrix& operator=(const SimpleMatrix&);
    ~SimpleMatrix();

    inline const Real& at(int64_t i, int64_t j) const {return data_[i * n_ + j];};
    inline Real& at(int64_t i, int64_t j) {return data_[i * n_ + j];};


    void zero();
    void uniform(Real);
    Real dotRow(const SimpleVector&, int64_t) const;
    void addRow(const SimpleVector&, int64_t, Real);

    void multiplyRow(const SimpleVector& nums, int64_t ib = 0, int64_t ie = -1);
    void divideRow(const SimpleVector& denoms, int64_t ib = 0, int64_t ie = -1);

    Real l2NormRow(int64_t i) const;
    void l2NormRow(SimpleVector& norms) const;

    void save(std::ostream&);
    void load(std::istream&);

    void shrinkSubset(const std::vector<int32_t> &subset);
    void convertColMajor();
};

}
