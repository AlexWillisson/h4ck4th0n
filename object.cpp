#include "hack.h"
#include <netinet/in.h>

bool Object::send(Socket socket) {
	char *buf = new char[35];
	*((int *)buf) = htonl(*reinterpret_cast<int *>(&p.x));
	*((int *)(buf + 4)) = htonl(*reinterpret_cast<int*>(&p.y));
	*((int *)(buf + 8)) = htonl(*reinterpret_cast<int*>(&v.x));
	*((int *)(buf + 12)) = htonl(*reinterpret_cast<int*>(&v.y));
	*((int *)(buf + 16)) = htonl(*reinterpret_cast<int*>(&mass));
	*((int *)(buf + 20)) = htonl(*reinterpret_cast<int*>(&rad));
	*(buf + 24) = color.r;
	*(buf + 25) = color.g;
	*(buf + 26) = color.b;
	*((int *)(buf + 27)) = htonl(*reinterpret_cast<int *>(&h));
	*((int *)(buf + 31)) = htonl(id);
	return socket.send(buf, 35);
}

bool Object::receive(Socket socket) {
	char *buf = new char[35];
	if(!socket.receive(buf, 35))
		return false;
	p.x = *reinterpret_cast<float*>ntohl(*((int *)(buf)));
	p.y = *reinterpret_cast<float*>ntohl(*((int *)(buf + 4)));
	v.x = *reinterpret_cast<float*>ntohl(*((int *)(buf + 8)));
	v.y = *reinterpret_cast<float*>ntohl(*((int *)(buf + 12)));
	mass =*reinterpret_cast<float*>ntohl(*((int *)(buf + 16)));
	rad = *reinterpret_cast<float*>ntohl(*((int *)(buf + 20)));
	color.r = *(buf + 24);
	color.g = *(buf + 25);
	color.b = *(buf + 26);
	h = *reinterpret_cast<float*>ntohl(*((int *)(buf + 27)));
	id = ntohl(*((int *)(buf + 31)));
	return true;
}
