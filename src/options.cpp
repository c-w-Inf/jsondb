#include "options.h"

#include <stdexcept>

namespace jsondb {

option::option (int argc, char** argv) {
    host = "0.0.0.0";
    port = 10954;

    for (int i = 1; i < argc; ++i) {
        if (argv[i] == std::string ("-p") || argv[i] == std::string ("--port")) {
            if (i + 1 == argc) {
                throw std::invalid_argument ("an integer must be designated for --port (-p)");
            }
            try {
                port = std::stoi (argv[++i]);
            } catch (...) {
                throw std::invalid_argument ("the argument for --port (-p) must be an integer");
            }
        } else if (argv[i] == std::string ("-h") || argv[i] == std::string ("--host")) {
            if (i + 1 == argc) {
                throw std::invalid_argument ("a string must be designated for --host (-h)");
            }
            host = argv[i + 1];
        } else {
            throw std::invalid_argument ("unknown option");
        }
    }
}

}  // namespace jsondb
