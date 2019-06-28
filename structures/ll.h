/*
 * ll.hpp
 * 
 * Copyright 2018 d <deimos@mercury>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#ifndef LL_HPP
#define LL_HPP
#include <ctime>
#include <stdio.h>
#include <stdlib.h>

struct Node
{
	void * obj;
	int codes[4];
	time_t t;
	Node * next = nullptr;
	Node * prev = nullptr;
};

class LL
{
	public:
		Node *  Insert(void * newb);
		Node *  Insert(void * newb, int c);
		Node * Insert(void * newb, int c, int x, int y, int s);
		Node * Remove(Node *);
		Node * PopHead();
		Node * head = nullptr;
		Node * tail = nullptr;
};

#endif /* LL_HPP */ 
