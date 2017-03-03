#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#include "glm/gtx/string_cast.hpp"


namespace gui
{

    class StringComponent
    {
    public:
		StringComponent() {};
		virtual ~StringComponent() {};
        virtual std::string getString() = 0;
    };

    class StringComponentString : public StringComponent
    {
    private:
        std::string _value;
    public:
        StringComponentString(std::string value);
		virtual ~StringComponentString() {};
        std::string getString();
    };

    class StringComponentFloat : public StringComponent
    {
    private:
        float* _value;
    public:
        int _precision = 3;

        StringComponentFloat(float* value);
		virtual ~StringComponentFloat() {};
        std::string getString();
    };
    class StringComponentFloatConst : public StringComponent
    {
    private:
        float _value;
        int _precision;
    public:
        StringComponentFloatConst(float value, int precision);
		virtual ~StringComponentFloatConst() {};
        std::string getString();
    };

    class StringComponentInt : public StringComponent
    {
    private:
        int* _value;
    public:
        StringComponentInt(int* value);
		virtual ~StringComponentInt() {};
        std::string getString();
    };

    class StringComponentVec3 : public StringComponent
    {
    private:
        glm::vec3* _value;
    public:
        StringComponentVec3(glm::vec3* value);
		virtual ~StringComponentVec3() {};
        std::string getString();
    };

}
