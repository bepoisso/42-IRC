/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_process.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heperez <heperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:08:13 by heperez           #+#    #+#             */
/*   Updated: 2025/07/14 15:08:14 by heperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"

void signal_handler(int sig) 
{
    (void)sig;
    g_serverRunning = false;
}

/* CLEAN_VECTOR_USER : free the memory of g_userList */
void clean_vector_user(void)
{
    for (size_t i = 0; i < g_userList.size(); i++)
    {
        delete g_userList[i];
    }
    g_userList.clear();
}

/* CLEAN_VECTOR_CHANNEL : free the memory of g_channelList */
void clean_vector_channel(void)
{
    for (size_t i = 0; i < g_channelList.size(); i++)
    {
        delete g_channelList[i];
    }
    g_channelList.clear();
}

/* CLOSE_VECTOR_FD : ensure their is no fd still open in the vector */
void close_vector_fd(std::vector<pollfd>& fds)
{
    for (size_t i = fds.size() - 1; i < fds.size(); i--)
    {
        close(fds[i].fd);
    }
    fds.clear();
    std::vector<pollfd>().swap(fds);
}
