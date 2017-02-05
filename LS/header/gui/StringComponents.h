#pragma once

#include <iostream>
#include <string>

#include "glm/gtx/string_cast.hpp"


namespace gui
{

    class StringComponent
    {
    public:
        StringComponent() {}
        virtual std::string getString() = 0;
    };

    class StringComponentString : public StringComponent
    {
    private:
        std::string _value;
    public:
        StringComponentString(std::string value);
        std::string getString();
    };

    class StringComponentFloat : public StringComponent
    {
    private:
        float* _value;
    public:
        StringComponentFloat(float* value);
        std::string getString();
    };

    class StringComponentInt : public StringComponent
    {
    private:
        int* _value;
    public:
        StringComponentInt(int* value);
        std::string getString();
    };

    class StringComponentVec3 : public StringComponent
    {
    private:
        glm::vec3* _value;
    public:
        StringComponentVec3(glm::vec3* value);
        std::string getString();
    };

}
