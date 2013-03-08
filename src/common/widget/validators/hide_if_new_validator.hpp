#ifndef _TMS_COMMON_WIDGET_VALIDATORS__HIDE_IF_NEW_VALIDATOR_HPP_
#define _TMS_COMMON_WIDGET_VALIDATORS__HIDE_IF_NEW_VALIDATOR_HPP_

class StringValidator : public wxTextValidator {
 public:
  typedef boost::function<std::string ()> GetterFunction;
  typedef boost::function<void (std::string)> SetterFunction;
  StringValidator(GetterFunction getter, SetterFunction setter);
  StringValidator(const StringValidator &validator);
  bool TransferToWindow();
  bool TransferFromWindow();
  wxObject* Clone() const;
 protected:
  GetterFunction getter_;
  SetterFunction setter_;
};

#endif // _TMS_COMMON_WIDGET_VALIDATORS__HIDE_IF_NEW_VALIDATOR_HPP_
