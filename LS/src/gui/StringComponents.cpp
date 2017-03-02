#include "gui/StringComponents.h"

namespace gui
{
    StringComponentString::StringComponentString(std::string value)
    {
        _value = value;
    }
    std::string StringComponentString::getString()
    {
        return _value;
    }

    StringComponentFloat::StringComponentFloat(float* value)
    {
        _value = value;
    }
    std::string StringComponentFloat::getString()
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(_precision) << *_value;
        return stream.str();
    }
    StringComponentFloatConst::StringComponentFloatConst(float value, int precision)
    {
        _value = value;
        _precision = precision;
    }
    std::string StringComponentFloatConst::getString()
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(_precision) << _value;
        return stream.str();
    }

    StringComponentInt::StringComponentInt(int* value)
    {
        _value = value;
    }
    std::string StringComponentInt::getString()
    {
        return std::to_string(*_value);
    }

    StringComponentVec3::StringComponentVec3(glm::vec3* value)
    {
        _value = value;
    }
    std::string StringComponentVec3::getString()
    {
        return "vec3(x: " + std::to_string(_value->x) + ", y: " + std::to_string(_value->y) + ", z: " + std::to_string(_value->z) + ")";
    }
}
