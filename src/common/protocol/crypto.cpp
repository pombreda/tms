#include "crypto.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>

using namespace tms::common::protocol;
using namespace std;

namespace tms {
namespace common {
namespace protocol {

string sha256(const string &str) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, str.c_str(), str.size());
  SHA256_Final(hash, &sha256);
  stringstream ret;
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
  {
    ret << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]);
  }
  return ret.str();
}

}
}
}
