/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bepoisso <bepoisso@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:07:27 by bepoisso          #+#    #+#             */
/*   Updated: 2025/07/14 15:08:45 by bepoisso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

    /* Canonical Form */

channel::channel(std::string name, std::string pass, std::string topic,  user &owner)
: _name(name), _password(pass), _topic(topic), _limitUser(0), _permTopic(false), _permInvite(false) {
    std::clog << GREEN << "success: create " << *this << RESET << std::endl;
    _addUser(owner, true);
    this->marvin_bot = new bot(*this);
}

channel::~channel() {
    delete marvin_bot;
}

channel::channel(const channel & other) : marvin_bot(other.marvin_bot) {
    *this = other;
}

channel & channel::operator=(const channel & other) {
    if (this != &other) {
        _name = other._name;
        _password = other._password;
        _topic = other._topic;
        _limitUser = other._limitUser;
        _permTopic = other._permTopic;
        _permInvite = other._permInvite;
        _users = other._users;
        marvin_bot = other.marvin_bot;
    }
    return *this;
}

    /* Operators */
std::ostream& operator<<(std::ostream& os, const channel& c) {
    os << BOLD << HIGH ;
    os << "<CHANNEL # Name: " << c.getName();
    os << " | Topic: " << c.getTopic();
    os << " | Passwd: " << c.getPassword();
    os << " | UserLimit: "<< c.getLimitUser() << ">" << UNBOLD << UNHIGH;

    return os;
}

    /*  Getters & Setters */
const std::string channel::getName() const {
    return _name;
}

const std::string       channel::getTopic() const {
    return _topic;
}

const std::string   channel::getPassword() const {
    return  _password;
}


unsigned int  channel::getLimitUser() const {
    return _limitUser;
}

std::map<user*, bool> &       channel::getUsers() {
    return _users;
}


    /* Methods */

void    channel::_printUserList() {
    std::clog << "USER LIST :" << std::endl;
    
    for (std::map<user*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
        if (it->second == true)
            std::clog << "ADMIN : ";
        std::clog << *(it->first) << std::endl;
    }
}

void channel::_addUser(user & target, bool perm) {
    std::map<user*, bool>::iterator it = _users.find(&target);

    if (it != _users.end()) {
        std::cerr << RED << "error: " << target << RED << " already exist" << RESET << std::endl;
        return;
    }
    _users[&target] = perm;
    std::clog << GREEN << "success: " << target << GREEN << " with grade " << perm << " was add to " << *this << RESET << std::endl;
}

void channel::_delUser(user &target) {
    std::map<user*, bool>::iterator it = _users.find(&target);
    if (it == _users.end()) {
        std::cerr << RED << "error: <USER> not found" << RESET << std::endl;
        return;
    }
    _users.erase(it);
    std::clog << GREEN << "success: " << target << GREEN << " was delete from " << *this << RESET << std::endl;
}

void channel::addUserToBanList(user * target) {
    std::map<user*, bool>::iterator it = _banList.find(target);

    if (it != _banList.end()) {
        std::cerr << RED << "error: <TARGET_USER> is already in the list" << RESET << std::endl;
        return;
    }
    _banList[target] = true;
}

void channel::delUserToBanList(user * target) {
    std::map<user*, bool>::iterator it = _banList.find(target);

    if (it == _banList.end()) {
        std::cerr << RED << "error: <TARGET_USER> is not in list (" << target << ")" << RESET << std::endl;
        return;
    }
    _banList[target] = false;
}

void channel::addUserToMuteList(user * target) {
    std::map<user*, bool>::iterator it = _muteList.find(target);

    if (it != _muteList.end()) {
        std::cerr << RED << "error: <TARGET_USER> is already in the list" << RESET << std::endl;
        return;
    }
    _muteList[target] = true;
}

void channel::delUserToMuteList(user * target) {
    std::map<user*, bool>::iterator it = _muteList.find(target);

    if (it == _muteList.end()) {
        std::cerr << RED << "error: <TARGET_USER> is not in list (" << target << ")" << RESET << std::endl;
        return;
    }
    _muteList[target] = false;
}

bool channel::_isUserBan(user & target) {
    std::map<user*, bool>::iterator it = _banList.find(&target);
    if (it != _banList.end())
        return it->second;
    return false;
}

bool channel::_isUserMute(user & target) {
    std::map<user*, bool>::iterator it = _muteList.find(&target);
    if (it != _muteList.end())
        return it->second;
    return false;
}

bool channel::_isInChannel(user & target) {
    std::map<user*, bool>::iterator it = _users.find(&target);
    if (it == _users.end())
        return false;
    return true;
}

std::string channel::_error(int code, const std::string & nickname, const std::string & param, const std::string & message) {
    std::ostringstream s;
    s << ":server ";
    if (code != 0)
        s << code << " ";
    s << nickname << " " << param << " :" << message;
    return s.str();
}

unsigned int channel::nbrUser() {
    int i = 0;
    std::map<user*, bool>::iterator it = _users.begin();
    while (it != _users.end()) {
        it++;
        i++;
    }
    return i;
}
