#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include "connection_manager.hpp"
#include "request_handler.hpp"

namespace http {
namespace server {

// The top-level class of the HTTP server.
class server
{
public:
    server(const server&) = delete;
    server& operator=(const server&) = delete;

    // Consturct the server to listen on the specified TCP address and
    // port, and server up files from the given directory.
    explicit server(const std::string& address, const std::string& port,
            const std::string& doc_root);

    // Run the server's io_service loop.
    void run();
private:

    // Perform an aysnchronous accept operations.
    void do_accept();

    // Wait for a request to stop the server.
    void do_await_stop();


// **************************  member variable  *************************

    // The io_service used to perform asynchronous operations.
    boost::asio::io_service io_service_;

    // The signal_set is used to register for prcess termination
    // notifications.
    boost::asio::signal_set signals_;

    // Acceptor used to listen for incoming connections.
    boost::asio::ip::tcp::acceptor acceptor_;

    // The connection manager which owns all live connections.
    connection_manager connection_manager_;

    // The next socket to be accepted.
    boost::asio::ip::tcp::socket socket_;

    // The handler for all incoming requests.
    request_handler request_handler_;
};

} // namespace server
} // namespace http

#endif // HTTP_SERVER_HPP
