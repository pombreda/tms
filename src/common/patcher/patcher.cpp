#include "patcher.hpp"
// common
#include <archive/archive.hpp>
#include <string/string.hpp>
// boost
#include <boost/filesystem.hpp>
// log4cplus
#include <log4cplus/loggingmacros.h>
#include <log4cplus/logger.h>
// std
#include <fstream>
#include <cstdio>
#include <iostream>

using namespace tms::common::patcher;
using namespace tms::common::string;
using namespace tms::common::archive;
using namespace boost::filesystem;
using namespace log4cplus;
using namespace std; 
Logger logger = Logger::getInstance(WStringFromUTF8String("patcher"));
void tms::common::patcher::Patch(path patch, path root) {
  path patch_folder = root / path("patch");
  remove_all(patch_folder);
  create_directory(patch_folder);
  Decompress(patch, root);
  path patch_description = patch_folder / path("description");
  ifstream descr(patch_description.string().c_str());
  const int bufsize = 1024;
  char buf[bufsize];
  LOG4CPLUS_INFO(logger, 
		 WStringFromUTF8String("Applying patch." ));  
  while (!descr.eof()) {
    descr.getline(buf, bufsize);
    if (std::string(buf) == "") break;
    path patched_file = patch_folder / path(buf);
    path dest_file = root / path(buf);
    LOG4CPLUS_INFO(logger, 
		   WStringFromUTF8String("Patching " 
					 + patched_file.string() + "." ));  
    if (exists(dest_file) && is_directory(dest_file)) {
      continue;
    }
    if (exists(dest_file)) {
      rename(dest_file, path(dest_file.string() + ".old"));
    }
    copy(patched_file, dest_file);
  }
  LOG4CPLUS_INFO(logger, 
		 WStringFromUTF8String("Patch applied." ));  

}
