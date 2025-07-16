/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:07:34 by hhecquet          #+#    #+#             */
/*   Updated: 2025/07/14 15:08:42 by hhecquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <map>

#include "ircserv.hpp"
#include "channel.hpp"
#include "bot.hpp"
#include "user.hpp"

class user;
class channel;
class bot;

extern std::map<int, std::string> g_result;
extern std::vector<int> g_correctPass;

/* Logs.cpp */
void add_logs(int fd, std::string line, bool in_out);

/* parserUtils.cpp */
channel* find_channel(std::string chan);
user* find_user(std::string name, int fd);
std::string header(user *User);
void ft_mode(user *callingUser, channel *targetChannel, std::string line);
void sendSideList(int fd, user *callingUser, channel *targetChannel);
std::string header(user *User);

/* parser.cpp */
std::map<int, std::string> parser(int fd, std::string ip, std::string line);

/* parserBot.cpp */
int parser_bot(user *callingUser, std::string chan, std::string line);
