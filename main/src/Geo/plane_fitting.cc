
#pragma once

#include "plane_fitting.hh"
#include "iterate.hh"
#include <Utils/error_handling.hh>
#include "Eigen/SVD"
#include <vector>


namespace Geo {

namespace {

struct PlaneFit : public IPlaneFit
{
  virtual void init(size_t _size) override;
  virtual void add_point(const Vector3& _pt) override;
  virtual bool compute(Vector3& _center, Vector3& _normal) override;

  Eigen::MatrixXd matr_;
  int ind_ = 0;
};

void PlaneFit::init(size_t _size)
{
  matr_.resize(3, _size);
  ind_ = 0;
}

void PlaneFit::add_point(const Vector3& _pt)
{
  THROW_IF(ind_ > matr_.cols(), "Adding too many points");
  iterate_forw<3>::eval([this, &_pt](size_t _i) {matr_(_i, ind_) = _pt[_i]; });
  ++ind_;
}

// Find the best plane for n points using the singular value decomposition.
// This is where I took the idea:
// http://math.stackexchange.com/questions/99299/best-fitting-plane-given-a-set-of-points?answertab=votes#tab-top
bool PlaneFit::compute(Vector3& _center, Vector3& _normal)
{
  if (ind_ == 0)
    return false;
  if (ind_ < matr_.cols())
    matr_.conservativeResize(Eigen::NoChange, ind_);

  Eigen::MatrixXd mid_pt(3, 1);
  mid_pt(0, 0) = mid_pt(1, 0) = mid_pt(2, 0) = 0;

  for (int i = 0; i < matr_.cols(); ++i)
    mid_pt += matr_.col(i);

  mid_pt /= double(matr_.cols());

  iterate_forw<3>::eval([&_center, &mid_pt](int _i) { _center[_i] = mid_pt(_i, 0); });

  for (int i = 0; i < matr_.cols(); ++i)
    matr_.col(i) -= mid_pt;

  Eigen::JacobiSVD<Eigen::MatrixXd> svd(matr_, Eigen::ComputeThinU);
  auto umatr = svd.matrixU();
  iterate_forw<3>::eval([&_normal, &umatr](int _i) { _normal[_i] = umatr(_i, 2); });
  return true;
}

}//namespace

std::shared_ptr<IPlaneFit> IPlaneFit::make()
{
  return std::make_shared<PlaneFit>();
}

}//namespace Geo
