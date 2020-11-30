#include "URI.h"

#include <algorithm>    // find
#include <iterator>


namespace hazmat::io {

    std::unordered_map<std::string, Uri::Protocols> Uri::ProtocolMap = {
        {"file",  Uri::Protocols::file },
        {"http",  Uri::Protocols::http },
        {"https", Uri::Protocols::https }
    };

    Uri::Uri(const std::string& uri)
        : UriText(uri)
    {
        Parse();
    }

    Uri::Uri(const Uri& copy)
        : UriText(copy.UriText)
    {
        Parse();
    }

    Uri::Uri(Uri&& move) noexcept
        : UriText(move.UriText),
          QueryString(move.QueryString),
          Path(move.Path),
          Protocol(move.Protocol),
          Host(move.Host),
          Port(move.Port)
    {}

    Uri& Uri::operator=(const Uri& copy) 
    {
        this->UriText = copy.UriText;
        Parse();

        return *this;
    }

    Uri& Uri::operator=(Uri&& move) noexcept
    {
        this->UriText = move.UriText;
        this->Path = move.Path;
        this->Protocol = move.Protocol;
        this->Host = move.Host;
        this->Port = move.Port;

        return *this;
    }

    void Uri::Parse()
    {
        using iterator_t = std::string::const_iterator;

        // Clear the fields before parsing
        Protocol = "";
        Host = "";
        Port = "";
        Path = "";
        QueryString = "";

        if (UriText.length() == 0)
            return;

        iterator_t uriEnd = UriText.end();

        // get query start
        iterator_t queryStart = std::find(UriText.cbegin(), uriEnd, '?');

        // protocol
        iterator_t protocolStart = UriText.begin();
        iterator_t protocolEnd = std::find(protocolStart, uriEnd, ':');

        if (protocolEnd != uriEnd)
        {
            std::string prot = &*(protocolEnd);
            if ((prot.length() > 3) && (prot.substr(0, 3) == "://"))
            {
                Protocol = std::string(protocolStart, protocolEnd);
                protocolEnd += 3;   //      ://
            }
            else
                protocolEnd = UriText.begin();  // no protocol
        }
        else
            protocolEnd = UriText.begin();  // no protocol

        // host
        iterator_t hostStart = protocolEnd;
        iterator_t pathStart = std::find(hostStart, uriEnd, '/');  // get pathStart

        iterator_t hostEnd = std::find(protocolEnd,
            (pathStart != uriEnd) ? pathStart : queryStart,
            ':');  // check for port

        Host = std::string(hostStart, hostEnd);

        // port
        if ((hostEnd != uriEnd) && ((&*(hostEnd))[0] == ':'))  // we have a port
        {
            hostEnd++;
            iterator_t portEnd = (pathStart != uriEnd) ? pathStart : queryStart;
            Port = std::string(hostEnd, portEnd);
        }

        // path
        if (pathStart != uriEnd)
            Path = std::string(pathStart, queryStart);

        // query
        if (queryStart != uriEnd)
            QueryString = std::string(queryStart, UriText.cend());

        return;
    }
}