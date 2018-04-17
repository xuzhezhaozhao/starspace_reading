/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#include "simple_matrix.h"

#include <assert.h>
#include <random>

#include "simple_vector.h"

namespace starspace {

SimpleMatrix::SimpleMatrix() {
  m_ = 0;
  n_ = 0;
  data_ = nullptr;
}

SimpleMatrix::SimpleMatrix(int64_t m, int64_t n) {
  m_ = m;
  n_ = n;
  data_ = new Real[m * n];
}

SimpleMatrix::SimpleMatrix(const SimpleMatrix& other) {
  m_ = other.m_;
  n_ = other.n_;
  data_ = new Real[m_ * n_];
  for (int64_t i = 0; i < (m_ * n_); i++) {
    data_[i] = other.data_[i];
  }
}

SimpleMatrix& SimpleMatrix::operator=(const SimpleMatrix& other) {
  SimpleMatrix temp(other);
  m_ = temp.m_;
  n_ = temp.n_;
  std::swap(data_, temp.data_);
  return *this;
}

SimpleMatrix::~SimpleMatrix() {
  delete[] data_;
}

void SimpleMatrix::zero() {
  for (int64_t i = 0; i < (m_ * n_); i++) {
      data_[i] = 0.0;
  }
}

void SimpleMatrix::uniform(Real a) {
  std::minstd_rand rng(1);
  std::uniform_real_distribution<> uniform(-a, a);
  for (int64_t i = 0; i < (m_ * n_); i++) {
    data_[i] = uniform(rng);
  }
}

Real SimpleMatrix::dotRow(const SimpleVector& vec, int64_t i) const {
  assert(i >= 0);
  assert(i < m_);
  assert(vec.size() == n_);
  Real d = 0.0;
  for (int64_t j = 0; j < n_; j++) {
    d += at(i, j) * vec.data_[j];
  }
  return d;
}

void SimpleMatrix::addRow(const SimpleVector& vec, int64_t i, Real a) {
  assert(i >= 0);
  assert(i < m_);
  assert(vec.size() == n_);
  for (int64_t j = 0; j < n_; j++) {
    data_[i * n_ + j] += a * vec.data_[j];
  }
}

void SimpleMatrix::multiplyRow(const SimpleVector& nums, int64_t ib, int64_t ie) {
  if (ie == -1) {ie = m_;}
  assert(ie <= nums.size());
  for (auto i = ib; i < ie; i++) {
    Real n = nums[i-ib];
    if (n != 0) {
      for (auto j = 0; j < n_; j++) {
        at(i, j) *= n;
      }
    }
  }
}

void SimpleMatrix::divideRow(const SimpleVector& denoms, int64_t ib, int64_t ie) {
  if (ie == -1) {ie = m_;}
  assert(ie <= denoms.size());
  for (auto i = ib; i < ie; i++) {
    Real n = denoms[i-ib];
    if (n != 0) {
      for (auto j = 0; j < n_; j++) {
        at(i, j) /= n;
      }
    }
  }
}

Real SimpleMatrix::l2NormRow(int64_t i) const {
  auto norm = 0.0;
  for (auto j = 0; j < n_; j++) {
    const Real v = at(i,j);
    norm += v * v;
  }
  return std::sqrt(norm);
}

void SimpleMatrix::l2NormRow(SimpleVector& norms) const {
  assert(norms.size() == m_);
    for (auto i = 0; i < m_; i++) {
      norms[i] = l2NormRow(i);
    }
}

void SimpleMatrix::save(std::ostream& out) {
  out.write((char*) &m_, sizeof(int64_t));
  out.write((char*) &n_, sizeof(int64_t));
  out.write((char*) data_, m_ * n_ * sizeof(Real));
}

void SimpleMatrix::load(std::istream& in) {
  in.read((char*) &m_, sizeof(int64_t));
  in.read((char*) &n_, sizeof(int64_t));
  delete[] data_;
  data_ = new Real[m_ * n_];
  in.read((char*) data_, m_ * n_ * sizeof(Real));
}

void SimpleMatrix::shrinkSubset(const std::vector<int32_t> &subset) {
  int64_t new_m = subset.size();

  Real *new_data = new Real[new_m * n_];
  for (size_t k = 0; k < subset.size(); ++k) {
    int idx = subset[k];
    for (int i = 0; i < n_; ++i) {
      new_data[k * n_ + i] = data_[idx * n_ + i];
    }
  }

  delete[] data_;
  m_ = new_m;
  data_ = new_data;
}

void SimpleMatrix::convertColMajor() {
  Real *new_data = new Real[m_ * n_];
  // 遍历列
  int i = 0;
  for (int col = 0; col < n_; ++col) {
    for (int row = 0; row < m_; ++row) {
      new_data[i++] = data_[row * n_ + col];
    }
  }

  delete[] data_;
  data_ = new_data;
}

}
