/******************************************************************************
 * SequencerGUI                                                               *
 * Copyright (C) 2015                                                         *
 * Tomassino Ferrauto <t_ferrauto@yahoo.it>                                   *
 * Luca Anastasio <anastasio.lu@gmail.com>                                    *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation; either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software                *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA *
 ******************************************************************************/

#ifndef UTILS
#define UTILS

#include <memory>

/**
 * \file utils.h
 *
 * Utility functions
 */

// Checking C++ version and implementing std::make_unique if this is C++11
#if __cplusplus <= 199711L
	#error This program requires a C++14 or C++11 compiler
#elif __cplusplus <= 201103L

// Implementing make_unique
namespace std {
	template<typename T, typename... Ts>
	std::unique_ptr<T> make_unique(Ts&&... params)
	{
		return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
	}
}

#endif

#endif // UTILS

