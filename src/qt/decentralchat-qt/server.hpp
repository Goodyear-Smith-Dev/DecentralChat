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
// Filename: server.hpp
// Description: A TCP server
//

#pragma once

#include <iostream>

#include <QAbstractSocket>
#include <QByteArray>
#include <QDataStream>
#include <QHostAddress>
#include <QIODevice>
#include <QList>
#include <QNetworkInterface>
#include <QString>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

class Server: public QTcpServer {
	Q_OBJECT

public:
	explicit Server(QObject* parent = nullptr);
	~Server();

	inline const QHostAddress& getHostname() const;
	inline uint16_t getPort() const;

private:
	QTcpServer* m_TcpServer;
	QTcpSocket* m_CurrentSocket = new QTcpSocket();

	QHostAddress m_HostAddress;
	uint16_t m_Port;
	QString m_IncomingData;

signals:
	void messageReceived(const QString& message);
};

inline const QHostAddress& Server::getHostname() const {
	return m_HostAddress;
}

inline uint16_t Server::getPort() const {
	return m_Port;
}
