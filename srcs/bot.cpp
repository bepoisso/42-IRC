/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bepoisso <bepoisso@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:07:21 by bepoisso          #+#    #+#             */
/*   Updated: 2025/07/14 15:07:22 by bepoisso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"

    /* Canonical Form */
bot::bot(channel & chan) : _users(chan.getUsers()), _chan(&chan) {
}

bot::~bot() {
}

bot::bot(const bot & other) : _users(other._users) {
    *this = other;
}

bot & bot::operator=(const bot & other) {
    if (this != &other) {
        _chan = other._chan;
        _users = other._users;
    }
    return *this;
}

    // * MESSAGE
    std::map<user*, std::string> bot::_botMessageAll(const std::string & msg) {
        std::map<user*, std::string> message;
        for(std::map<user*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
            message[it->first] = ":🤖Marvin_bot PRIVMSG #" + _chan->getName() + " :" + msg;
        }
        return message;
    }

    std::string bot::_error(int code, const std::string & nickname, const std::string & param, const std::string & message) {
        std::stringstream s;
        s << ":🤖Marvin_bot" << " " << code << " " << nickname << " " << param << " :" << message;
        return s.str();
    }

    std::map<user*, std::string> bot::_botMessageUser(user * target, const std::string & msg) {
        std::map<user*, std::string> message;
        std::map<user*, bool>::iterator it = _users.find(target);

        if (it == _users.end()) {
            std::clog << RED << "error : <USER> not found" << RESET << std::endl;
        }
        message[target] = ":🤖Marvin_bot PRIVMSG #" + _chan->getName() + " :(wisper 🤫): " + msg;
        return message;
    }


    std::string bot::_botDMUser(user *target, const std::string & msg) {
        return std::string(":🤖Marvin_bot PRIVMSG " + target->getNickname() + " :" + msg);
    }


    bool bot::_userIsOp(const std::string & nikename) {
        for (std::map<user*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
            if (it->first->getNickname() == nikename)
                return it->second;
        }
        return false;
    }

    user * bot::_whoIsUser(const std::string & userNick) {
        for (std::map<user*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
            if (it->first->getNickname() == userNick)
                return it->first;
        }
        return NULL;
    }

    user * bot::_whoIsGlobalUser(const std::string & userNick) {
        for (size_t i = 0; i < g_userList.size(); i++) {
            if (g_userList[i]->getNickname() == userNick)
                return g_userList[i];
        }
        return NULL;
    }

    // * OP COMMAND
    std::map<user*, std::string>  bot::ban(const std::string & op, const std::string & target, const std::string & reason) {
        std::map<user*, std::string> message;
        user * temp = _whoIsGlobalUser(op);
        
        if (!_chan->_isInChannel(*temp)) {
            message[temp] = _chanError();
            return message;
        }
        else if (_chan->_isUserMute(*temp)) {
            message[temp] = _muteError();
            return message;
        }
        else if (_chan->_isUserBan(*temp)) {
            message[temp] = _banError();
            return message;
        }

        if (_userIsOp(op) == false)
            return _botMessageUser(_whoIsUser(op), _error(ERR_CHANOPRIVSNEEDED, op, "#" + _chan->getName(), "You're not channel operator"));
        if (_whoIsUser(target) == NULL)
            return _botMessageUser(_whoIsUser(op), _error(ERR_NOSUCHNICK, op, target, "No such nickname"));
        
        _chan->addUserToBanList(_whoIsUser(target));
        _chan->_delUser(*(_whoIsUser(target)));
        message = _botMessageAll(target + " was ban form the channel by " + op + " reason : " + reason);
        message[_whoIsGlobalUser(target)] = ":🤖Marvin_bot PRIVMSG #" + _chan->getName() + " :You are ban from the server #" + _chan->getName() + " by " + op + " Reason: " + reason; 
        return message;
    }

    std::map<user*, std::string>  bot::unban(std::string op, std::string target) {
        std::map<user *, std::string> message;
        user * temp = _whoIsGlobalUser(op);
        
        if (!_chan->_isInChannel(*temp)) {
            message[temp] = _chanError();
            return message;
        }
        else if (_chan->_isUserMute(*temp)) {
            message[temp] = _muteError();
            return message;
        }
        else if (_chan->_isUserBan(*temp)) {
            message[temp] = _banError();
            return message;
        }

        if (_userIsOp(op) == false)
            return _botMessageUser(_whoIsUser(op), "error: permission denied");
        _chan->delUserToBanList(_whoIsGlobalUser(target)); 
        return _botMessageAll(target + " was unban form the channel by " + op);
    }

    std::map<user*, std::string>  bot::info(std::string op, std::string target) {
        std::map<user *, std::string> message;
        user * temp = _whoIsGlobalUser(op);
        
        if (!_chan->_isInChannel(*temp)) {
            message[temp] = _chanError();
            return message;
        }
        else if (_chan->_isUserMute(*temp)) {
            message[temp] = _muteError();
            return message;
        }
        else if (_chan->_isUserBan(*temp)) {
            message[temp] = _banError();
            return message;
        }

        temp = _whoIsGlobalUser(target);
        if (_userIsOp(op) == false)
            return _botMessageUser(_whoIsUser(op), "error: permission denied");
        if (!_chan->_isInChannel(*temp)) {
            message[_whoIsGlobalUser(op)] = ":server PRIVMSG #" + _chan->getName() + " :User not found in #" + _chan->getName();;
            return message;
        }
        user *u = _whoIsUser(target);
        return _botMessageUser(_whoIsUser(op), "INFO : < NICK: " + u->getNickname() + " | USER: " + u->getUsername() + " | REAL NAME: " + u->getUsername() + " | IP: " + u->getIP() + ">");
    }

    std::map<user*, std::string>  bot::mute(std::string op, std::string target, std::string reason) {
        std::map<user *, std::string> message;
        user * temp = _whoIsGlobalUser(op);
        
        if (!_chan->_isInChannel(*temp)) {
            message[temp] = _chanError();
            return message;
        }
        else if (_chan->_isUserMute(*temp)) {
            message[temp] = _muteError();
            return message;
        }
        else if (_chan->_isUserBan(*temp)) {
            message[temp] = _banError();
            return message;
        }

        if (_userIsOp(op) == false)
            return _botMessageUser(_whoIsUser(op), "error: permission denied");

        if (_whoIsUser(target) == NULL)
            return _botMessageUser(_whoIsUser(op), "error: <USER> not found");

        _chan->addUserToMuteList(_whoIsUser(target));
        message = _botMessageAll(target + " was mute by " + op + " reason : " + reason);
        message[_whoIsGlobalUser(target)] = ":🤖Marvin_bot PRIVMSG #" + _chan->getName() + " :You are mute from the server #" + _chan->getName() + " by " + op + " Reason: " + reason; 
        return message;
    }

    std::map<user*, std::string>  bot::unmute(std::string op, std::string target) {
        std::map<user *, std::string> message;
        user * temp = _whoIsGlobalUser(op);
        
        if (!_chan->_isInChannel(*temp)) {
            message[temp] = _chanError();
            return message;
        }
        else if (_chan->_isUserMute(*temp)) {
            message[temp] = _muteError();
            return message;
        }
        else if (_chan->_isUserBan(*temp)) {
            message[temp] = _banError();
            return message;
        }

        if (_userIsOp(op) == false)
            return _botMessageUser(_whoIsUser(op), "error: permission denied");

        _chan->delUserToMuteList(_whoIsUser(target));
        return _botMessageAll(target + " was unmute form the channel by " + op);
    }

    std::map<user*, std::string>  bot::warn(std::string op, std::string target, std::string reason) {
        std::map<user *, std::string> message;
        user * temp = _whoIsGlobalUser(op);
        
        if (!_chan->_isInChannel(*temp)) {
            message[temp] = _chanError();
            return message;
        }
        else if (_chan->_isUserMute(*temp)) {
            message[temp] = _muteError();
            return message;
        }
        else if (_chan->_isUserBan(*temp)) {
            message[temp] = _banError();
            return message;
        }

        if (_userIsOp(op) == false)
            return _botMessageUser(_whoIsUser(op), "error: permission denied");
        
        std::string s = "❗ " + target + "(" + _whoIsUser(target)->getUsername() + ") have been WARN by " + op + "(" + _whoIsUser(op)->getUsername() +")! Reason: " + reason + "❗";
        return _botMessageAll(s);
    }

    std::map<user*, std::string>  bot::embed(std::string op, std::string msg) {
        std::map<user *, std::string> message;
        user * temp = _whoIsGlobalUser(op);
        
        if (!_chan->_isInChannel(*temp)) {
            message[temp] = _chanError();
            return message;
        }
        else if (_chan->_isUserMute(*temp)) {
            message[temp] = _muteError();
            return message;
        }
        else if (_chan->_isUserBan(*temp)) {
            message[temp] = _banError();
            return message;
        }
        
        if (_userIsOp(op) == false)
            return _botMessageUser(_whoIsUser(op), "error: permission denied");
        else
            return _botMessageAll(msg);
    }

    // *Global COMMAND
    std::map<user*, std::string>  bot::anon(std::string msg) {
        std::map<user *, std::string> message;
        for (std::map<user*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
            message[it->first] = ":👻Anonymous PRIVMSG #" + _chan->getName() + " :" + msg;
        }
        return message;
    }

    std::map<user*, std::string>  bot::ping(const std::string & target) {
        std::map<user*, std::string> message;
        user * temp = _whoIsGlobalUser(target);
        if (!_chan->_isInChannel(*temp)) {
            message[temp] = _chanError();
            return message;
        }
        else if (_chan->_isUserMute(*temp)) {
            message[temp] = _muteError();
            return message;
        }
        else if (_chan->_isUserBan(*temp)) {
            message[temp] = _banError();
            return message;
        }
        message[temp] = ":🤖Marvin_bot PRIVMSG #" + _chan->getName() + " :Pong";
        return message;
    }

    std::map<user*, std::string>  bot::flip(const std::string & target) {
        std::map<user*, std::string> message;
        user * temp = _whoIsGlobalUser(target);

        if (!_chan->_isInChannel(*temp)) {
            message[temp] = _chanError();
            return message;
        }
        else if (_chan->_isUserMute(*temp)) {
            message[temp] = _muteError();
            return message;
        }
        else if (_chan->_isUserBan(*temp)) {
            message[temp] = _banError();
            return message;
        }

        if (rand() % 2)
            message[temp] = ":🤖Marvin_bot PRIVMSG #" + _chan->getName() + " :head";
        else
            message[temp] = ":🤖Marvin_bot PRIVMSG #" + _chan->getName() + " :tails";
        return message;
    }

    std::map<user*, std::string>  bot::help(const std::string & target) {
        std::map<user*, std::string> message;
        message[_whoIsUser(target)] = ":🤖Marvin_bot PRIVMSG #" + _chan->getName() + " :Commands :\n\
        - !help                                 Displays all the commands the bot can handle.\n\
        - !ping                                 Checks if the bot is active and responds with a pong and response time.\n\
        - !flip                                 Heads or tails.\n\
        - !anon     <MSG>                       Sends an anonymous message.\n\n\
    OP COMMANDS:\n\
        - !embed    <MSG>                      Send a system-style message.\n\
        - !info     <nickname>                 Displays all info about the user [Username, Nickname, ID, IP].\n\
        - !warn     <nickname> <reason>        Warn a user for bad behavior.\n\
        - !mute     <nickname> <reason>        Prevent a user from posting messages in the channel.\n\
        - !unmute   <nickname>                 Restore a user's ability to write in the channel.\n\
        - !ban      <nickname> <reason>        Ban a user by nickname.\n\
        - !unban    <nickname>                 Unban the user.\n";
return message;

    }

    std::string bot::_muteError() {
        return ":server PRIVMSG #" + _chan->getName() + " :Can't send message you are mute form #" + _chan->getName();
    }

    std::string bot::_banError() {
        return ":server PRIVMSG #" + _chan->getName() + " :Can't send message you are ban form #" + _chan->getName();
    }

    std::string bot::_chanError() {
        return ":server PRIVMSG #" + _chan->getName() + " :Can't send message you are not in #" + _chan->getName();
    }
