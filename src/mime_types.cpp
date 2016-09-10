#include "mime_types.hpp"
#include <string>
#include <map>

namespace http {
namespace server {
namespace mime_types {


// Just for fun. I think the another method is better.
std::map<std::string, std::string> mappings
{
        { "gif", "image/gif" },
        { "htm", "text/html" },
        { "html", "text/html" },
        { "jpg", "image/jpeg" },
        { "png", "image/png" }
};

// *************** Another method *******************
//struct mapping
//{
//    const char* extension;
//    const char* mime_type;
//} mappings[] =
//{
//    { "gif", "image/gif" },
//    { "htm", "text/html" },
//    { "html", "text/html" },
//    { "jpg", "image/jpeg" },
//    { "png", "image/png" }
//};

std::string extension_to_type(const std::string& extension)
{
    for (auto m : mappings)
    {
        if (m.first == extension)
        {
            return m.second;
        }
    }

// *************** Another method ********************
    // for (mapping m : mappings)
    // {
    //     if (m.extension == extension)
    //     {
    //         return m.mime_type;
    //     }
    // }
    
    return "text/plain";
}

} // namespace mime_types
} // namespace server
} // namespace http
