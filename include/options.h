#pragma once

#include <string>

namespace jsondb {

struct option {
    std::string host;
    int port;

    option (int argc, char** argv);
};

}  // namespace jsondb
