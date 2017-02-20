/* Mattias F 2017*/
#include "Render/Mesh/Animation/Channel.h"

Channel::Channel()
{
}
Channel::Channel(std::vector<std::vector<ChannelKey>> &channelKeys) 
	: _channelKeys(std::move(channelKeys))
{
}


Channel::~Channel()
{
}

int Channel::getNext(float elapsedTime, unsigned int chType) const
{
	for (unsigned int i = 0; i < _channelKeys[chType].size(); i++)
	{
		if (elapsedTime < _channelKeys[chType][i]._time)
			return i;
	}
	return -1;
}
/* Get a specific key from a channel */
ChannelKey Channel::getKey(unsigned int index, unsigned int chType) const {
	return _channelKeys[chType][index];
}