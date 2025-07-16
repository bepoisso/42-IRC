/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heperez <heperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:07:41 by heperez           #+#    #+#             */
/*   Updated: 2025/07/14 15:08:46 by heperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANSI_COLOR

# define ANSI_COLOR
# define BLACK   "\e[0;30m"
# define RED     "\e[0;31m"  // Reserved to Error
# define GREEN   "\e[0;32m"  // Reserved to Success
# define ORANGE  "\e[0;33m"  // Reserved to Warning
# define BLUE    "\e[0;34m"  // Reserved to Info
# define PURPLE  "\e[0;35m"	 // Reserved to Try
# define CYAN    "\e[0;36m"
# define WHITE   "\e[0;37m"  // Reserved to FD trafic
# define RESET   "\e[0m"

# define BOLD    "\033[1m"
# define UNBOLD    "\033[22m"

# define HIGH    "\033[1m"
# define UNHIGH  "\033[22m"

#endif

#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <poll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <csignal>
#include <map>

#include "packetChecker.hpp"

#include "parser.hpp"

#include "bot.hpp"
#include "channel.hpp"
#include "user.hpp"

class user;
class channel;
class bot;

extern bool 					g_serverRunning;
extern int						g_serverPort;
extern std::string 				g_password;
extern std::vector<user *>		g_userList;
extern std::vector<channel *> 	g_channelList;

// stop_process.cpp -----------------------------
void signal_handler(int sig);
void clean_vector_user(void);
void clean_vector_channel(void);
void close_vector_fd(std::vector<pollfd>& fds);
//-----------------------------------------------

// network_tools.cpp ----------------------------
std::string get_client_ip(int client_fd);
int create_socket_ipv4(std::vector<pollfd>& fds);
int create_socket_ipv6(std::vector<pollfd>& fds);
//-----------------------------------------------
