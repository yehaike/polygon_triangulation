#include <Geo/vector.hh>
#include <memory>

#include <vector>

namespace Geo {

template <size_t dimT>
struct IBsplineFitting
{
  struct IFunction
  {
    virtual Geo::Vector<dimT> evaluate(const double _t) const = 0;
    virtual Geo::Vector<dimT> closest_point(const Geo::Vector<dimT>& _pt) const = 0;
  };

  // Setup
  virtual bool init(const size_t _deg,
    const std::vector<double>& _knots, const IFunction& _f) = 0;
  virtual void set_parameter_correction_iterations(size_t _itr_nmbr) = 0;
  virtual void set_favour_boundaries(const bool _fvr_bndr = true) = 0;
  virtual void set_samples_per_interval(const size_t _smpl_per_intrvl = 4) = 0;

  // Compute
  virtual void compute() = 0;

  // Get results
  virtual const std::vector<Vector<dimT>>& X() const = 0;
  //virtual Vector<dimT> eval(const double _t) const = 0;

  /*! Factory */
  static std::shared_ptr<IBsplineFitting<dimT>> make();
};

}//namespace Geo
