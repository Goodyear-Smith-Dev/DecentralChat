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

	m_Port = 16000;

	std::cout << "The server is running on IP: " << m_HostAddress.toString().toStdString() << " Port: " << m_Port << "\n";

	connect(this, &QTcpServer::newConnection, this, [this] {
		m_CurrentSocket = nextPendingConnection();
		std::cout << "Connection made with " << m_CurrentSocket->peerAddress().toString().toStdString() << "\n";

		// Read data from the socket
		connect(m_CurrentSocket, &QTcpSocket::readyRead, this, [this]() {
			m_IncomingData += QString::fromUtf8(m_CurrentSocket->readAll());
			qDebug() << m_IncomingData;
			while (m_IncomingData.contains("\n")) {
				int index = m_IncomingData.indexOf('\n');
				QString message = m_IncomingData.left(index);
				m_IncomingData = m_IncomingData.mid(index + 1);
				std::cout << "Received message: " << message << "\n";
			}

			if (!m_IncomingData.isEmpty()) {
				std::cout << "Received message: " << m_IncomingData.toStdString() << "\n";
				m_IncomingData.clear();
			}

			// NOTE: Altertnate (simpler) method
			// QByteArray data = m_CurrentSocket->readAll();
			// QString message = QString::fromUtf8(data);
			// std::cout << message.toStdString() << "\n";
		});

		qDebug() << m_CurrentSocket->peerAddress().toString(); // The peer address should now be available.
	});

	if (!listen(m_HostAddress, m_Port)) {
		std::cout << "Unable to start the server. Error: " << errorString().toStdString() << "\n";
	}
	else {
		std::cout << "Listening...\n";
	}
}

Server::~Server() {
	delete m_TcpServer;
}