#include <httplib.h>

#include <iostream>

#include "options.h"

int main (int argc, char** argv) {
    try {
        jsondb::option opt (argc, argv);

        httplib::Server server;

        server.set_pre_routing_handler ([opt] (const httplib::Request& req, httplib::Response) {
            if (opt.verbose) {
                const auto& cip = req.remote_addr;
                std::cout << cip << std::string (16 - cip.length (), ' ') << req.path << std::endl;
            }

            return httplib::Server::HandlerResponse::Unhandled;
        });

        server.Get ("/ver", [] (const httplib::Request& req, httplib::Response& res) {
            res.set_content ("0.0.1", "text/plain");
        });

        server.Get ("/body", [] (const httplib::Request& req, httplib::Response& res) {
            res.set_content ("{}", "application/json");
        });

        std::cout << "jsondb listening on " << opt.host << ':' << opt.port << std::endl;
        if (!server.listen (opt.host, opt.port)) {
            std::cout << "jsondb: cannot listen" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "jsondb: error: " << e.what () << std::endl;
    }

    return 0;
}
