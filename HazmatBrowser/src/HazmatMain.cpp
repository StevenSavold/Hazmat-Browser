#include <iostream>

#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Layers/BrowserLayer.h"

namespace hazmat {
    class HazmatBrowser : public Hazel::Application
    {
    public:
        HazmatBrowser()
            : Application("Hazmat")
        {
            // Disable vsync (for now)
            GetWindow().SetVSync(false);
            
            PushLayer(new BrowserLayer());
        }

        ~HazmatBrowser() = default;

    private:

    };
}


namespace Hazel {
    
    Application* CreateApplication()
    {
        return new hazmat::HazmatBrowser{};
    }

}