#include "NovaEditor/EditorLayer.h"
#include "Novar/Core/EntryPoint.h"

namespace NV
{
    class NVEditorApp : public Application
    {
    private:
        /* data */
    public:
        NVEditorApp ()
        : Application("Nova Editor")
        {
            
        // PushLayer(new ExampleLayer());

        PushLayer(new EditorLayer());
            
        }
        ~NVEditorApp ()
        {

        }
    };


    // To be defined in CLIENT
    NV::Application* CreateApplication()
    {
        return new NVEditorApp();
    }

}
