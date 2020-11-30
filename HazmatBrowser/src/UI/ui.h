#pragma once

#include <imgui.h>

namespace hazmat::ui {

    struct ViewportOutput {
        bool isFocused;
        bool isHovered;
        ImVec2 size;
    };

    template <typename UICallback>
    void Button(const char* label, const char* tooltipText, UICallback callback)
    {
        if (ImGui::Button(label))
        {
            callback();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip(tooltipText);
    }

    template <typename UICallback>
    void ArrowButton(const char* label, const char* tooltipText, ImGuiDir direction, UICallback callback)
    {
        if (ImGui::ArrowButton(label, direction))
        {
            callback();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip(tooltipText);
    }

    template <typename UICallback>
    void RefreshButton(UICallback callback)
    {
        Button("Refresh", "Reload this page", callback);
    }

    template <typename UICallback>
    void HomeButton(UICallback callback)
    {
        Button("Home", "Load the default page", callback);
    }

    template <typename UICallback>
    void BackButton(UICallback callback)
    {
        ArrowButton("Back", "Go back one page", ImGuiDir_Left, callback);
    }

    template <typename UICallback>
    void ForwardButton(UICallback callback)
    {
        ArrowButton("Forward", "Go forward one page", ImGuiDir_Right, callback);
    }


    ViewportOutput viewport(uint64_t framebufferId)
    {
        //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        //ImGui::Begin("Viewport");

        ViewportOutput output;

        output.isFocused = ImGui::IsWindowFocused();
        output.isHovered = ImGui::IsWindowHovered();
        Hazel::Application::Get().GetImGuiLayer()->BlockEvents(!output.isFocused || !output.isHovered);

        output.size = ImGui::GetContentRegionAvail();
        
        ImGui::Image(reinterpret_cast<void*>(framebufferId), output.size, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        //ImGui::End();
        //ImGui::PopStyleVar();

        return output;
    }


}