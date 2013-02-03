#ifndef ENET_PLUS_BASE_ERROR_HPP_
#define ENET_PLUS_BASE_ERROR_HPP_

// WARNING: 'Error' class in NOT thread safe.
// TODO: make it thread safe.

#include <cstdarg>
#include <cstdio>

#include <vector>
#include <sstream>
#include <string>

#include <enet-plus/base/macros.hpp>

#include <enet-plus/dll.hpp>

namespace enet {

class Error {
public:
  ENET_PLUS_DECL static void Print() {
    std::vector<std::string>::const_iterator itr;
    for(itr = messages.begin(); itr != messages.end(); ++itr) {
      fprintf(stderr, "%s\n", itr->c_str());
    }
  }

  static void Throw(const char* file, unsigned int line, const char* fmt, ...) {
    static char buf[1024];
    
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf) - 1, fmt, args);
    va_end(args);
    
    std::stringstream ss;
    ss << "Error in file " << file << " on line " << line << ":" << std::endl;
    ss << buf << std::endl << '\0';
    
    Error::messages.push_back(ss.str());
  }

private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(Error);

  static std::vector<std::string> messages;
};

#define THROW_ERROR(msg, ...) enet::Error::Throw(__FILE__, __LINE__, msg, ##__VA_ARGS__)

} // namespace enet

#endif // ENET_PLUS_BASE_ERROR_HPP_
