/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:08:56 by hhecquet          #+#    #+#             */
/*   Updated: 2025/07/14 15:09:09 by hhecquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

void add_logs(int fd, std::string line, bool in_out) {
	//in_out : true when output, false when input
    std::string filename = "logs.temp";

    std::ofstream fichier;
    fichier.open(filename.c_str(), std::ios::out | std::ios::app);
	time_t now = time(0);

	tm *ltm = localtime(&now);
	
	fichier << "[";
	fichier << std::setfill('0') << std::setw(4) << 1900 + ltm->tm_year;
	fichier << std::setfill('0') << std::setw(2) << 1 + ltm->tm_mon;
	fichier << std::setfill('0') << std::setw(2) << ltm->tm_mday;
	fichier << "-";
	fichier << std::setfill('0') << std::setw(2) << ltm->tm_hour;
	fichier << std::setfill('0') << std::setw(2) << ltm->tm_min;
	fichier << std::setfill('0') << std::setw(2) << ltm->tm_sec;
	fichier << "] ";
	fichier << "[fd:" << fd << "]";
	if (in_out == true)
		fichier << "-> ";
	else
		fichier << "<- ";

    fichier << line;
    fichier.close();
}