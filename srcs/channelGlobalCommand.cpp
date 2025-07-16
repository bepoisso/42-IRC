/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelGlobalCommand.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bepoisso <bepoisso@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:07:52 by bepoisso          #+#    #+#             */
/*   Updated: 2025/07/14 15:07:53 by bepoisso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

// * GLOBAL USER COMMAND
std::map<user*, std::string>   channel::join(user & target, std::string & typePassword) {
    std::map<user*, bool>::iterator it = _users.find(&target);
    std::map<user*, std::string>    message;

    if (it != _users.end()) {
        std::clog << RED << "error: " << target << " is alrady in the channel" << RESET << std::endl;
        message[&target] = _error(ERR_USERONCHANNEL, target.getNickname(), "#" + this->getName(), "You are already in the channel");
        return message;
    }

    std::clog << "Limits: " << _limitUser << std::endl << " nbruser: " << nbrUser() << std::endl;
    if (this->nbrUser() >= _limitUser && _limitUser != 0) {
        message[&target] = _error(ERR_CHANNELISFULL, target.getNickname(), "#" + this->getName() + " " + typePassword, "Channel is full");
        return message;
    }
    

    if (_permInvite == true) {
        std::vector<channel *> inv = target.getInvite();
        for (size_t i = 0; i < inv.size(); i++) {
            if (inv[i] == this)
                goto GoodInvite;
        }
        std::clog << ORANGE << "warning: " << target << " is not invite to the channel" << RESET << std::endl;
        message[&target] = _error(ERR_INVITEONLYCHAN, target.getNickname(), "#" + this->getName(), "You are not invite in this channel");
        return message;
    }

GoodInvite:
    if (_isUserBan(target)) {
        std::clog << ORANGE << "warning: " << target << " is ban from " << *this << RESET << std::endl;
        message[&target] = _error(ERR_BANNEDFROMCHAN, target.getNickname(), "#" + this->getName(), "You are banned from this channel");
        return message;
    }

    if (typePassword != _password) {
        std::clog << ORANGE << "warning: wrong password" << RESET << std::endl;
        message[&target] = _error(ERR_BADCHANNELKEY, target.getNickname(), "#" + this->getName(), "Wrong password for this channel");
    }
    else {
        _addUser(target, false);
        message = _allMessage("JOIN #" + this->getName());
    }
    return message;
}

std::map<user*, std::string>   channel::leave(user & target) {
    std::map<user*, bool>::iterator it = _users.find(&target);
    std::map<user*, std::string>    message;
    std::string nick = target.getNickname();

    if (it == _users.end()) {
        std::clog << RED << "error: " << target << " is not in the channel" << RESET << std::endl;
        message[&target] = _error(ERR_NOTONCHANNEL, target.getNickname(), "#" + this->getName(), "You are not in the channel");
        return message;
    }
    std::clog << GREEN << "success: " << target << " has left the channel" << RESET << std::endl;
    _delUser(target);
    if (nbrUser() <= 0)
        return message;
    return _allMessage("PART #" + this->getName() + " :" + nick + " has left the channel");
}

std::map<user*, std::string>    channel::topic(user & target) {
    std::map<user*, bool>::iterator it = _users.find(&target);
    std::map<user*, std::string>    message;
    
    if (it == _users.end()) {
        std::clog << RED << "error : <TARGET_USER> not found" << RESET << std::endl;
        return message;
    }
    std::clog << GREEN << "success: topic was send" << RESET << std::endl;
    return _allMessage("TOPIC #" + this->getName() + " :" + _topic);
}

std::map<user*, std::string>    channel::topic(user & target, const std::string & newtopic) {
    std::map<user*, bool>::iterator it = _users.find(&target);
    std::map<user*, std::string>    message;
    
    if (it == _users.end()) {
        std::clog << RED << "error : <TARGET_USER> not found" << RESET << std::endl;
        return message;
    }
    else if (newtopic == "") {
        return topic(target);
    }
    else if (it->second == false && _permTopic == true) {
        std::clog << ORANGE << "warning: permssiom denied" << RESET << std::endl;
        message[&target] = _error(ERR_CHANOPRIVSNEEDED, target.getNickname(), "#" + this->getName(), "You're not channel operator");
        return message;
    }

    std::clog << GREEN << "success: topic was update" << RESET << std::endl;
    _topic = newtopic;
   message = _allMessage("TOPIC #" + this->getName() + " :" + newtopic);
    return message;
}

// * MESSAGE

std::map<user*, std::string> channel::_allMessage(const std::string & msg) {
    std::map<user*, std::string> message;
    for (std::map<user*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
        message[it->first] = msg;
    }
    std::clog << GREEN << "success: _allMessage message was send" << RESET << std::endl;
    return message;
}

std::map<user*, std::string>   channel::_systemMessage(const std::string & senderMSG) {
    std::map<user*, std::string> message;

    for (std::map<user*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
        message[it->first] = ":server PRIVMSG #" + this->getName() + " :" + senderMSG;
    }
    std::clog << GREEN << "success: server message was send" << RESET << std::endl;
    return message;
}

std::map<user*, std::string>   channel::sendMessage(user & sender, const std::string & senderMSG) {
    std::map<user*, std::string> message;
    if (_isUserMute(sender)) {
        std::clog << ORANGE << "warning: " << sender << " try to send message but he is muted" << RESET << std::endl;        message[&sender] = ":server PRIVMSG #" + getName() + " :You are muted in this channel";
        message[&sender] = marvin_bot->_muteError();
    }
    else if (_isUserBan(sender)) {
        std::clog << ORANGE << "warning: " << sender << " try to send message but he is banned" << RESET << std::endl;
        message[&sender] = marvin_bot->_banError();

    }
    else if (!_isInChannel(sender)) {
        message[&sender] = marvin_bot->_chanError();
    }
    else {
        for (std::map<user*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
			if (it->first != &sender)
            	message[it->first] = "PRIVMSG #" + this->getName() + " :" + senderMSG;
        }
    }
    return message;
}

std::map<user*, std::string> channel::invite(user & sender, user & target) {
    std::map<user*, std::string> message;
    std::map<user*, bool>::iterator it = _users.find(&sender);

    if (it == _users.end()) {
        std::cerr << RED << "error: <SENDER_USER> not found" << RESET << std::endl;
        return message;
    }

    for (size_t i = 0; i < g_userList.size(); i++) {
        if (g_userList[i] == &target)
        goto GoodUser;
    }
    message[&sender] = _error(ERR_NOSUCHNICK, sender.getNickname(), target.getNickname(), "User not found");
    return message;
GoodUser:
    
    if (target.findInviteChannel(*this)) {
        std::clog << ORANGE << "warning: user is alrady invited to this channel" << RESET << std::endl;
        message[&sender] = _error(ERR_USERONCHANNEL, sender.getNickname(), "#" + this->getName(), "User is already invited to this channel");
        return message;
    }

    std::clog << GREEN << "success: " << target << "was invite to " << *this << " by " << sender << RESET << std::endl;
    target.addInviteList(*this);
    message[&sender] = ":server 341 " + sender.getNickname() + " " + target.getNickname() + " " + this->getName();
    message[&target] = "INVITE " + target.getNickname() + " :#" + this->getName(); 
    return message;
}
