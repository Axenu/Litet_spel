#include "Sound/Irrklang.h"

IrrKlang::IrrKlang()
{
	_engine = createIrrKlangDevice();

	if (!_engine)
		return;

	_timer = 0.0f;
}

IrrKlang::~IrrKlang()
{
	if (_engine)
	{
		_engine->drop();
		_engine = nullptr;
	}
}

void IrrKlang::PlaySource2DSound(ISoundSource* source, bool loop)
{
	_engine->play2D(source, loop);
}

void IrrKlang::PlaySource3DSound(ISoundSource* source, bool loop, glm::vec3 listenerPos, glm::vec3 origin, glm::vec3 lookDir, glm::vec3 up, float dt)
{
	origin = glm::vec3(origin - listenerPos);
 
	if (_timer < (source->getPlayLength() / 1000.f))
	{
		_engine->setListenerPosition(vec3df(0.0f, 0.0f, 0.0f), -vec3df(lookDir.x, lookDir.y, lookDir.z), vec3df(0.0f, 0.0f, 0.0f), vec3df(up.x, up.y, up.z));
	}
	else
	{
		_engine->play3D(source, vec3df(origin.x, origin.y, origin.z), loop, false, false, false);
		_timer = 0.0f;
	}

	_timer += dt;
}

void IrrKlang::PlaySource3DSound(ISoundSource* source, bool loop, glm::vec3 listenerPos, glm::vec3 origin, glm::vec3 lookDir, glm::vec3 up)
{
	origin = glm::vec3(origin - listenerPos);

	_engine->setListenerPosition(vec3df(0.0f, 0.0f, 0.0f), -vec3df(lookDir.x, lookDir.y, lookDir.z), vec3df(0.0f, 0.0f, 0.0f), vec3df(up.x, up.y, up.z));
	_engine->play3D(source, vec3df(origin.x, origin.y, origin.z), loop, false, false, false);
}

ISoundSource* IrrKlang::GetSound(char* filename)
{
	return _engine->getSoundSource(filename);
}
