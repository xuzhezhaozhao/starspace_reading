
#include "openblas_util.h"

namespace starspace {

void cblas_dot(const SimpleVector &vec, const SimpleMatrix &mat,
               SimpleVector &output) {
  assert(output.m_ == mat.m_);
  int m = static_cast<int>(mat.m_);
  int n = static_cast<int>(mat.n_);
  Real alpha = 1.0;
  Real *a = mat.data_;
  int lda = m;
  Real *x = vec.data_;
  int incx = 1;
  Real beta = 0.0;
  Real *y = output.data_;
  int incy = 1;

  cblas_sgemv(CblasColMajor, CblasNoTrans, m, n,
      alpha, a, lda, x, incx, beta, y, incy);
}
}
