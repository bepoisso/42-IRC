/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bepoisso <bepoisso@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:08:15 by bepoisso          #+#    #+#             */
/*   Updated: 2025/07/14 15:08:27 by bepoisso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"

user::user(std::string nick, std::string ip, int socket) : _nickName(nick), _IP(ip), _socket(socket) {
}

user::~user() {
}

user::user(const user &other) {
    *this = other;
}

user &user::operator=(const user &other) {
    if (this != &other) {
        //Cant assigne const atributs
    }
    return *this;
}

/* Operator */
bool user::operator<(const user& other) const {
    return _socket < other._socket;
}

std::ostream& operator<<(std::ostream& os, const user& u) {
    os << BOLD << HIGH ;
    os << "<USER # Nickname: " << u.getNickname();
    os << " | Username: " << u.getUsername();
    os << " | Complet Name: " << u.getCompletName();
    os << " | IP: " << u.getIP();
    os << " | ID: " << u.getSocket() << ">" << UNBOLD << UNHIGH;
    return os;
}

/* Getter / Setter */
const std::string & user::getNickname() const {
    return _nickName;
}

const std::string & user::getUsername() const {
    return _userName;
}

const std::string & user::getIP() const {
    return _IP;
}

const int &         user::getSocket() const {
    return _socket;
}

const std::vector<channel*> & user::getInvite() const {
    return _invite;
}

const std::string & user::getCompletName() const {
    return _completName;
}

void user::setUserCompletName(std::string NewUser, std::string newCompletName) {
    _userName = NewUser;
    _completName = newCompletName;
}

/* Methode */

void user::printInviteList() {
    std::cout << CYAN << "---Invite channel list---" << std::endl;
    std::cout << "for: " << *this << std::endl;
    for (std::vector<channel*>::iterator it = _invite.begin(); it != _invite.end(); it++) {
        std::cout << *it << std::endl;
    }
    std::cout << RESET;
}

void user::addInviteList(channel & c) {
    _invite.push_back(&c);
}

bool user::findInviteChannel(channel & c) {
    for (size_t i = 0; i < _invite.size(); i++) {
        if (_invite[i] == &c)
            return true;
    }
    return false;
}
