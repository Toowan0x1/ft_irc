/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:27:20 by oel-houm          #+#    #+#             */
/*   Updated: 2024/01/18 13:27:21 by oel-houm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Channel.hpp"

Channel::Channel() {
    /**/
}

Channel::Channel(std::string channel) {
    /**/ (void)channel;
}

Channel::~Channel() {
    /**/
}

/* -- -- --  C H A N N E L  -- -- --  

Channel::Channel(){
    //
}

Channel::Channel(std::string s, std::string topic) : channelName(s), channelTopic(topic)
{
    std::cout << this->channelName + " has been created!" << std::endl;
}

Channel::Channel(std::string s, std::string topic, std::string pswd) : channelName(s), channelTopic(topic)
{
    if (pswd != ""){
        this->keyMode = true;
        this->setTheKey(pswd);
    }
    else
        this->keyMode = false;
        this->limitMode = false;
        this->topicProtectMode = false;
        this->inviteMode = false;
    std::cout << this->channelName + " has been created!" << std::endl;
}





Channel::~Channel(){
    if (!this->channelOps.empty())
        this->channelOps.clear();
    if (!this->channelMembers.empty())
        this->channelMembers.clear();
    std::cout << "The Channel : " + channelName + " is unusable anymore!" << std::endl;
}

*/