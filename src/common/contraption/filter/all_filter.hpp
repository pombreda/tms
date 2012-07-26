#ifndef _TMS_COMMON_CONTRAPTION__ALL_FILTER_HPP_
#define _TMS_COMMON_CONTRAPTION__ALL_FILTER_HPP_
namespace tms {
namespace common {
namespace contraption {

class AllFilter : public Filter {
  std::string ToString() const {
    return "All";
  }
};

FilterP All() {
  return FilterP(new AllFilter);
}

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__ALL_FILTER_HPP_
