#include "DomTree.h"

namespace hazmat::html {

    static std::unique_ptr<DomTreeNode> ConstructDom(GumboNode* node);

    DomTreeNode::DomTreeNode(DomTreeNode* parent)
        : m_Parent(parent) {}

    void DomTreeNode::PushBackChild(std::unique_ptr<DomTreeNode> node)
    {
        node->m_Parent = this;
        m_Children.push_back(std::move(node));
    }

    void DomTreeNode::SetInnerText(const std::string& innerText)
    {
        m_InnerText = innerText;
    }

    void DomTreeNode::SetType(Tag tagType)
    {
        m_NodeType = tagType;
    }

    const DomTreeNode* DomTreeNode::Find(Tag tagType) const
    {
        if (m_NodeType == tagType)
            return this;

        for (const auto& child : m_Children)
        {
            auto result = child->Find(tagType);
            if (result)
                return result;
        }    

        return nullptr;
    }

    const std::string& DomTreeNode::GetInnerText() const
    {
        if (m_NodeType != Tag::LAST && m_Children.size() == 1)
        {
            return m_Children[0]->m_InnerText;
        }
        else
        {
            return m_InnerText;
        }
    }

    const std::unordered_map<std::string, std::string>& DomTreeNode::GetAttributes() const
    {
        return m_Attributes;
    }

    bool DomTreeNode::AddAttribute(const std::string& key, const std::string& value)
    {
        if (auto& iter = m_Attributes.find(key); iter != m_Attributes.end())
            return false;

        m_Attributes[key] = value;
        return true;
    }


    DomTreeNode::DomIterator DomTreeNode::begin() { return m_Children.begin(); }
    DomTreeNode::DomIterator DomTreeNode::end() { return m_Children.end(); }

    DomTreeNode::ConstDomIterator DomTreeNode::cbegin() const { return m_Children.cbegin(); }
    DomTreeNode::ConstDomIterator DomTreeNode::cend() const { return m_Children.cend(); }




    std::unique_ptr<DomTreeNode> CreateDomTree(const std::string& htmlContent)
    {
        return CreateDomTree(Html{ htmlContent });
    }
    
    std::unique_ptr<DomTreeNode> CreateDomTree(const Html& html)
    {
        return ConstructDom(html.Content()->root);
    }

    static std::unique_ptr<DomTreeNode> ConstructDom(GumboNode* node)
    {
        std::unique_ptr<DomTreeNode> output;
        /** Document node.  v will be a GumboDocument. */
        //GUMBO_NODE_DOCUMENT,
        /** Element node.  v will be a GumboElement. */
        //GUMBO_NODE_ELEMENT,
        /** Text node.  v will be a GumboText. */
        //GUMBO_NODE_TEXT,
        /** CDATA node. v will be a GumboText. */
        //GUMBO_NODE_CDATA,
        /** Comment node.  v will be a GumboText, excluding comment delimiters. */
        //GUMBO_NODE_COMMENT,
        /** Text node, where all contents is whitespace.  v will be a GumboText. */
        //GUMBO_NODE_WHITESPACE,
        /** Template node.  This is separate from GUMBO_NODE_ELEMENT because many
            * client libraries will want to ignore the contents of template nodes, as
            * the spec suggests.  Recursing on GUMBO_NODE_ELEMENT will do the right thing
            * here, while clients that want to include template contents should also
            * check for GUMBO_NODE_TEMPLATE.  v will be a GumboElement.  */
        //GUMBO_NODE_TEMPLATE
        
        if (node->type == GUMBO_NODE_DOCUMENT)
        {
            __debugbreak();
        }
        else if (node->type == GUMBO_NODE_ELEMENT)
        {
            output = std::make_unique<DomTreeNode>();

            // --> Assign attribs and tag somewhere here <-- //
            output->SetType((Tag)node->v.element.tag);

            for (int i = 0; i < node->v.element.attributes.length; ++i)
            {
                GumboAttribute* gumboAttrib = (GumboAttribute*)(node->v.element.attributes.data[i]);
                std::string name = gumboAttrib->name;
                std::string value = gumboAttrib->value;
                if (!(output->AddAttribute(name, value)))
                {
                    __debugbreak();
                }
            }

            GumboVector* children = &node->v.element.children;
            for (unsigned int i = 0; i < children->length; ++i)
            {
                auto domChild = ConstructDom((GumboNode*)children->data[i]);

                // If the domChild exists, add the to the list of children
                if (domChild) 
                    output->PushBackChild(std::move(domChild));
            }
            return output;
        }
        else if (node->type == GUMBO_NODE_TEXT)
        {
            output = std::make_unique<DomTreeNode>();
            output->SetInnerText(node->v.text.text);
            return output;
        }
        else if (node->type == GUMBO_NODE_CDATA)
        {
            __debugbreak();
        }
        else /* GUMBO_NODE_COMMENT, GUMBO_NODE_WHITESPACE, GUMBO_NODE_TEMPLATE */
        {
            return nullptr;
        }
    }

}
