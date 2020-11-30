#pragma once

#include <Hazel.h>
#include <glm/glm.hpp>
#include <memory>

#include "Browser/Tab.h"
#include "UI/DomViewPanel.h"




namespace hazmat {
    class BrowserLayer : public Hazel::Layer
    {
    public:
        BrowserLayer();
        virtual ~BrowserLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void OnUpdate(Hazel::Timestep ts) override;
        virtual void OnImGuiRender() override;
        void OnEvent(Hazel::Event& e) override;

    private:
        bool OnKeyPressed(Hazel::KeyPressedEvent& e);

        void NewTab();
        void CleanupClosedTabs();

    private:
        std::vector<std::unique_ptr<Tab>> m_Tabs;
        Tab* m_ActiveTab = nullptr;
        ui::DomViewPanel m_DomViewPanel;

    };
}
