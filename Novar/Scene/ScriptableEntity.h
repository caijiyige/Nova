#include "Novar/Scene/Entity.h"

namespace NV
{
    class ScriptableEntity
    {
        public:
            /* data */
            std::string Name;
            std::string FilePath;
            ScriptableEntity() = default;
            
            ScriptableEntity(const ScriptableEntity&)= default;
            
            ScriptableEntity(const std::string& name, const std::string& filePath)
                : Name(name), FilePath(filePath), m_Entity(nullptr, 0) {}
 
            operator const std::string&() const { return Name; }
            operator std::string() { return Name; }
            
            private:
            Entity m_Entity;
            
    };
}           