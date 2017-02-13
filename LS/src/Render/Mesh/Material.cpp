#include "Render/Mesh/Material.h"
#include "Render/Mesh/Shader/MeshShader.h"

Material::Material(MeshShader *shader)
	: _shader(shader)
{
	if (_shader)
		link();
}

Material::~Material()
{

}

/* Bind shader and assign related uniforms
*/
void Material::assignUniforms(RenderInfo &fD, const glm::mat4 &modelMatrix) const {
	_shader->assignUniforms(fD, modelMatrix, _shaderLink.get());
}

void Material::link() {
	_shaderLink = _shader->linkMaterial(*this);
}

bool Material::getValue(const std::string &id, Material::Value *&ref) {

	//We check if the value already exists:
	std::map<std::string, Value>::iterator it = _data.find(id);
	if (it != _data.end()) { //Check that value existed
		ref = &it->second;
		return true;
	}
	return false;
}

void Material::setValue(const std::string &id, Value &value) {
	_data[id] = std::move(value);
	link(); //Re-link the material
}

bool Material::tryGet(const std::string &id, float &value) {
	Material::Value *val = nullptr;
	if (getValue(id, val) && val->_size == 1) {
		value = val->_value.get()[0];
		return true;
	}
	return false;
}
bool Material::tryGet(const std::string &id, glm::vec2 &vec) {
	Material::Value *val = nullptr;
	if (getValue(id, val) && val->_size == 4) {
		vec = *reinterpret_cast<glm::vec4*>(val->_value.get());
		return true;
	}
	return false;
}
bool Material::tryGet(const std::string &id, glm::vec3 &vec) {
	Material::Value *val = nullptr;
	if (getValue(id, val) && val->_size == 3) {
		vec = *reinterpret_cast<glm::vec3*>(val->_value.get());
		return true;
	}
	return false;
}
bool Material::tryGet(const std::string &id, glm::vec4 &vec) {
	Material::Value *val = nullptr;
	if (getValue(id, val) && val->_size == 4) {
		vec = *reinterpret_cast<glm::vec4*>(val->_value.get());
		return true;
	}
	return false;
}
void Material::setFloat(const std::string &id, float value) {
	float *val = new float[1];
	val[0] = value;
	//avoid this:
	// setValue(id, Value(val, 1));
	//do this instead:
	Value tempVal(val, 1);
	setValue(id, tempVal);
}
void Material::setVec2(const std::string &id, const glm::vec2 &value) {
	float *val = new float[2];
	val[0] = value.x;
	val[1] = value.y;
	//avoid this:
	// setValue(id, Value(val, 2));
	//do this instead:
	Value tempVal(val, 2);
	setValue(id, tempVal);
}
void Material::setVec3(const std::string &id, const glm::vec3 &value) {
	float *val = new float[3];
	val[0] = value.x;
	val[1] = value.y;
	val[2] = value.z;
	//avoid this:
	// setValue(id, Value(val, 3));
	//do this instead:
	Value tempVal(val, 3);
	setValue(id, tempVal);
}
void Material::setVec4(const std::string &id, const glm::vec4 &value) {
	float *val = new float[4];
	val[0] = value.x;
	val[1] = value.y;
	val[2] = value.z;
	val[3] = value.w;
	//avoid this:
	// setValue(id, Value(val, 4));
	//do this instead:
	Value tempVal(val, 4);
	setValue(id, tempVal);
}
void Material::setColor(const std::string &id, const glm::vec4 &color) {
	setVec4(id, color);
}

MeshShader * Material::getpShader()
{
	return _shader;
}
