#include "EditerLayer.h"
#include "EntryPoint.h"

namespace NV
{
    class NVEditerApp : public Application
    {
    private:
        /* data */
    public:
        NVEditerApp ()
        : Application("Nova Editer")
        {
            
        // PushLayer(new ExampleLayer());

        PushLayer(new EditerLayer());
            
        }
        ~NVEditerApp ()
        {

        }
    };


    // To be defined in CLIENT
    NV::Application* CreateApplication()
    {
        return new NVEditerApp();
    }

}
