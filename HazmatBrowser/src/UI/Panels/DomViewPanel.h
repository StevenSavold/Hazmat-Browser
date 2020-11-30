#pragma once
#include "HTML/DomTree.h"


namespace hazmat::ui {

    class DomViewPanel
    {
    public:
        DomViewPanel() = default;
        ~DomViewPanel() = default;

        void Draw(html::DomTreeNode& domRoot);
        void UnselectDomNode();

    private:
        void DrawNode(html::DomTreeNode& node);
        void DrawNodeProperties(html::DomTreeNode& node);

    private:
        html::DomTreeNode* m_SelectedNode = nullptr;
    };

}