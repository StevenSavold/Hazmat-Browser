#pragma once

extern "C" {
    #include "gumbo.h"
}

#include <string>

namespace hazmat::html {

    class Html 
    {
    public:
        Html(const std::string& htmlString);

        ~Html();

        std::string getText() const ;
        GumboOutput* Content() const ;
        

    private:
        static std::string cleantext(GumboNode* node);

    private:
        GumboOutput* m_ParserOutput;
    };



}