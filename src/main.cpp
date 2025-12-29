#include <httplib.h>

#include <iostream>

int main () {
    httplib::Server server;
    server.Get ("/ver", [] (const httplib::Request& req, httplib::Response& res) {
        std::string client_ip = req.remote_addr;

        res.set_content ("0.0.1", "text/plain");

        std::cout << client_ip << std::string (16 - client_ip.length (), ' ') << req.path << std::endl;
    });

    std::cout << "jsondb listening..." << std::endl;
    server.listen ("0.0.0.0", 8080);

    return 0;
}
