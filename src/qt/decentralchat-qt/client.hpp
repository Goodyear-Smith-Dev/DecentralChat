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

#pragma once

#include <iostream>
#include <mutex>
#include <condition_variable>

#include <QAbstractSocket>
#include <QDataStream>
#include <QIODevice>
#include <QString>
#include <QObject>
#include <QTcpSocket>

class Client: public QObject {
	Q_OBJECT

public:
	explicit Client(QObject* parent = nullptr);
	~Client();

	void ConnectToHost(const QHostAddress& address, uint16_t port);
	void SendMessage(const QString& message);

private:
	QTcpSocket* m_TcpSocket;
    QDataStream m_ReceivedMessage;

	std::mutex m_Mutex;
	std::condition_variable m_Cv;
};