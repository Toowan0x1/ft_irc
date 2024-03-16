# IRC

(International standard RFC 2812)

Internet Relay Chat (IRC) is a text-based chat system for instant messaging. IRC is designed for group communication in discussion forums, called channels but also allows one-on-one communication via private messages as well as chat and data transfer, including file sharing.

Internet Relay Chat is implemented as an application layer protocol to facilitate communication in the form of text. The chat process works on a client–server networking model. Users connect, using a client—which may be a web app, a standalone desktop program, or embedded into part of a larger program—to an IRC server, which may be part of a larger IRC network. Examples of programs used to connect include Mibbit, IRCCloud, KiwiIRC, and mIRC.

<!-- <img src="https://media.forgecdn.net/attachments/33/680/ircbridgescreenshot.png"> -->
<!-- <img src="https://raw.githubusercontent.com/Toowan0x1/ft_irc/master/Assets/chatserver.png"> -->
<!-- <img src="https://i.redd.it/nsodxt2h967a1.jpg"> -->

## Mandatory Part

| Description | An IRC server in C++ 98 |
| ------ | ------ |
| Program name | ircserv |
| Arguments | port: The listening port, password: The connection password |
| External functs | Everything in C++ 98. socket, close, setsockopt, getsockname, getprotobyname, gethostbyname, getaddrinfo, freeaddrinfo, bind, connect, listen, accept, htons, htonl, ntohs, ntohl, inet_addr, inet_ntoa, send, recv, signal, lseek, fstat, fcntl, poll (or equivalent) |
| Libft authorized | n/a |

### User Modes to be implemented:
  - **`i` (invisible):** Makes the user invisible to others who query the server for a list of users.
  - **`o` (operator):** Grants administrative privileges to the user, allowing them to perform certain administrative actions on the network.
  - **`w` (wallops):** Allows the user to receive server WALLOPS messages, which are typically used for important network-wide announcements.
  - **`a` (away):** Marks the user as being away from the keyboard or not actively participating in the conversation.
  - **`r` (restricted):** Limits the user's ability to receive messages from channels they haven't joined.

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

## Popular IRC CLients

IRC (Internet Relay Chat) clients are applications that facilitate communication on the IRC network, allowing users to connect, chat, and participate in discussions with other users around the world. Whether you prefer a command-line interface or a graphical user interface, there are various IRC clients to suit your preferences.

- [irssi](https://irssi.org/download/)
- [weechat](https://weechat.org/)
- [xchat](http://xchat.org/)
- [hexchat](https://hexchat.github.io/downloads.html)
- [konversation](https://konversation.kde.org/)

<img src="https://raw.githubusercontent.com/Toowan0x1/ft_irc/master/Assets/Screenshot%20from%202024-01-30%2006-26-25.png">

#### Choosing an IRC Client
- Selecting an IRC client depends on your preferences and requirements. Whether you favor a command-line environment, a user-friendly GUI, or specific features, there's an IRC client suitable for you. Explore the options and find the client that aligns with your communication style and workflow.

#### Important Note
- Our IRC program has been tested and is fully compatible with the HexChat IRC client. HexChat is used as our reference client during development and testing to ensure seamless compatibility and optimal performance.

## Lessons
- Socket programming & multiplixing
- poll error monitor and handling
- event-driven programming

## Supported Commands
- [x] USER
- [x] NICK
- [x] PASS
- [x] JOIN
- [x] WHO
- [x] WHOIS
- [x] LIST
- [x] MODE
- [x] KICK
- [x] TOPIC
- [x] MSG
- [ ] Leave
- [ ] INVITE

## User Modes
- https://libera.chat/guides/usermodes

## Remarque :
This project is under building, and not finished yet.
