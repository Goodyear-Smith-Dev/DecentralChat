//
// This file is part of DecentralChat
// Copyright (C) 2023 Ryan Smith, Jake Goodyear
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or any
// later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>
//
// Filename: client.hpp
// Description: A TCP client connection
//

#include "client.hpp"

Client::Client(QObject* parent):
	QObject(parent),
	m_TcpSocket(new QTcpSocket(this))
{
    connect(m_TcpSocket, &QAbstractSocket::errorOccurred, this, [this] {
		std::cout << "Error: " << m_TcpSocket->errorString().toStdString() << "\n";
	});
}

Client::~Client() {
	delete m_TcpSocket;
}

void Client::ConnectToHost(const QHostAddress& address, uint16_t port) {
	connect(m_TcpSocket, &QAbstractSocket::connected, this, [this]() mutable {
		std::cout << "Connected\n";
		SendMessage("Hello, World!\n");
	});


	if (m_TcpSocket->state() != QAbstractSocket::ConnectedState) {
		m_TcpSocket->connectToHost(address, port);
	}
}

void Client::SendMessage(const std::string& message) {
	if (m_TcpSocket->state() != QAbstractSocket::ConnectedState) {
		return;
	}

	std::cout << "Sending message...\n";
	const char* strData = message.c_str();
	m_TcpSocket->write(strData, std::strlen(strData));
}
