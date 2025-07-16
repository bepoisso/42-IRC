/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bepoisso <bepoisso@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:07:29 by bepoisso          #+#    #+#             */
/*   Updated: 2025/07/14 15:07:29 by bepoisso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <iterator>
#include <cstdlib>
#include <sstream>

#include "user.hpp"
#include "bot.hpp"
#include "ircserv.hpp"

class user;
class bot;

class channel {
    private:
    // Attributes
    std::string                             _name;
    std::string                             _password;
    std::string                             _topic;
    unsigned int                            _limitUser; // ? 0 = Unlimited
    bool                                    _permTopic;
    bool                                    _permInvite;
    
    std::map<user*, bool>                   _users;
    std::map<user*, bool>                   _banList;
    std::map<user*, bool>                   _muteList;
    
    /* Private Methode */
    void                                    _addUser(user &, bool);
    void                                    _printUserList();
    
    std::map<user*, std::string>            _systemMessage(const std::string & senderMSG);
    std::map<user*, std::string>            _allMessage(const std::string & msg);
    std::string                             _error(int code, const std::string & nickname, const std::string & param, const std::string & message);
    
    channel(const channel &other);
    channel &                               operator=(const channel &other);
    
    public:
    bot*                                    marvin_bot;
    
    // Canonical Form
    channel(std::string name, std::string pass, std::string topic,  user &owner);
    ~channel();
    
    /* Operator */
    friend std::ostream&                    operator<<(std::ostream& os, const channel& c);
    
    /* Getter / Setter */
    const std::string                        getName() const;
    const std::string                        getTopic() const;
    const std::string                        getPassword() const;
    unsigned int                             getLimitUser() const;
    std::map<user*, bool> &                  getUsers();
    
    
    
    /* Methode */
    void                                     addUserToBanList(user *);
    void                                     delUserToBanList(user *);
    void                                     addUserToMuteList(user *);
    void                                     delUserToMuteList(user *);
    void                                     _delUser(user &);
    bool                                     _isUserBan(user &); 
    bool                                     _isUserMute(user &);
    bool                                     _isInChannel(user &);
    unsigned int                             nbrUser();
    
    // * Messages
    std::map<user*, std::string>             sendMessage(user & sender, const std::string & senderMSG);
    
    // * Command
    std::map<user*, std::string>             join(user & target, std::string & typePassword);
    std::map<user*, std::string>             leave(user & target);
    std::map<user*, std::string>             invite(user & sender, user & target);
    std::map<user*, std::string>             topic(user & target);
    std::map<user*, std::string>             topic(user & target, const std::string & newtopic);
    
    // * OP command
    std::map<user*, std::string>             kickUser(user & op, user & target, const std::string & reason);
    // * MODE :
    /* -i */ std::map<user*, std::string>    updateInvitePerm(user & op, bool);
    /* -t */ std::map<user*, std::string>    updateTopicPerm(user & op, bool);
    /* -k */ std::map<user*, std::string>    updatePassword(user & op, std::string &);
    /* -o */ std::map<user*, std::string>    updatePerm(user & op, user & target, bool perm);
    /* -l */ std::map<user*, std::string>    updateUserLimit(user & op, unsigned int);
    
    
};

enum NumericReplies
{
	ERR_NOSUCHNICK        = 401, // No such nickname
	ERR_NOSUCHCHANNEL     = 403, // No such channel
	ERR_CANNOTSENDTOCHAN  = 404, // Cannot send to channel
	ERR_TOOMANYCHANNELS   = 405, // User has joined too many channels
	ERR_NONICKNAMEGIVEN   = 431, // No nickname given
	ERR_ERRONEUSNICKNAME  = 432, // Invalid nickname
	ERR_NICKNAMEINUSE     = 433, // Nickname is already in use
	ERR_USERNOTINCHANNEL  = 441, // Target user is not in the channel
	ERR_NOTONCHANNEL      = 442, // You are not on that channel
	ERR_USERONCHANNEL     = 443, // User is already on channel
	ERR_NOTREGISTERED     = 451, // You have not registered (NICK/USER/PASS missing)
	ERR_NEEDMOREPARAMS    = 461, // Not enough parameters
	ERR_ALREADYREGISTERED = 462, // Already registered
	ERR_PASSWDMISMATCH    = 464, // Password incorrect
	ERR_CHANNELISFULL     = 471, // Channel is full
	ERR_INVITEONLYCHAN    = 473, // Channel is invite-only (+i)
	ERR_BANNEDFROMCHAN    = 474, // Banned from channel
	ERR_BADCHANNELKEY     = 475, // Incorrect channel password
	ERR_CHANOPRIVSNEEDED  = 482, // You're not channel operator
	ERR_UMODEUNKNOWNFLAG  = 501, // Unknown user mode flag
	ERR_USERSDONTMATCH    = 502  // Cannot change mode for other users
};
