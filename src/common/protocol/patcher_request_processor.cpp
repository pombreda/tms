#include "patcher_request_processor.hpp"
// std
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <fstream>
// log4cplus
#include <log4cplus/loggingmacros.h>
// protobuf
#include <google/protobuf/text_format.h>
//boost
#include <boost/filesystem.hpp>
// common
#include <protocol/message/patch_client_request.hpp>
#include <protocol/message/patch_client_response.hpp>
#include <protocol/message/patch_server_request.hpp>
#include <protocol/message/error_response.hpp>
#include <protocol/server.hpp>
#include <string/string.hpp>
#include <contraption/field_type.hpp>
#include <contraption/record.hpp>
#include <patcher/patcher.hpp>

using namespace std;
using namespace tms::common::protocol;
using namespace tms::common::contraption;
using namespace tms::common::string;
using namespace tms::common::protocol::message;
using namespace tms::common::patcher;
using namespace boost::filesystem;

PatcherRequestProcessor
::PatcherRequestProcessor(Server &server,
			RequestProcessorP request_processor, 
			ModelP users,
			const SOCIDBScheme &scheme) :
  ModelBackendRequestProcessor(server, request_processor, users, scheme) {
}

PatcherRequestProcessor
::PatcherRequestProcessor(RequestProcessorP request_processor, 
			ModelP users,
			const SOCIDBScheme &scheme) :
  ModelBackendRequestProcessor(request_processor, users, scheme) {
  }

RequestProcessorP PatcherRequestProcessor::Duplicate() const {
  return RequestProcessorP(
      new PatcherRequestProcessor(*server_,
				  request_processor_->Duplicate(), 
				  users_, 
				  scheme_));
}


MessageP PatcherRequestProcessor::PatchServer(
			     const PatchServerRequest &request, 
			     Server &server) {
  LOG4CPLUS_INFO(logger_, 
		 WStringFromUTF8String("Patching server"));  
  ContraptionP user = server.Get<ContraptionP>("user");  
  if (!user->Get<int>("admin")) {
    return MessageP(new ErrorResponse());
  }
  server.Stop();
  ofstream fout("latest.patch", ios::out | ios::binary);
  fout.write(request.patch_archive().c_str(), request.patch_archive().size());
  fout.close();
  Patch(path("latest.patch"), current_path());  
  LOG4CPLUS_INFO(logger_, 
		 WStringFromUTF8String("Patch request evaled"));  
  exit(0);
  return MessageP();
}

MessageP PatcherRequestProcessor::PatchClient(
			     const PatchClientRequest &request,
			     Server &/*server*/) {
  LOG4CPLUS_INFO(logger_, 
		 WStringFromUTF8String("Patching client"));  
  
  if (request.version() != kVersion) {
  
    path patch = current_path() / path("patches") 
      / path(request.version()) / path(request.system());
    
    ostringstream sout;
    if (exists(patch)) {
      LOG4CPLUS_INFO(logger_, 
		     WStringFromUTF8String("Patch " + patch.string() + " found"));
      ifstream fin(patch.string().c_str(), ios::in | ios::binary);
      
      const int bufsize = 10240;
      char buf[bufsize];
      while (!fin.eof()) {
	fin.read(buf, bufsize);
	sout.write(buf, fin.gcount());
      }
    }  else {
      LOG4CPLUS_INFO(logger_, 
		     WStringFromUTF8String("Patch " + patch.string() + " not found"));
    }
    PatchClientResponseP response(new PatchClientResponse());
    response->set_patch_archive(sout.str());
    return response;
  } else {
    return MessageP(new ErrorResponse());
  }
}


MessageP PatcherRequestProcessor::Eval(const Message &message, Server &server) {
  if (server_->Check("user")) {

  }
  return LoginRequestProcessor::Eval(message, server);
}

void PatcherRequestProcessor::Register(SimpleRequestProcessor &processor) {
  processor.AddHandler(static_cast< boost::function<MessageP (const PatchServerRequest&, Server &server)> >(
      boost::bind(&PatcherRequestProcessor::PatchServer,
                  _1,
                  _2)));
  processor.AddHandler(static_cast< boost::function<MessageP (const PatchClientRequest&, Server &server)> >(
      boost::bind(&PatcherRequestProcessor::PatchClient,
                  _1,
                  _2)));
   
}
