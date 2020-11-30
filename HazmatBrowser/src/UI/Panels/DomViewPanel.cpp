#include "DomViewPanel.h"
#include <imgui.h>

#include "HTML/HtmlTags.h"


namespace hazmat::ui {

    void DomViewPanel::Draw(html::DomTreeNode& domRoot) 
    {
        ImGui::Begin("Dom View");

        // Recursevly draw the nodes
        DrawNode(domRoot);  

        // If there is a node which has been selected,
        // Draw its properties in a section under the DomView
        if (m_SelectedNode)
            DrawNodeProperties(*m_SelectedNode);

        ImGui::End(); // End Dom View
    }

    void DomViewPanel::UnselectDomNode()
    {
        m_SelectedNode = nullptr;
    }

    void DomViewPanel::DrawNode(html::DomTreeNode& node) 
    {
        ImGuiTreeNodeFlags flags = 
            ((m_SelectedNode == &node) ? ImGuiTreeNodeFlags_Selected : 0) | 
            // Add me: If node has no children use ImGuiTreeNodeFlags_Leaf
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_SpanAvailWidth;

        // TODO: Probably not a good idea to use 
        // the address of the node as the ID. Change me later
        bool opened = ImGui::TreeNodeEx((void*)& node, flags, html::TagNames[(size_t)node.Type()]);
        if (ImGui::IsItemClicked())
        {
            m_SelectedNode = &node;
        }

        if (opened)
        {
            for (auto& child : node)
            {
                if (child->Type() != html::Tag::LAST)
                    DrawNode(*(child.get()));
            }
            ImGui::TreePop();
        }

        
    }

    void DomViewPanel::DrawNodeProperties(html::DomTreeNode& node)
    {
        ImGui::Begin("Dom Properties");

        ImGui::Text("Node Type: %s", html::TagNames[(size_t)node.Type()]);
        ImGui::Text("Inner Text: %s", node.GetInnerText().c_str());
        ImGui::Text("Attributes:");

        for (const auto& attrib : node.GetAttributes())
        {
            ImGui::Text("\t%s : %s", attrib.first.c_str(), attrib.second.c_str());
        }

        ImGui::End(); // End Dom Properties
    }

}
