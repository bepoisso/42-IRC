/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bepoisso <bepoisso@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:08:17 by bepoisso          #+#    #+#             */
/*   Updated: 2025/07/14 15:08:24 by bepoisso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "channel.hpp"

class channel;

class user {
private:
    // Attributes
    const std::string _nickName;
          std::string _userName;
          std::string _completName;
    const std::string _IP;
    int               _socket;

    std::vector<channel *> _invite;

    user(const user &other);
    user &operator=(const user &other);

public:
    // Canonical Form
    user(std::string nick, std::string ip, int socket);
    ~user();

/* Operator */
bool operator<(const user& other) const;
friend std::ostream& operator<<(std::ostream& os, const user& u);


/* Getter / Setter */
    const std::string &             getNickname() const;
    const std::string &             getUsername() const;
    const std::string &             getCompletName() const;
    const std::string &             getIP()       const;
    const int &                     getSocket()   const;
    const std::vector<channel *> &  getInvite()   const;

    void                            setUserCompletName(std::string NewUser, std::string newCompletName);
/* Methode */
    void printInviteList();
    void addInviteList(channel & c);
    bool findInviteChannel(channel & c);

};
