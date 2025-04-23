#include "Sandbox2D.h"
#include "EntryPoint.h"

class SandBoxApp : public NV::Application
{
private:
    /* data */
public:
    SandBoxApp ()
    {
        
       // PushLayer(new ExampleLayer());

       PushLayer(new Sandbox2D());
        
    }
    ~SandBoxApp ()
    {

    }
};


// To be defined in CLIENT
NV::Application* NV::CreateApplication()
{
    return new SandBoxApp();
}
