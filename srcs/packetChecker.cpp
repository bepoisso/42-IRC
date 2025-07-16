/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packetChecker.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heperez <heperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:07:59 by heperez           #+#    #+#             */
/*   Updated: 2025/07/14 15:08:35 by heperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packetChecker.hpp"

// CANONICAL FORM -------------------------------------------------------------
packetChecker::packetChecker(void)
{
}

packetChecker::packetChecker(packetChecker const &src)
{
    this->packet = src.packet;
}

packetChecker & packetChecker::operator=(packetChecker const & rhs)
{
    if (this != &rhs)
        this->packet = rhs.packet;
    return *this;
}

packetChecker::~packetChecker(void)
{
    this->packet.clear();
}
//-----------------------------------------------------------------------------

/* STORE_BUFFER : add the receive buffer into the private map */
void packetChecker::store_buffer(int input_fd, std::string input_buffer)
{
    if (this->packet[input_fd].empty())
        this->packet[input_fd] = input_buffer;
    else
        this->packet[input_fd] += input_buffer;
}

/* CHECK_COMPLETION : check if the packet is complete with /n */
bool packetChecker::check_completion(int input_fd)
{
    size_t len = this->packet[input_fd].length();
    if (len < 1) 
		return false;

    for (size_t i = 0; i < len; i++) {
        if (this->packet[input_fd][i] == '\n')
            return true;
    }
    return false;
}

/* GET_STORED_PACKET : return the stored packet and remove it from the map */
std::string packetChecker::get_stored_packet(int input_fd)
{
    std::string tmp = this->packet[input_fd];
    this->packet[input_fd].clear();
    return (tmp);
}
