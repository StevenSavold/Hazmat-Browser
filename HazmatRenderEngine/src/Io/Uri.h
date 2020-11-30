#pragma once

#include <string>
#include <unordered_map>

namespace hazmat::io {
    struct Uri
    {
    public:
        Uri() = default;
        Uri(const Uri& copy);
        Uri(Uri&& move) noexcept;

        Uri(const std::string& uri);

        Uri& operator=(const Uri& copy);
        Uri& operator=(Uri&& move) noexcept;
        
    public:
        std::string UriText;

        std::string QueryString;
        std::string Path;
        std::string Protocol;
        std::string Host;
        std::string Port;

        enum class Protocols {
            http = 0,
            https,
            file
        };

        static std::unordered_map<std::string, Uri::Protocols> ProtocolMap;

    private: 
        void Parse();
    };
}