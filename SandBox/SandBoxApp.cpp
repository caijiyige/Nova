#include "Nova.h"

class SandBoxApp : public NV::Application
{
private:
    /* data */
public:
    SandBoxApp ()
    {
        // Initialize the application
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
