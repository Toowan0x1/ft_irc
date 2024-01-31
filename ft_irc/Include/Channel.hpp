/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:19:59 by oel-houm          #+#    #+#             */
/*   Updated: 2024/01/17 11:22:09 by oel-houm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class   Channel {
    private:
        //std::string _name;
        //std::vector<Client *> _members;
    public:
        std::string _name;
        std::string _pass;
        std::vector<Client *> _members;
        Channel();
        Channel(std::string channel);
        ~Channel();
};

#endif
