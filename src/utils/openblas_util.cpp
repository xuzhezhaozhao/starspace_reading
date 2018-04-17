
#include "openblas_util.h"
#include "../model.h"

namespace {
  using starspace::Real;
  void cblas_sgemv_helper(const Real *a, const Real *x, Real *y, int m, int n) {
    Real alpha = 1.0;
    int lda = m;
    int incx = 1;
    Real beta = 0.0;
    int incy = 1;
    cblas_sgemv(CblasColMajor, CblasNoTrans, m, n,
        alpha, a, lda, x, incx, beta, y, incy);
  }
}

namespace starspace {

void cblas_vec_dot_matrix(const SimpleVector &vec, const SimpleMatrix &mat,
               SimpleVector &output) {
  assert(output.m_ == mat.m_);
  assert(mat.n_ == vec.m_);

  int m = static_cast<int>(mat.m_);
  int n = static_cast<int>(mat.n_);
  const Real *a = mat.data_;
  const Real *x = vec.data_;
  Real *y = output.data_;
  cblas_sgemv_helper(a, x, y, m, n);
}

void cblas_vec_dot_matrix(const Matrix<Real> &vec, const SimpleMatrix &mat,
               SimpleVector &output) {
  assert(vec.getDims().r == 1);
  assert(vec.getDims().r == 1);

  int m = static_cast<int>(mat.m_);
  int n = static_cast<int>(mat.n_);
  const Real *a = mat.data_;
  const Real *x = vec.matrix.data().begin();
  Real *y = output.data_;
  cblas_sgemv_helper(a, x, y, m, n);
}

}
