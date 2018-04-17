
#pragma once

#include <assert.h>

#include "deps/openblas/include/cblas.h"
#include "simple_matrix.h"
#include "simple_vector.h"
#include "../matrix.h"

namespace starspace {

void cblas_vec_dot_matrix(const SimpleVector &vec, const SimpleMatrix &mat,
               SimpleVector &output);

// vec must be one row matrix
void cblas_vec_dot_matrix(const Matrix<Real> &vec, const SimpleMatrix &mat,
               SimpleVector &output);
}
