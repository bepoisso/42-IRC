/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserBot.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:07:28 by hhecquet          #+#    #+#             */
/*   Updated: 2025/07/14 15:08:39 by hhecquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

int parser_bot(user *callingUser, std::string chan, std::string line) {
	std::istringstream iss(line);
	std::string cmd = "",target = "", param = "", buf = "";
	std::string commands[] = {"!help","!ping","!flip","!anon","!embed","!info","!warn","!mute","!unmute","!ban","!unban"};
	iss >> cmd;

	int i;
	for (i = 0; i < 11; ++i) {
		if (cmd == commands[i])
			break;

	} switch (i) {
		case 0: //* !help
		{
			channel *targetChannel = find_channel(chan.substr(1));
			if (!targetChannel)
				return (0);
			std::map<user*, std::string> messages = targetChannel->marvin_bot->help(callingUser->getNickname());
			for (std::map<user*, std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
				std::istringstream iss(it->second);
				iss >> buf;
				if (buf[0] == ':')
					g_result.insert(std::make_pair(it->first->getSocket(), it->second + "\r\n"));
				else
					g_result.insert(std::make_pair(it->first->getSocket(), header(callingUser) + it->second + "\r\n"));
			}
			return(1);
		}
		case 1: //* !ping
		{
			channel *targetChannel = find_channel(chan.substr(1));
			if (!targetChannel)
				return (0);
			std::map<user*, std::string> messages = targetChannel->marvin_bot->ping(callingUser->getNickname());
			for (std::map<user*, std::string>::iterator ite = messages.begin(); ite != messages.end(); ++ite) {
				std::istringstream iss(ite->second);
				iss >> buf;
				if (buf[0] == ':')
					g_result.insert(std::make_pair(ite->first->getSocket(), ite->second + "\r\n"));
				else
					g_result.insert(std::make_pair(ite->first->getSocket(), header(callingUser) + ite->second + "\r\n"));
			}
			break;
		}
		case 2:  //* !flip
		{
			channel *targetChannel = find_channel(chan.substr(1));
			if (!targetChannel)
				return (0);
			std::map<user*, std::string> messages = targetChannel->marvin_bot->flip(callingUser->getNickname());
			for (std::map<user*, std::string>::iterator ite = messages.begin(); ite != messages.end(); ++ite) {
				std::istringstream iss(ite->second);
				iss >> buf;
				if (buf[0] == ':')
					g_result.insert(std::make_pair(ite->first->getSocket(), ite->second + "\r\n"));
				else
					g_result.insert(std::make_pair(ite->first->getSocket(), header(callingUser) + ite->second + "\r\n"));
			}
			break;
		}
		case 3: //* !anon
		{
			channel *targetChannel = find_channel(chan.substr(1));
			if (!targetChannel)
				return (0);
			std::getline(iss, param);
			if (!param.empty() && param[0] == ' ')
				param = param.substr(1);
			std::map<user*, std::string> messages = targetChannel->marvin_bot->anon(param);
			for (std::map<user*, std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
				std::istringstream iss(it->second);
				iss >> buf;
				if (buf[0] == ':')
					g_result.insert(std::make_pair(it->first->getSocket(), it->second + "\r\n"));
				else
					g_result.insert(std::make_pair(it->first->getSocket(), header(callingUser) + it->second + "\r\n"));
			}
			return(1);
		}
		case 4: //* !embed
		{
			channel *targetChannel = find_channel(chan.substr(1));
			if (!targetChannel)
				return (0);
			std::getline(iss, param);
			if (!param.empty() && param[0] == ' ')
				param = param.substr(1);
			std::map<user*, std::string> messages = targetChannel->marvin_bot->embed(callingUser->getNickname(), param);
			for (std::map<user*, std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
				std::istringstream iss(it->second);
				iss >> buf;
				if (buf[0] == ':')
					g_result.insert(std::make_pair(it->first->getSocket(), it->second + "\r\n"));
				else
					g_result.insert(std::make_pair(it->first->getSocket(), header(callingUser) + it->second + "\r\n"));
			}
			return(1);
		}
		case 5: //* !info
		{
			channel *targetChannel = find_channel(chan.substr(1));
			if (!targetChannel)
				return (0);
			std::getline(iss, target);
			if (!target.empty() && target[0] == ' ')
				target = target.substr(1);
			std::map<user*, std::string> messages = targetChannel->marvin_bot->info(callingUser->getNickname(), target);
			for (std::map<user*, std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
				std::istringstream iss(it->second);
				iss >> buf;
				if (buf[0] == ':')
					g_result.insert(std::make_pair(it->first->getSocket(), it->second + "\r\n"));
				else
					g_result.insert(std::make_pair(it->first->getSocket(), header(callingUser) + it->second + "\r\n"));
			}
			return(1);
		}
		case 6: //* !warn
		{
			channel *targetChannel = find_channel(chan.substr(1));
			if (!targetChannel)
				return (0);
			iss >> target;
			std::getline(iss, param);
			if (!param.empty() && param[0] == ' ')
				param = param.substr(1);
			std::map<user*, std::string> messages = targetChannel->marvin_bot->warn(callingUser->getNickname(), target, param);
			for (std::map<user*, std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
				std::istringstream iss(it->second);
				iss >> buf;
				if (buf[0] == ':')
					g_result.insert(std::make_pair(it->first->getSocket(), it->second + "\r\n"));
				else
					g_result.insert(std::make_pair(it->first->getSocket(), header(callingUser) + it->second + "\r\n"));
			}
			return(1);
		}
		case 7: //* !mute
		{
			channel *targetChannel = find_channel(chan.substr(1));
			if (!targetChannel)
				return (0);
			iss >> target;
			std::getline(iss, param);
			if (!param.empty() && param[0] == ' ')
				param = param.substr(1);
			std::map<user*, std::string> messages = targetChannel->marvin_bot->mute(callingUser->getNickname(), target, param);
			for (std::map<user*, std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
				std::istringstream iss(it->second);
				iss >> buf;
				if (buf[0] == ':')
					g_result.insert(std::make_pair(it->first->getSocket(), it->second + "\r\n"));
				else
					g_result.insert(std::make_pair(it->first->getSocket(), header(callingUser) + it->second + "\r\n"));
			}
			return(1);
		}
		case 8: //* !unmute
		{
			channel *targetChannel = find_channel(chan.substr(1));
			if (!targetChannel)
				return (0);
			std::getline(iss, target);
			if (!target.empty() && target[0] == ' ')
				target = target.substr(1);
			std::map<user*, std::string> messages = targetChannel->marvin_bot->unmute(callingUser->getNickname(), target);
			for (std::map<user*, std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
				std::istringstream iss(it->second);
				iss >> buf;
				if (buf[0] == ':')
					g_result.insert(std::make_pair(it->first->getSocket(), it->second + "\r\n"));
				else
					g_result.insert(std::make_pair(it->first->getSocket(), header(callingUser) + it->second + "\r\n"));
			}
			return(1);
		}
		case 9: //* !ban
		{
			channel *targetChannel = find_channel(chan.substr(1));
			if (!targetChannel)
				return (0);
			iss >> target;
			std::getline(iss, param);
			if (!param.empty() && param[0] == ' ')
				param = param.substr(1);
			user *targetUser = find_user(target, 0);
			std::map<user*, std::string> messages = targetChannel->marvin_bot->ban(callingUser->getNickname(), target, param);
			std::string::size_type found;
			for (std::map<user*, std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
				std::istringstream iss(it->second);
				iss >> buf;
				if (buf[0] == ':')
					g_result.insert(std::make_pair(it->first->getSocket(), it->second + "\r\n"));
				else
					g_result.insert(std::make_pair(it->first->getSocket(), header(callingUser) + it->second + "\r\n"));	
				found = it->second.find("You are ban from the server");
				if (found != std::string::npos) {
					g_result[it->first->getSocket()] += header(callingUser) + "MODE " + chan + " +b " + targetUser->getNickname() + "\r\n";
					g_result[it->first->getSocket()] += header(callingUser) + "KICK " + chan + " " + targetUser->getNickname() + "\r\n";
				}
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
			return(1);
		}
		case 10: //* !unban
		{
			channel *targetChannel = find_channel(chan.substr(1));
			if (!targetChannel)
				return (0);
			std::getline(iss, target);
			if (!target.empty() && target[0] == ' ')
				target = target.substr(1);
			user *targetUser = find_user(target, 0);
			std::map<user*, std::string> messages = targetChannel->marvin_bot->unban(callingUser->getNickname(), target);
			std::string::size_type found;
			for (std::map<user*, std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
				std::istringstream iss(it->second);
				iss >> buf;
				if (buf[0] == ':')
					g_result.insert(std::make_pair(it->first->getSocket(), it->second + "\r\n"));
				else
					g_result.insert(std::make_pair(it->first->getSocket(), header(callingUser) + it->second + "\r\n"));
				found = it->second.find("was unban form the channel by");
				if (found != std::string::npos)
					g_result[callingUser->getSocket()] += header(callingUser) + "MODE " + chan + " -b " + targetUser->getNickname() + "\r\n";
			}
			return(1);
		}
		default:
			return (0);
	}
	return (0);
}
