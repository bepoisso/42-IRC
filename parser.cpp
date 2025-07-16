/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:07:37 by hhecquet          #+#    #+#             */
/*   Updated: 2025/07/14 16:31:54 by hhecquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

std::map<int, std::string> g_result;
std::vector<int> g_correctPass;

std::map<int, std::string>	parser(int fd, std::string ip, std::string line) {
	std::istringstream iss(line);
	std::string cmd = "", chan = "", target = "", param = "", buf = "";
	std::string commands[] = {"KICK","INVITE","TOPIC","mode","NICK","USER","JOIN","PRIVMSG","PASS","PART","QUIT"};
	g_result.clear();
	iss >> cmd;
	add_logs(fd, line, false);

	(void)ip;
	user *callingUser(find_user("", fd));
	std::vector<int>::iterator it;
	for (it = g_correctPass.begin(); it != g_correctPass.end(); ++it) {
		if (*it == fd)
			break;
	}
	if ((it == g_correctPass.end() && cmd != "PASS") || (!callingUser && (cmd != "NICK" && cmd != "PASS")))
		goto error;
	int i;
	for (i = 0; i < 11; ++i) {
		if (cmd == commands[i] || (commands[i] == "mode" && cmd == "MODE"))
			break;

	} switch (i) {
		case 0:	//* KICK 🤖
		{
			/* Kick a user from the channel*/
			iss >> chan >> target;
			
			channel *targetChannel = find_channel(chan.substr(1));
			user *targetUser(find_user(target, 0));
			if (chan.empty() || target.empty() || !targetChannel || !targetUser)
				goto error;
			std::getline(iss, param);
			if (!param.empty() && param[0] == ' ')
				param = param.substr(1);
			if (!param.empty() && param[0] == ':')
				param = param.substr(1);
			std::map<user*, std::string> messages = targetChannel->kickUser(*callingUser, *targetUser, param);
			std::string::size_type found;
			for (std::map<user*, std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
				std::istringstream iss(it->second);
				iss >> buf;
				if (buf[0] == ':')
					g_result.insert(std::make_pair(it->first->getSocket(), it->second + "\r\n"));
				else
					g_result.insert(std::make_pair(it->first->getSocket(), header(callingUser) + it->second + "\r\n"));
				found = it->second.find("You are kick from the channel");
				if (found != std::string::npos)
					g_result[it->first->getSocket()] += header(callingUser) + "KICK " + chan + " " + targetUser->getNickname() + "\r\n";
			}
			if (targetChannel && targetChannel->nbrUser() == 0) {
				for (std::vector<channel *>::iterator ite = g_channelList.begin(); ite != g_channelList.end(); ) {
					if ((*ite) == targetChannel) {
						delete *ite;
						ite = g_channelList.erase(ite);
					} else {
						++ite;
					}
				}
			}
			break;
		}
		case 1:	//* INVITE
		{
			/* Invite someone to a channel*/
			iss >> target;

			std::getline(iss, chan);
			if (!chan.empty() && chan[0] == ' ')
				chan = chan.substr(1);
			//TODO: regarde les photos
			channel *targetChannel = find_channel(chan.substr(1));
			user *targetUser(find_user(target, 0));
			if (!targetChannel || !targetUser)
				goto error;
			std::cout << "oui?\n";
			std::map<user*, std::string> messages = targetChannel->invite(*callingUser, *targetUser);
			for (std::map<user*, std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
				std::istringstream iss(it->second);
				iss >> buf;
				if (buf[0] == ':')
					g_result.insert(std::make_pair(it->first->getSocket(), it->second + "\r\n"));
				else
					g_result.insert(std::make_pair(it->first->getSocket(), header(callingUser) + it->second + "\r\n"));
			}
			break;
		}
		case 2:	//* TOPIC
		{
			/* Change the topic of a channel*/
			iss >> chan;

			channel *targetChannel = find_channel(chan.substr(1));
			if (!targetChannel)
				goto error;
			
			std::getline(iss, param);
			if (!param.empty() && param[0] == ' ')
				param = param.substr(1);
			if (!param.empty() && param[0] == ':')
				param = param.substr(1);
			std::map<user*, std::string> messages = targetChannel->topic(*callingUser, param);
			for (std::map<user*, std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
				std::istringstream iss(it->second);
				iss >> buf;
				if (buf[0] == ':')
					g_result.insert(std::make_pair(it->first->getSocket(), it->second + "\r\n"));
				else
					g_result.insert(std::make_pair(it->first->getSocket(), header(callingUser) + it->second + "\r\n"));
			}
			break;
		}
		case 3:	// *MODE
		{
			/* Change the channel's modes:
				+i / -i : Enable/Disable invite-only mode
				+t / -t : Allow only channel operators to change the topic
				+k / -k : Set/Remove the channel key (password)
				+o / -o : Grant/Revoke operator privileges
				+l / -l : Set/Remove the user limit for the channel */
			iss >> chan;
			channel *targetChannel;
			if (!chan.empty() && chan[0] == '#')
				targetChannel = find_channel(chan.substr(1));
			else
				targetChannel = find_channel(chan);
			std::getline(iss, buf);
			if (!targetChannel)
				goto error;
			ft_mode(callingUser, targetChannel, buf);
			break;
		}
		case 4:	//* NICK​
		{
			/* Add user's nickname */
			std::getline(iss, param);
			if (!param.empty() && param[0] == ' ')
				param = param.substr(1);
			std::vector<int>::iterator it;
			for (it = g_correctPass.begin(); it != g_correctPass.end(); ++it) {
				if (*it == fd)
					break;
			}
			if (param.empty() || find_user("", fd) != NULL || it == g_correctPass.end())
				goto error;
			if (find_user(param, 0) != NULL) {
				g_result.insert(std::make_pair(fd, "System Nickname already taken\r\n"));
				for (std::vector<int>::iterator iter = g_correctPass.begin(); iter != g_correctPass.end(); ++iter) {
					if (*iter == fd){
						iter = g_correctPass.erase(iter);
						break;
					}
				}
				return (g_result);
			}
			user *newUser = new user(param, ip, fd);
			g_userList.push_back(newUser);
			break;
		}
		case 5:	//* USER​
		{
			/* Add user's username */
			iss >> param;
			iss >> buf >> buf;
			std::getline(iss, buf);
			if (!buf.empty() && buf[0] == ' ')
				buf = buf.substr(1);
			if (!buf.empty() && buf[0] == ':')
				buf = buf.substr(1);
			if (callingUser->getUsername() == "")
				callingUser->setUserCompletName(param, buf);
			break;
		}
		case 6:	//* JOIN​
		{
			/* Join a channel */
			iss >> chan;
			std::getline(iss, param);
			if (!param.empty() && param[0] == ' ')
				param = param.substr(1);
			channel *targetChannel = find_channel(chan.substr(1));
			if (targetChannel && targetChannel->nbrUser() == 0) {
				for (std::vector<channel *>::iterator ite = g_channelList.begin(); ite != g_channelList.end(); ) {
					if ((*ite) == targetChannel) {
						delete *ite;
						ite = g_channelList.erase(ite);
					} else {
						++ite;
					}
				}
			}
			int error = 0;
			if (!targetChannel) {
				targetChannel = new channel(chan.substr(1), "", "", *callingUser);
				g_channelList.push_back(targetChannel);
				g_result.insert(std::make_pair(callingUser->getSocket(), header(callingUser) + "JOIN " + chan + "\r\n"));
				error = 1;
			} else {
				std::map<user*, std::string> messages = targetChannel->join(*callingUser, param);
				std::string::size_type found;
				for (std::map<user*, std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
					std::istringstream iss(it->second);
					iss >> buf;
					if (buf[0] == ':')
						g_result.insert(std::make_pair(it->first->getSocket(), it->second + "\r\n"));
					else
						g_result.insert(std::make_pair(it->first->getSocket(), header(callingUser) + it->second + "\r\n"));
					found = it->second.find("JOIN #");
					if (found != std::string::npos)
						error = 1;
				}
			}
			if (error == 1)
				sendSideList(fd, callingUser, targetChannel);
			break;
		}
		case 7:	//* PRIVMSG
		{
			/* Send private messages */
			iss >> target;
			std::getline(iss, param);
			if (param.empty() || target.empty())
				goto error;
			if (!param.empty() && param[0] == ' ')
				param = param.substr(1);
			if (param[0] == ':')
				param = param.substr(1);
			if (target[0] == '#' && (!param.empty() && param[0] == '!'))
				parser_bot(callingUser, target, param);
			else if (target[0] == '#') {
				channel* targetChannel = find_channel(target.substr(1));
				std::map<user*, std::string> messages = targetChannel->sendMessage(*callingUser, param);
				for (std::map<user*, std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
					std::istringstream iss(it->second);
					iss >> buf;
					if (buf[0] == ':')
						g_result.insert(std::make_pair(it->first->getSocket(), it->second + "\r\n"));
					else
						g_result.insert(std::make_pair(it->first->getSocket(), header(callingUser) + it->second + "\r\n"));
				}
			} else {
				user* targetUser = find_user(target, 0);
				g_result.insert(std::make_pair(targetUser->getSocket(), header(callingUser) + line + "\r\n"));
			}
			break;
		}
		case 8:	//* PASS
		{
			/* Check if the user connect to the server with the correct password */
			if (callingUser)
				goto error;
			std::getline(iss, param);
			if (!param.empty() && param[0] == ' ')
				param = param.substr(1);
			if (!param.empty() && param[0] == ':')
				param = param.substr(1);
			if (param != g_password) {
				g_result.insert(std::make_pair(fd, "System Password incorrect\r\n"));
				return (g_result);
			}
			g_correctPass.push_back(fd);
			break;
		}
		case 9:	//* PART
		{
			/* Leave a channel */
			iss >> chan;
			channel *targetChannel = find_channel(chan.substr(1));
			if (!targetChannel)
				goto error;
			std::map<user*, std::string> messages = targetChannel->leave(*callingUser);
			std::string::size_type found;
			int error = 0;
			for (std::map<user*, std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
				std::istringstream iss(it->second);
				iss >> buf;
				if (buf[0] == ':')
					g_result.insert(std::make_pair(it->first->getSocket(), it->second + "\r\n"));
				else
					g_result.insert(std::make_pair(it->first->getSocket(), header(callingUser) + it->second + "\r\n"));
				found = it->second.find("has left the channel");
				if (found != std::string::npos)
					error = 1;
			}
			if (error == 1 || messages == std::map<user*, std::string> ())
				g_result[fd] += header(callingUser) + "PART " + chan + "\r\n";
			if (targetChannel && targetChannel->nbrUser() == 0) {
				for (std::vector<channel *>::iterator ite = g_channelList.begin(); ite != g_channelList.end(); ) {
					if ((*ite) == targetChannel) {
						delete *ite;
						ite = g_channelList.erase(ite);
					} else {
						++ite;
					}
				}
			}
			break;
		}
		case 10: //* QUIT
		{
			/* Disconnect to the server */
			std::map<user*, bool> channelUsers;
			std::map<user*, std::string> messages;
			std::vector<int>::iterator iter;
			for (iter = g_correctPass.begin(); iter != g_correctPass.end(); ++iter) {
				if (*iter == fd) {
					iter = g_correctPass.erase(iter);
					break;
				}
			}
			std::vector<user *>::iterator itera;
			for (itera = g_userList.begin(); itera != g_userList.end(); ++itera) {
				if ((*itera)->getSocket() == fd) {
					delete *itera;
					itera = g_userList.erase(itera);
					break;
				}
			}
			if (iter == g_correctPass.end() || itera == g_userList.end())
				goto error;
			for(std::vector<channel *>::iterator it = g_channelList.begin(); it != g_channelList.end(); ++it)
			{
				channelUsers.clear();
				channelUsers = (*it)->getUsers();
				for (std::map<user*, bool>::iterator ite = channelUsers.begin(); ite != channelUsers.end(); ++ite) {
					if (ite->first->getSocket() && ite->first->getSocket() == fd) {
						messages.clear();
						messages = (*it)->leave(*callingUser);
						for (std::map<user*, std::string>::iterator iter = messages.begin(); iter != messages.end(); ++iter) {
							std::istringstream iss(iter->second);
							iss >> buf;
							if (buf[0] == ':')
								g_result.insert(std::make_pair(iter->first->getSocket(), iter->second + "\r\n"));
							else
								g_result.insert(std::make_pair(iter->first->getSocket(), header(callingUser) + iter->second + "\r\n"));
						}
					}
				}
			}
			break;
		}
		default:	
			error:
			return std::map<int, std::string>();
	}
	return (g_result);
}
