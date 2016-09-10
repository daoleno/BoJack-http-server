#ifndef HTTP_CONNECTION_HPP
#define HTTP_CONNECTION_HPP

#include <array>
#include <memory>
#include <boost/asio.hpp>
#include "reply.hpp"
#include "request.hpp"
#include "request_handler.hpp"
#include "request_parser.hpp"

namespace http {
namespace server {

// Forward Declaration.
// It's important to use manager's operation.
class connection_manager;

// Represents a single connection from a client.
class connection
    // It enables you to get a valid shared_ptr instance to this,
    // when all you have is this. 
    : public std::enable_shared_from_this<connection>
{
public:
    connection(const connection&) = delete;
    connection& operator=(const connection&) = delete;
    
    // Construct a connection with the given socket.
    explicit connection(boost::asio::ip::tcp::socket socket,
        connection_manager& manager, request_handler& handler);

    // Start the first asynchronous operation for connection.
    void start();

    // Stop all asynchronous operations associated with the connection.
    void stop();

private:
    // Perform an asynchronous read operation.
    void do_read();

    // Perform an asynchronous write operation.
    void do_write();

    // Socket for connection.
    boost::asio::ip::tcp::socket socket_;

    // The manager for this connection.
    connection_manager& connection_manager_;

    // Buffer for incoming data.
    std::array<char, 8192> buffer_;

    // The incoming request.
    request request_;
    
    // The reply to be sent back to the client.
    reply reply_;

    // The parser for the incoming request.
    request_parser request_parser_;

    // The handler used to process the incoming request.
    request_handler& request_handler_;
};

typedef std::shared_ptr<connection> connection_ptr;

} // namespace server
} // namespace http

#endif // HTTP_CONNECTION_HPP
