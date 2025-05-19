#include "NovaEditor/EditorLayer.h"
#include "Novar/Core/EntryPoint.h"

namespace NV
{
    class NVEditorApp : public Application
    {
    private:
        /* data */
    public:
        NVEditorApp (const ApplicationSpecification& spec)
        : Application(spec)
        {
            
        // PushLayer(new ExampleLayer());

        PushLayer(new EditorLayer());
            
        }
        ~NVEditorApp ()
        {

        }
    };


    // To be defined in CLIENT

    Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Nova engine";
		spec.CommandLineArgs = args;

		return new NVEditorApp(spec);
	}

}
