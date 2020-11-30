#include "Tab.h"
#include <filesystem>
#include "Io/FileUtils.h"
#include "Html/HtmlTags.h"


namespace hazmat {

    Tab::Tab(const std::string& title, const std::string& url, bool isOpened /* = true */)
        : m_Title(title),
        // Probably not a good idea to use memory adddress as uid
        m_UniqueTitle(title + "##" + std::to_string((uint64_t)this)),
        m_UrlText(url),
        m_IsOpened(isOpened)
    {
        // Setup FrameBuffer
        Hazel::FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Hazel::Framebuffer::Create(fbSpec);


        // If url is NOT an empty string, go fetch webpage from url
        // TODO
        ReloadUri();
        Refresh();
    }

    void Tab::OnUpdate(Hazel::Timestep ts)
    {
        // Resize
        if (Hazel::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            //m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
            //m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }
    }


    void Tab::ReloadUri()
    {
        // Parse the new Uri from the url bar
        io::Uri newUri{ m_UrlText };

        // error checking??

        // Commit the newly vetted Uri as this tabs new Uri
        m_Uri = newUri;
    }

    void Tab::Back()
    {
        HZ_INFO("Back");
    }

    void Tab::Forward()
    {
        HZ_INFO("Forward");
    }

    void Tab::Home()
    {
        std::string defaultPagePath = std::string("file://") +
            std::filesystem::current_path().generic_string() +
            "/assets/webpages/default.html";

        m_UrlText = defaultPagePath;

        ReloadUri();
        Refresh();
    }

    void Tab::Refresh()
    {
        FetchPage();

        HZ_INFO("page content: \n{}", m_PageContentString.c_str());

        m_DomTreeRoot = html::CreateDomTree(m_PageContentString);
        auto titleNode = m_DomTreeRoot->Find(html::Tag::TITLE);
        if (titleNode)
        {
            m_Title = titleNode->GetInnerText();
            m_UniqueTitle = m_Title + "##" + std::to_string((uint64_t)this);
        }

    }



    void Tab::FetchPage()
    {
        auto it = io::Uri::ProtocolMap.find(m_Uri.Protocol);

        if (it == io::Uri::ProtocolMap.end())
        {
            HZ_ASSERT(false, "Unsupported protocol!");
            return;
        }

        switch (it->second)
        {
        case io::Uri::Protocols::file:
            m_PageContentString = io::FetchFile(m_Uri.Host + ":" + m_Uri.Path);
            break;
        case io::Uri::Protocols::http:
        case io::Uri::Protocols::https:
            // TODO
            break;
        default:
            HZ_ASSERT(false, "Unsupported protocol!");
            break;
        }
    }

}