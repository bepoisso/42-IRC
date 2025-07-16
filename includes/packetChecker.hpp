/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packetChecker.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heperez <heperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:08:05 by heperez           #+#    #+#             */
/*   Updated: 2025/07/14 15:08:29 by heperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <iostream>

class packetChecker
{

public:

	// CANONICAL 1/2
	packetChecker(void);
	~packetChecker(void);
	//--------------

	void store_buffer(int input_fd, std::string input_buffer);
	bool check_completion(int input_fd);
	std::string get_stored_packet(int input_fd);

private:

	// CANONICAL 2/2
	packetChecker(packetChecker const & src);
	packetChecker & operator=(packetChecker const & rhs);
	//--------------

	std::map<int, std::string> packet;

};
