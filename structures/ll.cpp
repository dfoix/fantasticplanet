/*
 * ll.cpp
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


#include "ll.h"


Node *  LL::Insert(void * newb){
	Node * temp = new Node;
	temp->obj = newb;	
	if(head == nullptr){
		head = tail = temp;
		temp->next = nullptr;
		temp->prev = nullptr;
	
	}
	else
	{
		tail->next = temp;
		temp->prev = tail;
		tail = temp;
		temp->next = nullptr;
	}
	return temp;

}

Node *  LL::Insert(void * newb, int c){
	Node * temp = new Node;
	temp->obj = newb;	
	temp->codes[0] = c;
	if(head == nullptr){
		head = tail = temp;
		temp->next = nullptr;
		temp->prev = nullptr;
	
	}
	else
	{
		tail->next = temp;
		temp->prev = tail;
		tail = temp;
		temp->next = nullptr;
	}
	return temp;
}

Node *  LL::Insert(void * newb, int c, int x, int y, int s){
	Node * temp = new Node;
	temp->obj = newb;	
	temp->codes[0] = c;
	temp->codes[1] = x;
	temp->codes[2] = y;
	temp->codes[3] = s;

	if(head == nullptr){
		head = tail = temp;
		temp->next = nullptr;
		temp->prev = nullptr;
	
	}
	else
	{
		tail->next = temp;
		temp->prev = tail;
		tail = temp;
		temp->next = nullptr;
	}
	return temp;
}

Node * LL::Remove(Node * gone){
	if(gone->prev)
		gone->prev->next = gone->next;
	if(gone->next)
		gone->next->prev = gone->prev;
	if(head == tail){
		head = tail = nullptr;
	}
	else{
		if(gone == head){
			head = gone->next;
			head->prev = nullptr;
		}

		if(gone == tail){
			tail = gone->prev;
			tail->next = nullptr;
		}
	}
	
	return gone;
}
Node * LL::PopHead(void){
	Node * temp = head;
	if(head == tail)
		tail = nullptr;
	if(head != nullptr)
		head = head->next;
	return temp;
}
