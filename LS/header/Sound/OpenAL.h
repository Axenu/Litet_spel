#pragma once

#include <conio.h>
#include <stdlib.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alu.h>
#include <AL/alut.h>

#define FILENAME "simple.wav"

class OpenAL
{
public:
	OpenAL();
	~OpenAL();
	void PlaySource();

private:
	ALuint _buffer, _source, _state;
};