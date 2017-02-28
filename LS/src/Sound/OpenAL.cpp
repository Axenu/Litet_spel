#include "Sound/OpenAL.h"

OpenAL::OpenAL()
{
	//Initiate OpenAL
	alutInit(0, NULL);

	//Capture errors
	alGetError();

	//Load pcm data into buffer
	_buffer = alutCreateBufferFromFile(FILENAME);

	//Create sound (use buffer to fill source)
	alGenSources(1, &_source);
	alSourcei(_source, AL_BUFFER, _buffer);
}

OpenAL::~OpenAL()
{

}

void OpenAL::PlaySource()
{

}
