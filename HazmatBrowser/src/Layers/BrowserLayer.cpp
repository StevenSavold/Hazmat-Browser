#include "BrowserLayer.h"
#include "HTML/html.h"

#include "UI/ui.h"
#include <misc/cpp/imgui_stdlib.h>
#include <iostream>

namespace hazmat {


    BrowserLayer::BrowserLayer()
        : Layer("BrowserLayer")
    {
    }


    void BrowserLayer::OnAttach()
    {
        // Open a new tab by default
        NewTab();
    }


    void BrowserLayer::OnDetach() {}


    void BrowserLayer::OnUpdate(Hazel::Timestep ts) 
    {
        HZ_PROFILE_FUNCTION();

        CleanupClosedTabs();

        // If the active tab is null, there is no reason to update
        if (!m_ActiveTab)
        {
            HZ_WARN("Skipping Update, Active Tab is NULL!");
            return;
        }

        // update the tab
        m_ActiveTab->OnUpdate(ts);

        // Update
        //if (m_ViewportFocused)
        //    m_CameraController.OnUpdate(ts);

        // Render
        Hazel::Renderer2D::ResetStats();
        m_ActiveTab->m_Framebuffer->Bind();
        Hazel::RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
        Hazel::RenderCommand::Clear();

        // Update scene
        //m_ActiveScene->OnUpdate(ts);

        m_ActiveTab->m_Framebuffer->Unbind();
    }


    void BrowserLayer::OnImGuiRender()
    {
        HZ_PROFILE_FUNCTION();

        // Note: Switch this to true to enable dockspace
        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = /*ImGuiWindowFlags_MenuBar |*/ ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("DockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;


        ImGui::Begin("Browser Stuff");

        ImGuiTabBarFlags tab_bar_flags = /*ImGuiTabBarFlags_AutoSelectNewTabs |*/ ImGuiTabBarFlags_Reorderable;
        if (ImGui::BeginTabBar("BrowserTabBar", tab_bar_flags))
        {
            for (int i = 0; i < m_Tabs.size(); ++i)
            {
                auto& tab = m_Tabs[i];

                ImGuiTabItemFlags flags = ImGuiTabItemFlags_None;
                if (ImGui::BeginTabItem(tab->getUniqueTitle().c_str(), &(tab->m_IsOpened), flags))
                {
                    m_ActiveTab = tab.get(); // Store the active tab for updating later

                    ui::BackButton([&]() {
                        m_ActiveTab->Back();
                        m_DomViewPanel.UnselectDomNode();
                    });
                    ImGui::SameLine();

                    ui::ForwardButton([&]() {
                        m_ActiveTab->Forward();
                        m_DomViewPanel.UnselectDomNode();
                    });
                    ImGui::SameLine();

                    ui::RefreshButton([&]() {
                        m_ActiveTab->Refresh();
                        m_DomViewPanel.UnselectDomNode();
                    });
                    ImGui::SameLine();

                    ui::HomeButton([&]() {
                        m_ActiveTab->Home();
                        m_DomViewPanel.UnselectDomNode();
                    });
                    ImGui::SameLine();

                    if (ImGui::InputTextWithHint(" ", "Enter a URL", &(tab->m_UrlText), 0, [](ImGuiInputTextCallbackData * data) -> int {
                        HZ_INFO("URL Callback");
                        return 1;
                    }))
                    {
                        HZ_INFO("URL Text!!");
                    }


                    // The viewport for the tab
                    auto vpOut = ui::viewport(tab->m_Framebuffer->GetColorAttachmentRendererID());
                    tab->m_ViewportIsFocused = vpOut.isFocused;
                    tab->m_ViewportIsHovered = vpOut.isHovered;
                    tab->m_ViewportSize = { vpOut.size.x, vpOut.size.y };

                    ImGui::EndTabItem();
                }
            }

            ImGui::EndTabBar();
        }

        ImGui::End(); // End Browser Stuff

        ImGui::Begin("Debugging");

        ImGui::Text("Browser Stats:");
        ImGui::Text("\tFrames per Second: %.3f", ImGui::GetIO().Framerate);
        ImGui::Text("\tMilliseconds per Frame: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);

        ImGui::NewLine();

        ImGui::Text("Active Tab Properties:");
        ImGui::Text("\tTitle: %s", (m_ActiveTab) ? m_ActiveTab->getTitle().c_str() : "Null");
        ImGui::Text("\tUri: %s", (m_ActiveTab) ? m_ActiveTab->getUri().c_str() : "Null");
        ImGui::Text("\tUrl Bar Text: %s", (m_ActiveTab) ? m_ActiveTab->m_UrlText.c_str() : "Null");
        ImGui::Text("\tIs Opened: %s", (m_ActiveTab) ? ((m_ActiveTab->m_IsOpened) ? "true" : "false") : "Null");
        ImGui::Text("\tIs Viewport Hovered: %s", (m_ActiveTab) ? ((m_ActiveTab->m_ViewportIsHovered) ? "true" : "false") : "Null");
        ImGui::Text("\tIs Viewport Focused: %s", (m_ActiveTab) ? ((m_ActiveTab->m_ViewportIsFocused) ? "true" : "false") : "Null");
        ImGui::Text("\tViewport Width: %f", (m_ActiveTab) ? m_ActiveTab->m_ViewportSize.x : -1.0f);
        ImGui::Text("\tViewport Height: %f", (m_ActiveTab) ? m_ActiveTab->m_ViewportSize.y : -1.0f);

        ImGui::NewLine();

        ImGui::Text("URI Properties:");
        ImGui::Text("\tUriText: %s", (m_ActiveTab) ? m_ActiveTab->m_Uri.UriText.c_str() : "Null");
        ImGui::Text("\tProtocol: %s", (m_ActiveTab) ? m_ActiveTab->m_Uri.Protocol.c_str() : "Null");
        ImGui::Text("\tHost: %s", (m_ActiveTab) ? m_ActiveTab->m_Uri.Host.c_str() : "Null");
        ImGui::Text("\tPort: %s", (m_ActiveTab) ? m_ActiveTab->m_Uri.Port.c_str() : "Null");
        ImGui::Text("\tPath: %s", (m_ActiveTab) ? m_ActiveTab->m_Uri.Path.c_str() : "Null");
        ImGui::Text("\tQuery: %s", (m_ActiveTab) ? m_ActiveTab->m_Uri.QueryString.c_str() : "Null");


        ImGui::NewLine();

        ImGui::Text("All Tabs:");
        for (auto& tab : m_Tabs)
        {
            ImGui::Text("\t%s", tab->getTitle().c_str());
            ImGui::SameLine();
            ImGui::Checkbox("opened", &(tab->m_IsOpened));
        }


        ImGui::End(); // End Debugging

        // As long as active tab is not null, draw the Html and Dom windows
        if (m_ActiveTab)
        {
            ImGui::Begin("Raw Html");

            ImVec2 region = ImGui::GetContentRegionAvail();
            ImGui::InputTextMultiline(
                "Raw Html",
                &(m_ActiveTab->m_PageContentString),
                region
            );

            ImGui::End(); // End Raw Html

            // Draw the Dom View Panel
            m_DomViewPanel.Draw(*(m_ActiveTab->m_DomTreeRoot.get()));
        }

        ImGui::End(); // End Dockspace
    }


    void BrowserLayer::OnEvent(Hazel::Event& e) 
    {
        Hazel::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(BrowserLayer::OnKeyPressed));
    }


    bool BrowserLayer::OnKeyPressed(Hazel::KeyPressedEvent& e)
    {
        // Shortcuts
        if (e.GetRepeatCount() > 0)
            return false;

        bool control = Hazel::Input::IsKeyPressed(Hazel::Key::LeftControl) || Hazel::Input::IsKeyPressed(Hazel::Key::RightControl);
        bool shift = Hazel::Input::IsKeyPressed(Hazel::Key::LeftShift) || Hazel::Input::IsKeyPressed(Hazel::Key::RightShift);
        switch (e.GetKeyCode())
        {
        case Hazel::Key::N:
            // If Ctrl+N is pressed, create a new tab
            if (control)
                NewTab();

            break;

        case Hazel::Key::Enter:
            // If the Enter key is pressed, reload the Active Tabs Uri
            if (m_ActiveTab)
                m_ActiveTab->ReloadUri();

            break;
        }

        return false;
    }

    void BrowserLayer::NewTab()
    {
        static long long counter = 0;

        // TODO: This is a janky way to fix tabs needing unique names
        std::string title = "New Tab";
        //title += std::to_string(counter++);

        // push a new default tab to the browser tab bar
        m_Tabs.push_back(std::make_unique<Tab>(title, ""));
    }

    void BrowserLayer::CleanupClosedTabs()
    {
        // TODO: figure out a better way to handle tabs
        // right now cleaning up old tabs is HARD
    }  

}