/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bepoisso <bepoisso@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:07:23 by bepoisso          #+#    #+#             */
/*   Updated: 2025/07/14 15:07:24 by bepoisso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdlib>

#include "user.hpp"
#include "channel.hpp"

class user;
class channel;

class bot {
private:
    /* Attributes */
    std::map<user*, bool> & _users;
    channel * _chan;

    std::map<user*, std::string> _botMessageAll(const std::string & msg);
    std::map<user*, std::string> _botMessageUser(user * target, const std::string & msg);
    std::string                  _error(int code, const std::string & nickname, const std::string & param, const std::string & message);
    std::string                  _botDMUser(user *target, const std::string & msg);
    bool                         _userIsOp(const std::string & nikename);
    user *                       _whoIsUser(const std::string & userNick);
    user *                       _whoIsGlobalUser(const std::string & userNick);

public:
    /* Canonical Form */
    bot(channel & chan);                  // Default constructor
    ~bot();                               // Destructor
    bot(const bot & other);               // Copy constructor
    bot & operator=(const bot & other);   // Assignent operator

    /* Operators */

    /* Methods */
    std::string                  _muteError();
    std::string                  _banError();
    std::string                  _chanError();
    // * OP COMMAND
    std::map<user*, std::string>  ban(const std::string & op, const std::string & target, const std::string & reason);
    std::map<user*, std::string>  unban(std::string op, std::string target);
    std::map<user*, std::string>  info(std::string op, std::string target);
    std::map<user*, std::string>  mute(std::string op, std::string target, std::string reason);
    std::map<user*, std::string>  unmute(std::string op, std::string target);
    std::map<user*, std::string>  warn(std::string op, std::string target, std::string reason);
    std::map<user*, std::string>  embed(std::string op, std::string msg);

    // *Global COMMAND
    std::map<user*, std::string>  help(const std::string & target);
    std::map<user*, std::string>  anon(std::string msg);
    std::map<user*, std::string>  ping(const std::string & target);
    std::map<user*, std::string>  flip(const std::string & target);


};
