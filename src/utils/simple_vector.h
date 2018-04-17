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
#include <ostream>

#include "../model.h"

namespace starspace {

class SimpleMatrix;

class SimpleVector {

  public:
    int64_t m_;
    Real* data_;

    explicit SimpleVector(int64_t);
    SimpleVector& operator=(const SimpleVector& source);

    SimpleVector(const SimpleVector&) = delete;

    ~SimpleVector();

    Real& operator[](int64_t);
    const Real& operator[](int64_t) const;

    int64_t size() const;
    void zero();
    void mul(Real);
    Real dot(const SimpleVector& target);
    Real norm() const;
    void addVector(const SimpleVector& source);
    void addVector(const SimpleVector&, Real);
    void addRow(const SimpleMatrix&, int64_t);
    void addRow(const SimpleMatrix&, int64_t, Real);
    void mul(const SimpleMatrix&, const SimpleVector&);
    int64_t argmax();

    void concat(const SimpleVector&, const SimpleMatrix&, int64_t);
    void split(const SimpleVector&, const SimpleVector&);
};

std::ostream& operator<<(std::ostream&, const SimpleVector&);

}
