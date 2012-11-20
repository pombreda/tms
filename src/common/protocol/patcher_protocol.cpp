#include "patcher_protocol.hpp"
// common
#include <protocol/message/patch_client_request.hpp>
#include <protocol/message/patch_client_response.hpp>
#include <protocol/message/patch_server_request.hpp>
using namespace tms::common::protocol;
using namespace tms::common::protocol::message;

PatcherProtocol::PatcherProtocol() 
    throw(ProtocolException): 
    ModelBackendProtocol() {
  AddClasses();
}

PatcherProtocol::PatcherProtocol(const Protocol &other) 
    throw(ProtocolException): 
    ModelBackendProtocol(other) {
  AddClasses();
}

void PatcherProtocol::AddClasses() 
    throw(ProtocolException) {
  AddMessageClass<PatchClientResponse>();
  AddMessageClass<PatchServerRequest>();
  AddMessageClass<PatchClientRequest>();

}
