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

int Channel::getNext(float elapsedTime, unsigned int chInd) const
{
	for (unsigned int i = 0; i < _channelKeys[chInd].size(); i++)
	{
		if (elapsedTime < _channelKeys[chInd][i]._time)
			return i;
	}
	return -1;
}
int Channel::getPrevious(float elapsedTime, unsigned int chInd) const
{
	for (unsigned int i = 0; i < _channelKeys[chInd].size(); i++)
	{
		if (elapsedTime < _channelKeys[chInd][i]._time)
			return i > 0 ? i - 1 : 0; //If no key before the time return the first key.
	}
	return -1;
}

/* Get a specific key from a channel */
ChannelKey Channel::getKey(unsigned int index, unsigned int chInd) const {
	return _channelKeys[chInd][index];
}


unsigned int Channel::numKeys(unsigned int chInd) const {
	return chInd < _channelKeys.size() ? _channelKeys[chInd].size() : 0;
}
unsigned int Channel::numNodeChannels() const {
	return _channelKeys.size();
}