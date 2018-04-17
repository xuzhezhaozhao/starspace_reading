/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#include "simple_vector.h"

#include <assert.h>

#include <iomanip>
#include <cmath>
#include <iostream>

#include "simple_matrix.h"

namespace starspace {

SimpleVector::SimpleVector(int64_t m) {
  m_ = m;
  data_ = new Real[m];
}

SimpleVector& SimpleVector::operator=(const SimpleVector& source) {
  delete[] data_;
  m_ = source.m_;

  data_ = new Real[m_];

  for (int i = 0; i < m_; ++i) {
    data_[i] = source.data_[i];
  }

  return *this;
}

SimpleVector::~SimpleVector() {
  delete[] data_;
}

int64_t SimpleVector::size() const {
  return m_;
}

void SimpleVector::zero() {
  memset(data_, 0, sizeof(Real) * m_);
}

Real SimpleVector::norm() const {
  Real sum = 0;
  for (int64_t i = 0; i < m_; i++) {
    sum += data_[i] * data_[i];
  }
  return std::sqrt(sum);
}

void SimpleVector::mul(Real a) {
  for (int64_t i = 0; i < m_; i++) {
    data_[i] *= a;
  }
}

Real SimpleVector::dot(const SimpleVector& target) {
  assert(m_ == target.m_);
  Real sum = 0.0;
  for (int64_t i = 0; i < m_; i++) {
    sum += data_[i] * target.data_[i];
  }
  return sum;
}

void SimpleVector::addVector(const SimpleVector& source) {
  assert(m_ == source.m_);
  for (int64_t i = 0; i < m_; i++) {
    data_[i] += source.data_[i];
  }
}

void SimpleVector::addVector(const SimpleVector& source, Real s) {
  assert(m_ == source.m_);
  for (int64_t i = 0; i < m_; i++) {
    data_[i] += s * source.data_[i];
  }
}

void SimpleVector::addRow(const SimpleMatrix& A, int64_t i) {
  assert(i >= 0);
  assert(i < A.m_);
  assert(m_ == A.n_);
  for (int64_t j = 0; j < A.n_; j++) {
    data_[j] += A.at(i, j);
  }
}

void SimpleVector::addRow(const SimpleMatrix& A, int64_t i, Real a) {
  assert(i >= 0);
  assert(i < A.m_);
  assert(m_ == A.n_);
  for (int64_t j = 0; j < A.n_; j++) {
    data_[j] += a * A.at(i, j);
  }
}

void SimpleVector::mul(const SimpleMatrix& A, const SimpleVector& vec) {
  assert(A.m_ == m_);
  assert(A.n_ == vec.m_);
  for (int64_t i = 0; i < m_; i++) {
    data_[i] = A.dotRow(vec, i);
  }
}

int64_t SimpleVector::argmax() {
  Real max = data_[0];
  int64_t argmax = 0;
  for (int64_t i = 1; i < m_; i++) {
    if (data_[i] > max) {
      max = data_[i];
      argmax = i;
    }
  }
  return argmax;
}

Real& SimpleVector::operator[](int64_t i) {
  return data_[i];
}

const Real& SimpleVector::operator[](int64_t i) const {
  return data_[i];
}

void SimpleVector::concat(const SimpleVector& v, const SimpleMatrix& A, int64_t i) {
  assert(i >= 0);
  assert(i < A.m_);
  assert(m_ == v.m_ + A.n_);
  for (int64_t j = 0; j < v.m_; ++j) {
    data_[j] = v[j];
  }

  for (int64_t j = v.m_; j < m_; ++j) {
    data_[j] = A.at(i, j);
  }
}

void SimpleVector::split(const SimpleVector& left, const SimpleVector& right) {
  assert(m_ == left.m_ + right.m_);
  for (int64_t i = 0; i < left.m_; ++i) {
    left.data_[i] = data_[i];
  }

  for (int64_t i = left.m_; i < m_; ++i) {
    right.data_[i - left.m_] = data_[i];
  }
}

std::ostream& operator<<(std::ostream& os, const SimpleVector& v)
{
  os << std::setprecision(5);
  for (int64_t j = 0; j < v.m_; j++) {
    os << v.data_[j] << ' ';
  }
  return os;
}



}
