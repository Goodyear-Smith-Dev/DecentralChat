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
#include <thread>
#include <chrono>

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

class Server: public QTcpServer {
	Q_OBJECT

public:
	explicit Server(QObject* parent = nullptr);
	~Server();

	inline const QHostAddress& GetHostname() const;
	inline uint16_t GetPort() const;

protected:
	void incomingConnection(qintptr socketDescriptor) override;

private:
	QTcpServer* m_TcpServer;
	QHostAddress m_HostAddress;
	uint16_t m_Port;

signals:
	void MessageReceived(const QString& message);
};

inline const QHostAddress& Server::GetHostname() const {
	return m_HostAddress;
}

inline uint16_t Server::GetPort() const {
	return m_Port;
}
