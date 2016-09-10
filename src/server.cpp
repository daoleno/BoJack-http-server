#include "server.hpp"
#include <signal.h>
#include <iostream>
#include <utility>

namespace http {
namespace server {

server::server(const std::string& address, const std::string& port,
    const std::string& doc_root)
        : io_service_(),
          signals_(io_service_),
          acceptor_(io_service_),
          connection_manager_(),
          socket_(io_service_),
          request_handler_(doc_root)
{

    // Add a set of signals to signale_set that handle quiting operation
    // about server.
    signals_.add(SIGINT);
    signals_.add(SIGTERM);

// Check if it was defined before using it,SIGOUT is not supported by
// Windows.
#if defined(SIGQUIT)
    signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)

    do_await_stop();



    // Open the acceptor 
    boost::asio::ip::tcp::resolver resolver(io_service_);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
    acceptor_.open(endpoint.protocol());

    // Allow the socket to be bound to an address that already in use.
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    do_accept();
}

void server::run()
{
    // The io_service::run() call will block until all asynchronous
    // operations have finished. While the server is running, there is
    // always at least one asynchronous operation outstanding; the 
    // asychronous accept all waiting for new incoming connections.
    io_service_.run();
}

void server::do_accept()
{
    // Then async_accept() is called to accept the first connection 
    // attempt. A socket has to be passed as a first parameter to 
    // async_accept(), which will be used to send and receive data on a 
    // new connection.
    acceptor_.async_accept(socket_,
            [this](boost::system::error_code ec)
            {
                // Check whether the server was  stopped by a signal
                // before this completion handler had a chance to run.
                if (!acceptor_.is_open())
                {
                    return;
                }

                if (!ec)
                {
                    // Acccept succeeded
                    connection_manager_.start(
                        std::make_shared<connection>(std::move(socket_),
                        connection_manager_, request_handler_));
                }

                do_accept();
            });
}

void server::do_await_stop()
{
    signals_.async_wait(
            [this](boost::system::error_code ec, int signal_number)
            {
                // The server is stopped by cancelling all outstanding
                // asynchronous operations. Once all operations have
                // finished the io_service::run()
                // call will exit.
                std::cerr << ec.message() << std::endl;
                if (!ec)
                {
                    std::cerr << "received signal " << signal_number
                              << std::endl;
                    acceptor_.close();
                    connection_manager_.stop_all();
                }
            });
}

} // namespace server
} // namespace http

