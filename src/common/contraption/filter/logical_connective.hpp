#ifndef _TMS_COMMON_CONTRAPTION_FILTER__LOGICAL_CONNECTIVE_HPP_
#define _TMS_COMMON_CONTRAPTION_FILTER__LOGICAL_CONNECTIVE_HPP_
// common
#include <contraption/filter.hpp>

namespace tms {
namespace common {
namespace contraption {

class NotFilter : public Filter {
 public:
  NotFilter(FilterCP filter)
      throw() :
      filter_(filter) {}

  FilterCP filter() const {
    return filter_;
  }

  std::string ToString() const {
    return "Not (" + filter_->ToString() + ")";
  }
 private:
  FilterCP filter_;
};

inline FilterP Not(FilterCP filter) 
    throw() {
  return FilterP(new NotFilter(filter));
}

class AndFilter : public Filter {
 public:

  AndFilter(FilterCP filter1, FilterCP filter2) 
      throw() :
      filter1_(filter1),
      filter2_(filter2) {}

  FilterCP filter1() const {
    return filter1_;
  }

  FilterCP filter2() const {
    return filter2_;
  }

  std::string ToString() const {
    return "(" + filter1_->ToString() + ") AND (" + filter2_->ToString() + ")";
  }

 private:
  FilterCP filter1_;
  FilterCP filter2_;
};


inline FilterP And(FilterCP filter1, FilterCP filter2)
    throw() {
  return FilterP(new AndFilter(filter1, filter2));
}


class OrFilter : public Filter {
 public:
  OrFilter(FilterCP filter1, FilterCP filter2)
      throw() :
      filter1_(filter1),
      filter2_(filter2) {}

  FilterCP filter1() const {
    return filter1_;
  }

  FilterCP filter2() const {
    return filter2_;
  }

  std::string ToString() const {
    return "(" + filter1_->ToString() + ") OR (" + filter2_->ToString();
  }

 private:
  FilterCP filter1_;
  FilterCP filter2_;
};


inline FilterP Or(FilterCP filter1, FilterCP filter2) 
    throw() {
  return FilterP(new OrFilter(filter1, filter2));
}

}
}
}

#endif // _TMS_COMMON_CONTRAPTION_FILTER__LOGICAL_CONNECTIVE_HPP_
