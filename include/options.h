#pragma once

#include <string>

namespace jsondb {

struct option {
    std::string host = "0.0.0.0";
    int port = 10954;
    bool verbose = false;

    option (int argc, char** argv);
};

}  // namespace jsondb
