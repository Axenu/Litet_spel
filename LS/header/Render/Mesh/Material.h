#pragma once

#include <gl/glInclude.h>
#include"MaterialLink.h"
#include"../RenderInfo.h"
#include<map>

/* Forward declare
*/
class MeshShader;

class Material
{
public:

	Material(MeshShader *shader);
	virtual ~Material();
	virtual void SetUp() {};



	/* Bind shader and assign related uniforms
	*/
	virtual void assignUniforms(RenderInfo &fD, const glm::mat4 &modelMatrix) const;

	bool tryGet(const std::string &id, float &value);
	bool tryGet(const std::string &id, glm::vec2 &vec);
	bool tryGet(const std::string &id, glm::vec3 &vec);
	bool tryGet(const std::string &id, glm::vec4 &vec);

	void setFloat(const std::string &id, const float value);
	void setVec2(const std::string &id, const glm::vec2 &value);
	void setVec3(const std::string &id, const glm::vec3 &value);
	void setVec4(const std::string &id, const glm::vec4 &value);
	void setColor(const std::string &id, const glm::vec4 &color);

	MeshShader* getpShader();
protected:
	/* Data management
	*/
	struct Value {
		int _size;
		std::unique_ptr<float> _value;
		Value(){}
		Value(float *value, int size) : _size(size), _value(value) {}
		Value(const Value &copy) : _size(copy._size), _value(new float[_size])
		{
			std::memcpy(_value.get(), copy._value.get(), sizeof(float) * _size);
		}
		Value(Value &&copy) : _size(copy._size), _value(std::move(copy._value))
		{		}
		Value& operator=(Value&& val)
		{
			if (this == &val)
				return *this;
			_value = std::move(val._value);
			_size = val._size;
			return *this;
		}
	};
	/* Pointer reference to the shader
	*/
	MeshShader* _shader;
	bool getValue(const std::string &id, Value *&ref);
	void setValue(const std::string &id, Value &value);
private:
	/* Data used by the shader.
	*/
	std::shared_ptr<MaterialLink> _shaderLink;
	std::map<std::string, Value> _data;
	/* Links material data with the shader
	*/
	void link();

};