#ifndef _TMS_COMMON_PROTOCOL_MESSAGE__READ_RECORDS_REQUEST_HPP_
#define _TMS_COMMON_PROTOCOL_MESSAGE__READ_RECORDS_REQUEST_HPP_
#include <protocol/message/read_records_request.pb.h>
// boost
#include <boost/shared_ptr.hpp>
namespace tms {
namespace common {
namespace protocol {
namespace message {
typedef boost::shared_ptr<ReadRecordsRequest> ReadRecordsRequestP;
}
}
}
}
#endif // _TMS_COMMON_PROTOCOL_MESSAGE__READ_RECORDS_REQUEST_HPP_
