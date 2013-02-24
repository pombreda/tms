// server
#include <server/server.hpp>
#include <boost/program_options.hpp>
#include <iostream>
// log4cplus
#include <log4cplus/configurator.h>
// common
#include <string/string.hpp>

using namespace tms::server;
using namespace tms::common::protocol;
using namespace tms::common::string;
using namespace std;
using namespace log4cplus;
Logger logger = Logger::getInstance(WStringFromUTF8String("server"));
namespace po = boost::program_options;

#include <iostream>
using namespace std;
int main(int argc, char** argv){
  cerr << "Server stoped." << endl;
  try {    
    PropertyConfigurator config(WStringFromUTF8String("log.cfg"));
    config.configure();

    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "produce help message")
      ("port", po::value<std::string>(), "port to listen")
      ("db", po::value<std::string>(), "file with sqlite3 database")
      ("init", "initialize db schema (this will erase all data in db)")
      ;
    
    po::variables_map vm;        
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    
    
    if (vm.count("help")) {
      cout << desc << "\n";
      return 0;
    }

    if (vm.count("init")) {
      if (vm.count("db")) {
        LOG4CPLUS_INFO(logger, 
                       WStringFromUTF8String("Initializing database" ));
	cerr << "Initializing database..." << endl;
        InitSchema(vm["db"].as<string>());
        LOG4CPLUS_INFO(logger, 
                       WStringFromUTF8String("Database initialized" ));  
	cerr << "Database initialized." << endl;
        return 0;
      } else {
        cerr << "error: db must be provided if init flag is passed\n";
      }
    }        
    
    if (vm.count("db") && vm.count("port")) {
      LOG4CPLUS_INFO(logger, 
		     WStringFromUTF8String("Starting server" ));  

      cerr << "Server started." << endl;
      try {
	CreateServer(vm["port"].as<string>(), vm["db"].as<string>())->ListenHere(); 
      } catch(exception &e) {
	LOG4CPLUS_INFO(logger, 
		       WStringFromUTF8String(e.what()));  
      } catch (...) {
	LOG4CPLUS_INFO(logger, 
		       WStringFromUTF8String("Unknown problem occured"));  
	
      }
      LOG4CPLUS_INFO(logger, 
		     WStringFromUTF8String("Server stoped." ));  

      cerr << "Server stoped.";
    } else {
      cerr << "error: db and port must be provided\n";
    }
  }
  catch(exception& e) {
    cerr << "error: " << e.what() << "\n";
    return 1;
  }
  catch(...) {
    cerr << "Exception of unknown type!\n";
  }
  return 0;
}
