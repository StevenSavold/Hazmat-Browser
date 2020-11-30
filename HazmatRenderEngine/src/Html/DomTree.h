#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "html.h"
#include "HtmlTags.h"

namespace hazmat::html {

    class DomTreeNode
    {
    public:
        using DomIterator = std::vector<std::unique_ptr<DomTreeNode>>::iterator;
        using ConstDomIterator = std::vector<std::unique_ptr<DomTreeNode>>::const_iterator;
    
        DomTreeNode() = default;
        DomTreeNode(DomTreeNode* parent);
        ~DomTreeNode() = default;

        void PushBackChild(std::unique_ptr<DomTreeNode> node);
        void SetInnerText(const std::string& innerText);

        inline Tag Type() const { return m_NodeType; };
        void SetType(Tag tagType);

        const DomTreeNode* Find(Tag tagType) const;

        const std::string& GetInnerText() const;

        const std::unordered_map<std::string, std::string>& GetAttributes() const;
        bool AddAttribute(const std::string& key, const std::string& value);

        DomIterator begin();
        DomIterator end();

        ConstDomIterator cbegin() const;
        ConstDomIterator cend() const;

    private:
        DomTreeNode* m_Parent = nullptr;
        std::vector<std::unique_ptr<DomTreeNode>> m_Children;
        std::unordered_map<std::string, std::string> m_Attributes;
        Tag m_NodeType = Tag::LAST;
        std::string m_InnerText;

    };

    std::unique_ptr<DomTreeNode> CreateDomTree(const std::string& htmlContent);
    std::unique_ptr<DomTreeNode> CreateDomTree(const Html& html);

}
