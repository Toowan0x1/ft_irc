// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   boot.cpp                                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: hoakoumi <hoakoumi@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/03/22 14:23:49 by hoakoumi          #+#    #+#             */
// /*   Updated: 2024/03/24 04:02:23 by oel-houm         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "ft_irc/Include/Server.hpp"
#include "ft_irc/Include/Client.hpp"
#include "ft_irc/Include/Channel.hpp"


// static void    sendMsg(int fd, std::string msg)
// {
//     const char *_msg = msg.c_str();
//     size_t msgSize = strlen(_msg);
//     if (send(fd, _msg, msgSize, 0) < 0) {
//         std::cout << "send failed" << std::endl;
//     }
// }

void Server::bot(Client *client)
{
    (void)client;
    return;
}
//     std::string Greeting = "";
//     Greeting += "\n";
//     Greeting += " 💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙 \n";
//     Greeting += "                                         BOOT                                           \n";
//     Greeting += " 💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙 \n";
//     Greeting += " ······················································································\n";
//     Greeting += "  🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋\n";
//     Greeting += "  🦋 Available Commands:                                                                    🦋\n";
//     Greeting += "  🦋                                                                                        🦋\n";
//     Greeting += "  🦋    👢💨  KICK           :  Eject a client from the channel                             🦋\n";
//     Greeting += "  🦋                          👋 KICK <channel> <user> 👋                                   🦋\n";
//     Greeting += "  🦋                                                                                        🦋\n";
//     Greeting += "  🦋    💌💌  INVITE         :  Invite a client to a channel                                🦋\n";
//     Greeting += "  🦋                          👋 INVITE <nickname> <channel> 👋                             🦋\n";
//     Greeting += "  🦋                                                                                        🦋\n";
//     Greeting += "  🦋    📝📝  TOPIC          :  Change or view the channel topic                            🦋\n";
//     Greeting += "  🦋                          👋 TOPIC <channel> [ <topic> ] 👋                             🦋\n";
//     Greeting += "  🦋                                                                                        🦋\n";
//     Greeting += "  🦋    🚪🚪  JOIN           :  Joins the specified channel                                 🦋\n";
//     Greeting += "  🦋                          👋 JOIN <channel> <key> 👋                                    🦋\n";
//     Greeting += "  🦋                                                                                        🦋\n";
//     Greeting += "  🦋    📋📋  LIST           :  Lists currently available channels                          🦋\n";
//     Greeting += "  🦋                          👋 LIST <channel> <target> 👋                                 🦋\n";
//     Greeting += "  🦋                                                                                        🦋\n";
//     Greeting += "  🦋    💬💬  MSG            :  Sends a message                                             🦋\n";
//     Greeting += "  🦋                          👋 MSG <recipient> <message> 👋                               🦋\n";
//     Greeting += "  🦋                                                                                        🦋\n";
//     Greeting += "  🦋    🔐🔐  Pass           :  The PASS command is used to set a 'connection password'     🦋\n";
//     Greeting += "  🦋                          👋 PASS <password>  👋                                        🦋\n";
//     Greeting += "  🦋                                                                                        🦋\n";
//     Greeting += "  🦋    🛑🛑  Quit           :  Disconnects you from IRC                                    🦋\n";
//     Greeting += "  🦋                          👋 <Quit Message> 👋                                          🦋\n";
//     Greeting += "  🦋                                                                                        🦋\n";
//     Greeting += "  🦋    👤👤  User           :  used at the beginning of connection to specify              🦋\n";
//     Greeting += "  🦋                          the username, hostname and realname of a new user             🦋\n";
//     Greeting += "  🦋                          👋 <user> <mode> <unused> <realname>👋                        🦋\n";
//     Greeting += "  🦋                                                                                        🦋\n";
//     Greeting += "  🦋    🕵️‍♂️🕵️‍♂️  Whos           :  Display a list of users who are currently connected         🦋\n";
//     Greeting += "  🦋                          to the same channel or server as you                          🦋\n";
//     Greeting += "  🦋                          👋 who <channel> 👋                                           🦋\n";
//     Greeting += "  🦋                                                                                        🦋\n";
//     Greeting += "  🦋    🤔🤔  Whois          :  Retrieve information about a specific user                  🦋\n";
//     Greeting += "  🦋                          👋 whois <nickname> 👋                                        🦋\n";
//     Greeting += "  🦋                                                                                        🦋\n";
//     Greeting += "  🦋    🔧🔧  MODE           :  Change the channel's mode:                                  🦋\n";
//     Greeting += "  🦋                          👋 MODE <channel_name> <mode> 👋                              🦋\n";
//     Greeting += "  🦋                          🔒🔒 i : Set/remove Invite-only channel                       🦋\n";
//     Greeting += "  🦋                          📋🔒 t : Set/remove TOPIC restrictions to operators           🦋\n";
//     Greeting += "  🦋                          🔑🔑 k : Set/remove the channel key (password)                🦋\n";
//     Greeting += "  🦋                          👮‍♂️🔧 o: Give/take channel operator privilege                 🦋\n";
//     Greeting += "  ��🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋��🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋🦋\n";
//     Greeting += " 💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙💙 \n";

//     // send msg to new client
//     sendMsg(client->_clientFd, Greeting);
// }
