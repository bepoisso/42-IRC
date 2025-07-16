/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:07:55 by hhecquet          #+#    #+#             */
/*   Updated: 2025/07/14 15:07:55 by hhecquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

channel* find_channel(std::string chan) {
	for (std::vector<channel *>::iterator it = g_channelList.begin(); it != g_channelList.end(); ++it) {
		if ((*it)->getName() == chan)
			return (*it);
	}
	return NULL;
}

user* find_user(std::string name, int fd){
	for (std::vector<user *>::iterator it = g_userList.begin(); it != g_userList.end(); ++it) {
		if (name.empty()) {
            if ((*it)->getSocket() == fd)
                return (*it);
        } else {
            if ((*it)->getNickname() == name)
                return (*it);
        }
    }
    return NULL;
}

std::string header(user *User) {
	return (":" + User->getNickname() + "!" + User->getUsername() + "@" + User->getIP() + " ");
}

void ft_mode(user *callingUser, channel *targetChannel, std::string line) {
	std::istringstream iss(line);
	std::string mode = "", param = "", target = "", buf = "";
	char options[] = {'i','t','k','o','l'};
	
	iss >> mode;
	if (mode.length() != 2)
		return;
	
	int i;
	for (i = 0; i < 5; ++i) {
		if (mode[1] == options[i])
			break;

	} switch (i) {
		case 0:	//* i
		{
			std::map<user*, std::string> messages = targetChannel->updateInvitePerm(*callingUser, mode[0] == '+' ? true : false);
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
		case 1:	//* t
		{
			std::cout << mode << " mode\n";

			std::map<user*, std::string> messages = targetChannel->updateTopicPerm(*callingUser, mode[0] == '+' ? true : false);
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
		case 2:	//* k
		{
			if (mode[0] == '+'){
				std::getline(iss, param);
				if (!param.empty() && param[0] == ' ')
					param = param.substr(1);
			}
			std::map<user*, std::string> messages = targetChannel->updatePassword(*callingUser, param);
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
		case 3:	//* o
		{
			iss >> target;
			
			user *targetUser(find_user(target, 0));
			if (!targetUser)
				return;

			std::map<user*, std::string> messages = targetChannel->updatePerm(*callingUser, *targetUser, mode[0] == '+' ? true : false);
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
		case 4:	//* l
		{
			if (mode[0] == '+') {
				std::getline(iss, param);
				if (param.empty())
					param += "0";
			}
			std::map<user*, std::string> messages = targetChannel->updateUserLimit(*callingUser, atoi(param.c_str()));
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
		default:
			return;
	}
	std::getline(iss, buf);
	std::cout << buf << " buf\n";
	if (!buf.empty())
		ft_mode(callingUser, targetChannel, buf);
	return;
}

void sendSideList(int fd, user *callingUser, channel *targetChannel) {
	std::map<user*, bool> & channelUsers = targetChannel->getUsers();
	std::string param = ":server 353 " + callingUser->getNickname() + " = #" + targetChannel->getName() + " :";
	for (std::map<user*, bool>::iterator it = channelUsers.begin(); it != channelUsers.end(); ++it) {
		if (it->second == true)
			param += "@";
		param += it->first->getNickname() + " ";
	}
	param += "\r\n";
	g_result[fd] += param;
	param = ":server 366 " + callingUser->getNickname() + " #" + targetChannel->getName() + " :End of /NAMES list\r\n";
	g_result[fd] += param;
}
