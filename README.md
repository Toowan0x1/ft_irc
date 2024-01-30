# IRC

(International standard RFC 2812)

Internet Relay Chat (IRC) is a text-based chat system for instant messaging. IRC is designed for group communication in discussion forums, called channels but also allows one-on-one communication via private messages as well as chat and data transfer, including file sharing.

Internet Relay Chat is implemented as an application layer protocol to facilitate communication in the form of text. The chat process works on a client–server networking model. Users connect, using a client—which may be a web app, a standalone desktop program, or embedded into part of a larger program—to an IRC server, which may be part of a larger IRC network. Examples of programs used to connect include Mibbit, IRCCloud, KiwiIRC, and mIRC.

<!-- <img src="https://www.uni-koeln.de/~pbogusze/pictures/2022-09-22-134408_958x520_scrot.png"> -->

## Mandatory Part

| Description | An IRC server in C++ 98 |
| ------ | ------ |
| Program name | ircserv |
| Arguments | port: The listening port, password: The connection password |
| External functs | Everything in C++ 98. socket, close, setsockopt, getsockname, getprotobyname, gethostbyname, getaddrinfo, freeaddrinfo, bind, connect, listen, accept, htons, htonl, ntohs, ntohl, inet_addr, inet_ntoa, send, recv, signal, lseek, fstat, fcntl, poll (or equivalent) |
| Libft authorized | n/a |

## Bonus Part

Some extra features to be added into IRC server so it looks even more like and actual IRC server:
- Handle file transfer.
- A bot

## Usage
- Build the IRC server:
  ```sh
  make
  ```
- Run the IRC server:
     ```sh
    ./ircserv <SERVER_PORT> <SERVER_PASSWORD>
    ```
- Connect from client. e.g. hexchat:
    ```sh
    /server <SERVER_IP> <SERVER_PORT> <SERVER_PASSWORD>
    ```
## Lessons
- Socket programming & multiplixing
- poll error monitor and handling
- event-driven programming

## IRC CLients
<img src="https://raw.githubusercontent.com/Toowan0x1/ft_irc/master/Screenshot%20from%202024-01-30%2006-26-25.png">
- irssi
- weechat
- hexchat
- xchat
- konversation

## Supported Commands
- [x] USER
- [x] NICK
- [x] PASS
- [x] JOIN
- [x] WHO
- [x] WHOIS
- [ ] LIST
- [ ] MSG
- [ ] PRIVMSG
- [ ] KICK
- [ ] INVITE
- [ ] TOPIC
- [ ] MODE
