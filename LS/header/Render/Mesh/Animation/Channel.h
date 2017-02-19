#pragma once
#include <memory>
#include <vector>
#include "ChannelKeys.h"
/* An animated channel, holding the animated keys.
*/
class Channel
{
private:
	/* The set of different keys included in the channel.
	 * Each type holding the set of animated keys over the duration of the animation.
	 */
	std::vector<std::vector<ChannelKey>> _channelKeys;
public:
	Channel();
	Channel(std::vector<std::vector<ChannelKey>> &channelKeys);
	~Channel();
	/* Get next keyframe for the specific animation time. If there is no more -1 is returned
	*/
	int getNext(float elapsedTime, unsigned int chType) const;
	/* Get a specific key from a channel */
	ChannelKey getKey(unsigned int index, unsigned int chType) const;

};

