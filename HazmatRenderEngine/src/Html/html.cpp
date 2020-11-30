#include "html.h"

namespace hazmat::html {
    
    Html::Html(const std::string& htmlString)
        : m_ParserOutput(gumbo_parse(htmlString.c_str()))
    {}

    Html::~Html()
    {
        gumbo_destroy_output(&kGumboDefaultOptions, m_ParserOutput);
    }

    std::string Html::getText() const 
    {
        return Html::cleantext(this->m_ParserOutput->root);
    }

    GumboOutput* Html::Content() const  { return m_ParserOutput; }

    std::string Html::cleantext(GumboNode* node)
    {
        if (node->type == GUMBO_NODE_TEXT)
        {
            return std::string(node->v.text.text);
        }
        else if (node->type == GUMBO_NODE_ELEMENT &&
            node->v.element.tag != GUMBO_TAG_SCRIPT &&
            node->v.element.tag != GUMBO_TAG_STYLE)
        {
            std::string contents = "";
            GumboVector* children = &node->v.element.children;
            for (unsigned int i = 0; i < children->length; ++i)
            {
                const std::string text = cleantext((GumboNode*)children->data[i]);
                if (i != 0 && !text.empty())
                {
                    contents.append(" ");
                }
                contents.append(text);
            }
            return contents;
        }
        else
        {
            return "";
        }
    }

}