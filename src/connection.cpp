#include "connection.hpp"
#include <utility>
#include <cstring>

#include "connection_manager.hpp"

namespace http {
namespace server {

connection::connection(boost::asio::ip::tcp::socket socket,
    connection_manager& manager, request_handler& handler)
    : socket_(std::move(socket)),
      connection_manager_(manager),
      request_handler_(handler)
{
}

void connection::start()
{
    do_read();
}

void connection::stop()
{
    // Close a specfied socket conection.JUST STOP HERE! 
    // Grace closure in "connection_manager.cpp
    socket_.close();
}

void connection::do_read()
{
    // In order to make sure that connection object outlives the 
    // asynchronous operation:
    auto self(shared_from_this());
    
    // "asio::async_read_some()" reads "some" bytes and does not promise 
    // to do more than that.
    // But "asio::async_read()" read ensure that the requested amount of 
    // data is read before the asynchronous operation completes.
    socket_.async_read_some(boost::asio::buffer(buffer_),
            // Handle data
            [this, self](boost::system::error_code ec, std::size_t bytes_transferred)
            {
              if (!ec)
              {
                request_parser::result_type result;
                // unpack a std::tuple from request_parser::parse()
                std::tie(result, std::ignore) = request_parser_.parse(
                        request_, buffer_.data(), buffer_.data() + bytes_transferred);

                if (result == request_parser::good)
                {
                  request_handler_.handle_request(request_, reply_);
                  do_write();
                }
                else
                {
                  do_read();
                }
              }
              else if (ec != boost::asio::error::operation_aborted)
              {
                  connection_manager_.stop(self);
              }
            });
}

void connection::do_write()
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_, reply_.to_buffers(),
            [this, self](boost::system::error_code ec, std::size_t)
            {
              if (!ec)
              {
              // Initiate graceful connection closure.
              boost::system::error_code ignored_ec;
              // Shutdown both send and receive on the socket.
              // For portable behaviour with respect to graceful 
              // closure of a connected socket.
              socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
              }
              if ( ec != boost::asio::error::operation_aborted)
              {
                connection_manager_.stop(self);
              }
            });
}

} // namespace server
} // namespace http



