/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelOpCommand.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bepoisso <bepoisso@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:07:56 by bepoisso          #+#    #+#             */
/*   Updated: 2025/07/14 15:07:56 by bepoisso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

// * OP COMMANDE
std::map<user*, std::string> channel::updatePerm(user &op, user &target, bool perm) {
    std::map<user*, bool>::iterator it = _users.find(&op);
    std::map<user*, std::string> message;
    
    if (it == _users.end()) {
        std::cerr << RED << "error: operator not found" << RESET << std::endl ;
        return message;
    }
    else if (it->second == false) {
        std::clog << ORANGE << "warning: " << op << ORANGE << " permission denied" << RESET << std::endl;
        message[&op] = _error(ERR_CHANOPRIVSNEEDED, op.getNickname(), "#" + this->getName(), "You're not channel operator");
        return message;
    }

    it = _users.find(&target);
    if (it == _users.end()) {
        std::clog << RED << "error : <USER> not found" << RESET << std::endl;
        message[&op] = _error(ERR_NOSUCHNICK, op.getNickname(), "#" + this->getName(), "No such nickname");
        return message;
    }

// ? Not in mandatory but better
    if (perm == it->second && perm) {
        if (perm) {
            std::cerr << ORANGE << "warning: " << target << ORANGE << "is already OP" << RESET << std::endl;
            message[&op] = _error(0, op.getNickname(), "#" + this->getName(), "User is already OP");

            return message;
        }
        else if (perm == it->second && !perm){
            std::cerr << ORANGE << "warning: " << target << ORANGE << "is already DEOP" << RESET << std::endl;
            message[&op] = ":system PRIVMSG #" + this->getName() + " :error: " + target.getNickname() + " is already DEOP";
            message[&op] = _error(0, op.getNickname(), "#" + this->getName(), "User is already DEOP");
            return message;
        }
    }

    it->second = perm;
    if (perm) {
        message = _allMessage("MODE #" + getName() + " +o " + target.getNickname());
        std::clog << GREEN << "success: " << op << GREEN << " OP " << target << RESET << std::endl;
    }
    else {
        message = _allMessage("MODE #" + getName() + " -o " + target.getNickname());
        std::clog << GREEN << "success: " << op << GREEN << " DEOP " << target << RESET << std::endl;
    }
    return message;
}

std::map<user*, std::string> channel::kickUser(user & op, user & target, const std::string & reason) {
    std::map<user*, bool>::iterator it = _users.find(&op);
    std::map<user*, std::string> message;
    user *u = &target;
    if (it == _users.end())
        std::cerr << RED << "error : operator not found" << RESET << std::endl;
    else if (it->second == false) {
        std::clog << ORANGE << "warning : " << op << ORANGE << " permission denied" << RESET << std::endl;
        message[&op] = _error(ERR_CHANOPRIVSNEEDED, op.getNickname(), "#"+getName(), "You're not channel operator");
        return message;
    }

    it = _users.find(&target);
    if (it == _users.end()) {
        message[&op] = _error(ERR_NOSUCHNICK, op.getNickname(), "#" + this->getName(), "No such nickname");
        std::cerr << ORANGE << "warning: " << target << " user not found" << RESET << std::endl;
        return message;
    }
    else if (it->second == true) {
        std::cerr << ORANGE << "warning: " << target << ORANGE << " is already OP" << RESET << std::endl;
        message[&op] = _error(0, op.getNickname(), "#"+getName(), "This user is a Operator");
        return message;
    }

    _delUser(target);
    std::clog << GREEN << "success: " << op << GREEN << " kicked " << target << " Reason :" << reason << RESET << std::endl;
    message = _allMessage("KICK #" + this->getName() + " " + target.getNickname() + " :" + reason);
    message[u] = ":server PRIVMSG #" + this->getName() + " :You are kick from the channel #" + getName() + " by " + op.getNickname() + " Reason: " + reason;
    return message;
}

std::map<user*, std::string> channel::updateTopicPerm(user & op, bool perm) {
    std::map<user*, bool>::iterator it = _users.find(&op);
    std::map<user*, std::string> message;
    if (it == _users.end())
        std::cerr << RED << "error : operator not found" << RESET << std::endl;
    else if (it->second == false) {
        std::clog << ORANGE << "warning : " << op << ORANGE << " permission denied" << RESET << std::endl;
        message[&op] = _error(ERR_CHANOPRIVSNEEDED, op.getNickname(), "#"+getName(), "You're not channel operator");
        return message;
    }

    _permTopic = perm;
    std::clog << GREEN << "success: " << *this << "topic permission was update" << RESET << std::endl;
    message[&op] = "MODE #" + this->getName();
    if (perm)
        message[&op] += " +t";
    else
        message[&op] += " -t";
    message[&op] += " :Topic permission was update";
    return _allMessage(message[&op]);
}

std::map<user*, std::string> channel::updateInvitePerm(user & op, bool perm) {
    std::map<user*, bool>::iterator it = _users.find(&op);
    std::map<user*, std::string> message;
    if (it == _users.end())
        std::cerr << RED << "error : operator not found" << RESET << std::endl;
    else if (it->second == false) {
        std::clog << ORANGE << "warning : " << op << ORANGE << " permission denied" << RESET << std::endl;
        message[&op] = _error(ERR_CHANOPRIVSNEEDED, op.getNickname(), "#"+getName(), "You're not channel operator");
        return message;
    }

    _permInvite = perm;
    std::clog << GREEN << "success: " << *this << " invite permission was update" << RESET << std::endl;
    return _allMessage("MODE #" + this->getName() + " " + (perm ? "+i" : "-i") + " :Invite permission was update");
}

std::map<user*, std::string>   channel::updateUserLimit(user & op, unsigned int i) {
    std::map<user*, bool>::iterator it = _users.find(&op);
    std::map<user*, std::string> message;
    if (it == _users.end()) {
        std::cerr << RED << "error : operator not found" << RESET << std::endl;
        return message;
    }
    else if (it->second == false) {
        std::clog << ORANGE << "warning: " << op << ORANGE << " permission denied" << RESET << std::endl;
        message[&op] = _error(ERR_CHANOPRIVSNEEDED, op.getNickname(), "#"+getName(), "You're not channel operator");
        return message;
    }

    if (i > 9999) {
        std::clog << ORANGE << "warning: maximum users limit is 9999" << RESET << std::endl;
        message[&op] = _error(ERR_CHANNELISFULL, op.getNickname(), "#" + this->getName(), "Maximum user limit is 9999");
        return message;
    }
    else if (i < 2 && i != 0) {
        std::clog << ORANGE << "warning: minimum users limit is 2" << RESET << std::endl;
        message[&op] = _error(ERR_CHANNELISFULL, op.getNickname(), "#" + this->getName(), "Minimum user limit is 2");
        return message;
    }

    _limitUser = i;
    if (_limitUser == 0) {
        std::clog << GREEN << "success: limit was define to unlimited" << RESET << std::endl;
        return _allMessage("MODE #" + this->getName() + " -l :Limit was define to unlimited");
    }
    std::clog << GREEN << "success: limit was define to " << _limitUser << RESET << std::endl;
    std::ostringstream oss;
    oss << i;
    return _allMessage("MODE #" + this->getName() + " +l " + oss.str() + " :Limit was define to " + oss.str());
}

std::map<user*, std::string>   channel::updatePassword(user & op, std::string &newPass) {
    std::map<user*, bool>::iterator it = _users.find(&op);
    std::map<user*, std::string> message;
    if (it == _users.end()) {
        std::cerr << RED << "error : operator not found" << RESET << std::endl;
        return message;
    }
    else if (it->second == false) {
        std::clog << ORANGE << "warning: " << op << ORANGE << " permission denied" << RESET << std::endl;
        message[&op] = _error(ERR_CHANOPRIVSNEEDED, op.getNickname(), "#"+getName(), "You're not channel operator");
        return message;
    }

    _password = newPass;
    if (_password == "") {
        std::clog << GREEN << "success: password was delete" << RESET << std::endl;
        return _allMessage("MODE #" + this->getName() + " -k :Password was delete");
    }
        std::clog << GREEN << "success: password was define" << RESET << std::endl;
        return _allMessage("MODE #" + this->getName() + " +k " + newPass + " :Password was define");
}
