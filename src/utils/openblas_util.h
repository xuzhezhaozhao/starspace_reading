
#pragma once

#include <assert.h>

#include "deps/openblas/include/cblas.h"
#include "simple_matrix.h"
#include "simple_vector.h"

namespace starspace {

void cblas_dot(const SimpleVector &vec, const SimpleMatrix &mat,
               SimpleVector &output);
}
