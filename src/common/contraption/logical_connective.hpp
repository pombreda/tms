#ifndef _TMS_COMMON_CONTRAPTION__LOGICAL_CONNECTIVE_HPP_
#define _TMS_COMMON_CONTRAPTION__LOGICAL_CONNECTIVE_HPP_
// boost
#include <boost/shared_ptr.hpp>

namespace tms {
namespace common {
namespace contraption {

template<class Formula>
class NotT : public Formula {
 public:
  typedef boost::shared_ptr<Formula> FormulaP;
  NotT(FormulaP formula)
      throw() :
      formula_(formula) {}
  FormulaP formula() {
    return formula_;
  }
 private:
  FormulaP formula_;
};

template<class Formula>
NotT<Formula> Not(boost::shared_ptr<Formula> formula) 
    throw() {
  return NotT<Formula>(formula);
}

template<class Formula>
class AndT : public Formula {
 public:
  typedef boost::shared_ptr<Formula> FormulaP;
  AndT(FormulaP formula1, FormulaP formula2) 
      throw() :
      formula1_(formula1),
      formula2_(formula2) {}
  FormulaP formula1() {
    return formula1_;
  }
  FormulaP formula2() {
    return formula2_;
  }
 private:
  FormulaP formula1_;
  FormulaP formula2_;
};

template<class Formula>
AndT<Formula> And(boost::shared_ptr<Formula> formula1, boost::shared_ptr<Formula> formula2)
    throw() {
  return AndT<Formula>(formula1, formula2);
}

template<class Formula>
class OrT : public Formula {
 public:
  typedef boost::shared_ptr<Formula> FormulaP;
  OrT(FormulaP formula1, FormulaP formula2)
      throw() :
      formula1_(formula1),
      formula2_(formula2) {}
  FormulaP formula1() {
    return formula1_;
  }
  FormulaP formula2() {
    return formula2_;
  }
 private:
  FormulaP formula1_;
  FormulaP formula2_;
};

template<class Formula>
OrT<Formula> Or(boost::shared_ptr<Formula> formula1, boost::shared_ptr<Formula> formula2) 
    throw() {
  return OrT<Formula>(formula1, formula2);
}

}
}
}

#endif // _TMS_COMMON_CONTRAPTION__LOGICAL_CONNECTIVE_HPP_
