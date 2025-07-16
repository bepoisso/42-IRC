/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_tools.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heperez <heperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:07:55 by heperez           #+#    #+#             */
/*   Updated: 2025/07/14 15:07:55 by heperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"

/* GET_CLIENT_IP : returns the IP address of a specified fd socket */
std::string get_client_ip(int client_fd)
{
    char ip[INET6_ADDRSTRLEN];
    struct sockaddr_storage addr;
    socklen_t len = sizeof(addr);
    const void* src_addr;

    memset(&addr, 0, sizeof(addr));
    memset(ip, 0, INET6_ADDRSTRLEN);

    if (getpeername(client_fd, (struct sockaddr*)&addr, &len) < 0) 
        return ("unknown");

    if (addr.ss_family == AF_INET) 
        src_addr = &(((struct sockaddr_in*)&addr)->sin_addr);
	else 
        src_addr = &(((struct sockaddr_in6*)&addr)->sin6_addr);

    if (inet_ntop(addr.ss_family, src_addr, ip, INET6_ADDRSTRLEN) == NULL) 
        return ("unknown");

    return (std::string(ip));
}

/* CREATE_SOCKET_IPV4 : creates the default socket for IPv4 incoming connections */
int create_socket_ipv4(std::vector<pollfd>& fds)
{
	int fd_default_ipv4 = socket(AF_INET, SOCK_STREAM, 0); 
	if (fd_default_ipv4 < 0)
	{
		std::cerr << RED << "Error: listen socket (ipv4) creation not possible." << RESET << std::endl;
		exit (1);
	}

	sockaddr_in addr4;
	memset(&addr4, 0, sizeof(addr4));
	addr4.sin_family = AF_INET;
	addr4.sin_port = htons(g_serverPort);
	addr4.sin_addr.s_addr = INADDR_ANY;

	if (bind(fd_default_ipv4, (sockaddr*)&addr4, sizeof(addr4)) < 0)
	{
		std::cerr << RED << "Error: impossible to bind the socket (ipv4)" << RESET <<std::endl;
		close(fd_default_ipv4);
		exit (1);
	}
	listen(fd_default_ipv4, 10);

	struct pollfd ipv4_fd;
	ipv4_fd.fd = fd_default_ipv4;
	ipv4_fd.events = POLLIN;
	ipv4_fd.revents = 0;
	fds.push_back(ipv4_fd);
	return (fd_default_ipv4);
}

/* CREATE_SOCKET_IPV6 : creates the default socket for IPv6 incoming connections */
int create_socket_ipv6(std::vector<pollfd>& fds)
{
	int fd_default_ipv6 = socket(AF_INET6, SOCK_STREAM, 0);
	if (fd_default_ipv6 < 0)
	{
		std::cerr << RED <<"Error: listen socket (ipv6) creation not possible." << RESET <<std::endl;
		close_vector_fd(fds);
		exit (1);
	}
	sockaddr_in6 addr6;
	memset(&addr6, 0, sizeof(addr6));
	addr6.sin6_family = AF_INET6;
	addr6.sin6_port = htons(g_serverPort);
	addr6.sin6_addr = in6addr_any;

	int v6only = 1;	// Protection to avoid conflict with ipv4 accepted by ipv6 socket.
    setsockopt(fd_default_ipv6, IPPROTO_IPV6, IPV6_V6ONLY, &v6only, sizeof(v6only));

	if (bind(fd_default_ipv6, (sockaddr*)&addr6, sizeof(addr6)) < 0) 
	{
		std::cerr << RED << "Error: impossible to bind the socket (ipv6)" << RESET << std::endl;
		close(fd_default_ipv6);
		close_vector_fd(fds);
		exit (1);
    }
	listen(fd_default_ipv6, 10);

	struct pollfd ipv6_fd;
	ipv6_fd.fd = fd_default_ipv6;
	ipv6_fd.events = POLLIN;
	ipv6_fd.revents = 0;
	fds.push_back(ipv6_fd);
	return (fd_default_ipv6);
}
