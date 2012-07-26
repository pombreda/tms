#ifndef _TMS_COMMON_PROTOCOL_MESSAGE__FILTER_HPP_
#define _TMS_COMMON_PROTOCOL_MESSAGE__FILTER_HPP_
#include <protocol/message/filter.pb.h>
// std
#include <iostream>
// boost
#include <boost/shared_ptr.hpp>
// common
#include <contraption/filter.hpp>
#include <contraption/filter/all_filter.hpp>
#include <contraption/filter/logical_connective.hpp>
#include <contraption/filter/compare_filter.hpp>
#include <protocol/message/record.hpp>
namespace tms {
namespace common {
namespace protocol {
namespace message {
typedef boost::shared_ptr<Filter> FilterP;
typedef boost::shared_ptr<AndFilter> AndFilterP;
typedef boost::shared_ptr<OrFilter> OrFilterP;
typedef boost::shared_ptr<NotFilter> NotFilterP;
typedef boost::shared_ptr<CompareFilter> CompareFilterP;
typedef boost::shared_ptr<AllFilter> AllFilterP;

FilterP GetFilter(const contraption::Filter &filter) {
  FilterP ret(new Filter());
  const contraption::AllFilter *all_filter 
      = dynamic_cast<const contraption::AllFilter*>(&filter);
  if (all_filter) {
    ret->mutable_all_filter();
  }
  const contraption::NotFilter *not_filter 
      = dynamic_cast<const contraption::NotFilter*>(&filter);
  if (not_filter) {
    NotFilter &nf = *ret->mutable_not_filter();
    *nf.mutable_filter() = *GetFilter(*not_filter->filter());
  }
  const contraption::OrFilter *or_filter 
      = dynamic_cast<const contraption::OrFilter*>(&filter);
  if (or_filter) {
    OrFilter &orf = *ret->mutable_or_filter();
    *orf.mutable_filter1() = *GetFilter(*or_filter->filter1());
    *orf.mutable_filter2() = *GetFilter(*or_filter->filter2());
  }
  const contraption::AndFilter *and_filter 
      = dynamic_cast<const contraption::AndFilter*>(&filter);
  if (and_filter) {
    AndFilter &andf = *ret->mutable_and_filter();
    *andf.mutable_filter1() = *GetFilter(*and_filter->filter1());
    *andf.mutable_filter2() = *GetFilter(*and_filter->filter2());
  }

  const contraption::CompareFilterT<int> *compare_filter_int
      = dynamic_cast<const contraption::CompareFilterT<int>*>(&filter);
  if (compare_filter_int) {
    CompareFilter &cf = *ret->mutable_compare_filter();
    switch(compare_filter_int->type()) {
      case contraption::kEqual:
        cf.set_type(CompareFilter::kEqual);
        break;
      case contraption::kNotEqual:
        cf.set_type(CompareFilter::kNotEqual);
        break;
      case contraption::kLesser:
        cf.set_type(CompareFilter::kLesser);
        break;
      case contraption::kNotLesser:
        cf.set_type(CompareFilter::kNotLesser);
        break;
      case contraption::kGreater:
        cf.set_type(CompareFilter::kGreater);
        break;
      case contraption::kNotGreater:
        cf.set_type(CompareFilter::kNotGreater);
        break;        
    }
    Record &record = *cf.mutable_record();
    record.set_type(Record::kInt);
    record.set_field(compare_filter_int->backend_name());
    record.set_int_value(
        static_cast<google::protobuf::uint32>(compare_filter_int->value()));
  }

  const contraption::CompareFilterT<std::string> *compare_filter_string
      = dynamic_cast<const contraption::CompareFilterT<std::string>*>(&filter);
  if (compare_filter_string) {
    CompareFilter &cf = *ret->mutable_compare_filter();
    switch(compare_filter_string->type()) {
      case contraption::kEqual:
        cf.set_type(CompareFilter::kEqual);
        break;
      case contraption::kNotEqual:
        cf.set_type(CompareFilter::kNotEqual);
        break;
      case contraption::kLesser:
        cf.set_type(CompareFilter::kLesser);
        break;
      case contraption::kNotLesser:
        cf.set_type(CompareFilter::kNotLesser);
        break;
      case contraption::kGreater:
        cf.set_type(CompareFilter::kGreater);
        break;
      case contraption::kNotGreater:
        cf.set_type(CompareFilter::kNotGreater);
        break;        
    }
    Record &record = *cf.mutable_record();
    record.set_type(Record::kString);
    record.set_field(compare_filter_string->backend_name());
    record.set_string_value(
        static_cast<google::protobuf::string>(compare_filter_string->value()));
  }
  return ret;
}

contraption::FilterCP GetFilter(const Filter &filter) {
  if (filter.has_all_filter()) {
    return contraption::FilterCP(new contraption::AllFilter());
  } 
  if (filter.has_not_filter()) {
    return Not(GetFilter(filter.not_filter().filter()));
  }
  if (filter.has_and_filter()) {
    return And(GetFilter(filter.and_filter().filter1()), 
               GetFilter(filter.and_filter().filter2()));
  }
  if (filter.has_or_filter()) {
    return Or(GetFilter(filter.or_filter().filter1()), 
              GetFilter(filter.or_filter().filter2()));
  }
  if (filter.has_compare_filter()) {
    const Record &record = filter.compare_filter().record();
    if (record.type() == Record::kInt) {
      switch(filter.compare_filter().type()) {
        case CompareFilter::kEqual:
          return Compare(record.field(), 
                         contraption::kEqual, 
                         record.int_value());
          break;
        case CompareFilter::kNotEqual:
          return Compare(record.field(), 
                         contraption::kNotEqual, 
                         record.int_value());
          break;
        case CompareFilter::kLesser:
          return Compare(record.field(), 
                         contraption::kLesser, 
                         record.int_value());
          break;
        case CompareFilter::kNotLesser:
          return Compare(record.field(), 
                         contraption::kNotLesser, 
                         record.int_value());
          break;
        case CompareFilter::kGreater:
          return Compare(record.field(), 
                         contraption::kGreater, 
                         record.int_value());
          break;
        case CompareFilter::kNotGreater:
          return Compare(record.field(), 
                         contraption::kNotGreater, 
                         record.int_value());
          break;        
      }
    }
    if (record.type() == Record::kString) {
      switch(filter.compare_filter().type()) {
        case CompareFilter::kEqual:
          return Compare(record.field(), 
                         contraption::kEqual, 
                         record.string_value());
          break;
        case CompareFilter::kNotEqual:
          return Compare(record.field(), 
                         contraption::kNotEqual, 
                         record.string_value());
          break;
        case CompareFilter::kLesser:
          return Compare(record.field(), 
                         contraption::kLesser, 
                         record.string_value());
          break;
        case CompareFilter::kNotLesser:
          return Compare(record.field(), 
                         contraption::kNotLesser, 
                         record.string_value());
          break;
        case CompareFilter::kGreater:
          return Compare(record.field(), 
                         contraption::kGreater, 
                         record.string_value());
          break;
        case CompareFilter::kNotGreater:
          return Compare(record.field(), 
                         contraption::kNotGreater, 
                         record.string_value());
          break;        
      }
    }
  }
  return contraption::FilterCP();
}

}
}
}
}
#endif // _TMS_COMMON_PROTOCOL_MESSAGE__FILTER_HPP_
