<div align="center">
  <img height="200" src="https://raw.githubusercontent.com/Benjamin-poisson/My_image_bank/refs/heads/main/irc.png"  />
</div>

# ft_irc - Communication IRC

`ft_irc` is a custom implementation of an IRC (Internet Relay Chat) server written in C++.  
The goal of this project was to understand and implement network programming concepts such as sockets, communication protocols, and concurrency, while respecting the IRC specifications (RFC 1459 and related updates).  

## Status
<div align="center">
  <img height="200" src="https://raw.githubusercontent.com/Benjamin-poisson/My_image_bank/refs/heads/main/irc_success.png"  />
</div>

## Overview

Our server is compatible with IRC clients (we used **Konversation**) and allows multiple users to connect, join channels, and communicate in real time.

This project also includes bonus features to enhance usability and functionality.

---

## Features

### Mandatory
- Full parsing of IRC commands (RFC-compliant).
- Socket-based server handling multiple clients simultaneously.
- User and channel management with correct permission handling.
- Command support for:
  - `/nick`, `/user`, `/join`, `/part`, `/quit`
  - `/privmsg`
  - `/kick`, `/topic`, `/mode`
- Proper error handling and server responses.

### Bonus
- Support for **password-protected servers**.
- **Channel modes** for moderation (e.g., invite-only, topic restriction, user limits).
- **Private messaging** between users.
- **Multiple channel membership** for a single user.
- Improvements to parsing and stability for edge cases.

---

## Channel Bot
In addition to the server itself, we implemented a **bot for channel management**.  
The bot adds extra commands to enhance moderation, fun, and information handling.  

### General Commands
- `!help` – Displays all the commands the bot can handle.  
- `!ping` – Checks if the bot is active and responds with a pong and response time.  
- `!flip` – Heads or tails.  
- `!anon <MSG>` – Sends an anonymous message.  

### Operator Commands
- `!embed <MSG>` – Send a system-style message.  
- `!info <nickname>` – Displays all info about the user [Username, Nickname, ID, IP].  
- `!warn <nickname> <reason>` – Warn a user for bad behavior.  
- `!mute <nickname> <reason>` – Prevent a user from posting messages in the channel.  
- `!unmute <nickname>` – Restore a user's ability to write in the channel.  
- `!ban <nickname> <reason>` – Ban a user by nickname.  
- `!unban <nickname>` – Unban the user.  

This makes the server more interactive and closer to modern IRC usage.

---

## Installation

### Requirements
- C++98.  
- A UNIX-like system (Linux / macOS).  

### Compilation
```bash
make
```

This will generate the `ircserv` executable.

---

## Usage
Run the server with:  
```bash
./ircserv <port> <password>
```

- `<port>`: The TCP port the server will listen on (e.g., `6667`).  
- `<password>`: The server password required for clients to connect.  

### Example
```bash
./ircserv 6667 mypassword
```

Then connect with an IRC client (e.g., **Konversation**):  
```bash
konversation -c myserver -p 6667 -w mypassword
```

---

## Project Structure
- **Parsing**: Command parsing, argument validation, and error handling.  
- **Socket**: Low-level networking, connection handling, and client communication.  
- **Channel/User**: Data structures and logic for managing channels, users, and permissions.  

---

## Team Contributions
- **hhecquet** – Parsing  
- **heperez** – Socket  
- **bepoisso** – Channel/User/bot

---

## Disclamer
- 🇬🇧 This project has an educational purpose and you should under no circumstances copy and paste. Cheat is bad. Dont cheat

- 🇫🇷 Ce projet a un but educatif et vous ne devez en auccun cas faire du copier coller. Tricher c'est mal. Ne trichez pas

----
©42Perpignan(bepoisso)
