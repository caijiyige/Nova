#pragma once

#include <string>

namespace NV
{
    class Texture
    {
        public :
        virtual ~Texture() = default;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        virtual void Bind(unsigned int slot = 0) const = 0;
        virtual void UnBind() const = 0;

    };


    class  Texture2D :public Texture
    {
    public:
        static std::shared_ptr<Texture2D> Create(const std::string& path);

    };
    

    
}