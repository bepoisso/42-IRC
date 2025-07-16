/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heperez <heperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:07:49 by heperez           #+#    #+#             */
/*   Updated: 2025/07/14 15:07:49 by heperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"

bool					g_serverRunning;
int 					g_serverPort;
std::string 			g_password;
std::vector<user *>		g_userList;
std::vector<channel *>	g_channelList;


/* ARGV_CHECK : check if the input argument of the program is in the correct boundaries */
void argv_check(int argc, char **argv)
{	
	if (argc != 3)
	{
		std::cerr << "Error: start like this -> ./ircserv <port> <password>" << std::endl;
		exit (1);
	}
	g_password = argv[2];

	// PORT BOUNDARIES --------------------------------------------------------
	for (size_t i = 0; i < std::string(argv[1]).length(); i++) 
	{
		if (!isdigit(argv[1][i])) 
		{
			std::cerr << "Error: port must be a number" << std::endl;
			exit (1);
		}
	}
	g_serverPort = atoi(argv[1]);
	if (g_serverPort <= 0 || g_serverPort > 65535) 
	{
		std::cerr << "Error: port number must be between 1 and 65535" << std::endl;
		exit (1);
	}
	//-------------------------------------------------------------------------
}

int main(int argc, char **argv)
{
	argv_check(argc, argv);

	std::vector<pollfd> fds; // Vector to store all the stocket (fd etc...)
	int fd_default_ipv4 = create_socket_ipv4(fds);
	int fd_default_ipv6 = create_socket_ipv6(fds);

	packetChecker packet; 	// Object to store the incomplete packet.

	std::cout << GREEN << "Server initialization done ! - Waiting for incoming connection" << RESET << std::endl;
	g_serverRunning = true;
	signal(SIGINT, signal_handler);
	signal(SIGPIPE, SIG_IGN);
	while (g_serverRunning)
	{
		int incoming_msg = poll(&fds[0], fds.size(), -1);
		if (incoming_msg < 0 && g_serverRunning == true) 
		{
			std::cerr << RED <<"Error: poll return an error" << RESET <<std::endl;
			break;
		}
		for (int i = fds.size() - 1; i >= 0 && incoming_msg > 0; --i) {
			if (fds[i].revents & POLLIN) // We check the socket and POLLIN to see is there something to read
			{
				// FIRST TIME CONNECTION HANDLING
				if (fds[i].fd == fd_default_ipv4 || fds[i].fd == fd_default_ipv6) 
				{
					int client_fd = accept(fds[i].fd, NULL, NULL);
					if (client_fd < 0) 
					{
						std::cerr << RED << "Error: accept return an error" << RESET << std::endl;
						continue;
					}
					std::cout << BLUE <<"[+] New client connected : fd=" << client_fd << " ip= " << get_client_ip(client_fd) << RESET << std::endl;
					
					struct pollfd cli;
					cli.fd = client_fd;
					cli.events = POLLIN;
					cli.revents = 0;
					fds.push_back(cli);
				}

				// INCOMING TRAFIC AND DISCONNECTION
				else 
				{
					char buffer[1024];
					int n = recv(fds[i].fd, buffer, sizeof(buffer), 0);
					if (n > 0) 
					{
						std::cout << "[<- fd:"<< fds[i].fd << "]" << ": " << std::string(buffer, n) << std::endl;
						packet.store_buffer(fds[i].fd, std::string(buffer, n));
						if (packet.check_completion(fds[i].fd) == true)
						{
							std::map<int, std::string> outbound_msg = parser(fds[i].fd, get_client_ip(fds[i].fd), packet.get_stored_packet(fds[i].fd));
							for (std::map<int, std::string>::iterator it = outbound_msg.begin(); it != outbound_msg.end() ; it++)
							{
								if (it->first >= 3)
								{
									if (send(it->first, it->second.c_str(), it->second.length(), 0) < 0)
										std::cerr << "Error sending message to fd " << it->first << ": " << strerror(errno) << std::endl;
									else
									{
										add_logs(it->first, it->second, true);
										std::cout << "[-> fd:"<< it->first << "]" << ": " << it->second << std::endl;
									}
								}
							}
						}
						else
							std::cout << ORANGE << "Incomplete packet receive from fd=" << fds[i].fd << RESET << std::endl;
					}
					else
					{
						std::cout << BLUE << "[-] Client disconnected: fd=" << fds[i].fd << RESET << std::endl;
						parser(fds[i].fd, "", "QUIT"); // In case of disconnection without prior QUIT command
						close(fds[i].fd);
						fds.erase(fds.begin() + i);
					}
				}
				--incoming_msg;
			}
		}
	}
	clean_vector_user();
	clean_vector_channel();
	close_vector_fd(fds);
	std::cout << std::endl << GREEN << "Server stopping process - done" << RESET << std::endl;
	return (0);
}
