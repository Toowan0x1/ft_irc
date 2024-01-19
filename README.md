# IRC

Internet Relay Chat (IRC) is a text-based chat system for instant messaging. IRC is designed for group communication in discussion forums, called channels but also allows one-on-one communication via private messages as well as chat and data transfer, including file sharing.

Internet Relay Chat is implemented as an application layer protocol to facilitate communication in the form of text. The chat process works on a client–server networking model. Users connect, using a client—which may be a web app, a standalone desktop program, or embedded into part of a larger program—to an IRC server, which may be part of a larger IRC network. Examples of programs used to connect include Mibbit, IRCCloud, KiwiIRC, and mIRC.

<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/a/ae/Irssi_1.2.3_screenshot.png/800px-Irssi_1.2.3_screenshot.png">

## Usage
- Build the IRC server:  ```Make```
- Run the IRC server: ```./ircserv <SERVER_PORT> <SERVER_PASSWORD>```
- Connect from client. From CIRC: ```/server <SERVER_IP> <SERVER_PORT> <SERVER_PASSWORD>```

## Supported Commands
- /JOIN #channel_name
- /NICKNAME
- /MSG
- /PRIVMSG
- /LIST
- /WHO
- /WHOIS
- /KICK
- /INVITE
- /TOPIC
- /MODE
