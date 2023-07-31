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

Server::Server():
	m_TcpServer(new QTcpServer(this))
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

	std::cout << "The server is running on IP: " << m_HostAddress.toString().toStdString() << " Port: " << m_Port << std::endl;

	QString message = "Hello, World!";
	connect(m_TcpServer, &QTcpServer::newConnection, this, [=, &message] {
		this->SendMessage(message);
	});

	if (!m_TcpServer->listen(m_HostAddress, m_Port)) {
		std::cout << "Unable to start the server. Error: " << m_TcpServer->errorString().toStdString() << std::endl;
	}
	else {
		std::cout << "Listening..." << std::endl;
	}
}

Server::~Server() {
	delete m_TcpServer;
}

void Server::SendMessage(const QString& message) {
	std::cout << "Sending message..." << std::endl;

	QByteArray block;
    QDataStream dataStream(&block, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_6_5);
	dataStream << message; // Load the message into the data stream

	QTcpSocket* clientConnection = m_TcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);

	clientConnection->write(block);
    clientConnection->disconnectFromHost();
}