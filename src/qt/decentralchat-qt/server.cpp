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
// Filename: server.cpp
// Description: A TCP server
//

#include "server.hpp"

Server::Server(QObject* parent):
	QTcpServer(parent)
{
    const QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    // Use the first non-localhost IPv4 address
    for (const QHostAddress& entry: ipAddressesList) {
        if (entry != QHostAddress::LocalHost && entry.toIPv4Address()) {
            m_HostAddress = entry;
            break;
        }
    }

    // If we did not find one, use IPv4 localhost
    if (m_HostAddress.isNull()) {
        m_HostAddress = QHostAddress(QHostAddress::LocalHost);
	}

	m_Port = 16000;//m_TcpServer->serverPort();

	std::cout << "The server is running on IP: " << m_HostAddress.toString().toStdString() << " Port: " << m_Port << "\n";

	connect(this, &QTcpServer::newConnection, this, [] {
		std::cout << "Connection made" << "\n";
	});
	connect(this, &QTcpServer::pendingConnectionAvailable, this, [] {
		std::cout << "New connection available" << "\n";
	});

	if (!listen(m_HostAddress, m_Port)) {
		std::cout << "Unable to start the server. Error: " << errorString().toStdString() << "\n";
	}
	else {
		std::cout << "Listening..." << "\n";
	}
}

Server::~Server() {
	delete m_TcpServer;
}

void Server::incomingConnection(qintptr socketDescriptor) {
	// Create a new socket for each incoming connection
    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    // Read incoming data
    connect(socket, &QTcpSocket::readyRead, this, [this, &socket]() {
        QByteArray data = socket->readAll();
        QString message = QString::fromUtf8(data);
        emit MessageReceived(message);
    });
}