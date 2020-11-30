#pragma once
#include <string>
#include <glm/glm.hpp>
#include <Hazel.h>

#include "Io/Uri.h"
#include "Html/DomTree.h"

namespace hazmat {

    class Tab
    {
        friend class BrowserLayer;

    public:
        Tab(const std::string& title, const std::string& url, bool isOpened = true);
        ~Tab() = default;

        inline const std::string& getTitle() const { return m_Title; }
        inline const std::string& getUniqueTitle() const { return m_UniqueTitle; }
        inline const std::string& getUri() const { return m_Uri.UriText; }

        void ReloadUri();

        void Back();
        void Forward();
        void Home();
        void Refresh();

        void OnUpdate(Hazel::Timestep ts);

        void FetchPage();

    public:
        bool m_IsOpened;
        std::string m_UrlText;

        // Graphics Data
        Hazel::Ref<Hazel::Framebuffer> m_Framebuffer;
        bool m_ViewportIsHovered = false;
        bool m_ViewportIsFocused = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

    private:
        io::Uri m_Uri;
        std::unique_ptr<html::DomTreeNode> m_DomTreeRoot;
        std::string m_PageContentString;
        std::string m_Title;
        std::string m_UniqueTitle;
    };

}