#ifndef _mime_hpp_
#define _mime_hpp_

#include <map>
#include <string>

namespace eck {
  namespace mime {
    void initialize(void);
    void finish(void);
    std::string get_content_type(const std::string &);
  }
}

#endif
