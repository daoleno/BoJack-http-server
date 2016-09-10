# BoJack HTTP Server

此项目是一个 C++11 风格的基于 boost::asio 及其示例的异步 HTTP Server

(PS: BoJack 是一只马的名字)

## 推荐阅读顺序

* main
* server
 * connection_manager
 * connection
   * request_parser
   * request_handler
     * request
     * reply
       * header
       * mime_types

## 编译

    g++版本: g++ (GCC) 6.1.1 20160621 (Red Hat 6.1.1-3)
    操作系统: Linux 4.7.2-201.fc24.x86_64
> g++ -o http_server *pp -lboost_thread -pthread

## 运行
> ./http_server localhost 8080 ../www  

## 相关说明
Boost::asio 大大简化了 C++ 网络编程时与系统的交互难度


你可以通过
《[Boost.Asio C++ 网络编程](https://mmoaay.gitbooks.io/boost-asio-cpp-network-programming-chinese/content/Chapter1.html)》
大致做一个了解

注释相对清晰, Enjoy It!
