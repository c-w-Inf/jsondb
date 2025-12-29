#include <httplib.h>

#include <iostream>

#include "options.h"

int main (int argc, char** argv) {
    try {
        jsondb::option opt (argc, argv);

        httplib::Server server;
        server.Get ("/ver", [] (const httplib::Request& req, httplib::Response& res) {
            std::string client_ip = req.remote_addr;

            res.set_content ("0.0.1", "text/plain");

            std::cout << client_ip << std::string (16 - client_ip.length (), ' ') << req.path << std::endl;
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
