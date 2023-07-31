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

Client::Client():
	m_TcpSocket(new QTcpSocket(this))
{
	m_ReceivedMessage.setDevice(m_TcpSocket);
    m_ReceivedMessage.setVersion(QDataStream::Qt_6_0);

	connect(m_TcpSocket, &QAbstractSocket::connected, this, [] {
		std::cout << "Connected" << std::endl;
	});
	connect(m_TcpSocket, &QAbstractSocket::readyRead, this, &Client::FetchMessage);
    connect(m_TcpSocket, &QAbstractSocket::errorOccurred, this, [this] {
		std::cout << "Error: " << this->m_TcpSocket->errorString().toStdString() << std::endl;
	});
}

Client::~Client() {
	delete m_TcpSocket;
}

void Client::ConnectToHost(const QHostAddress& address, uint16_t port) {
	m_TcpSocket->abort();
	m_TcpSocket->connectToHost(address, port);
	//m_TcpSocket->waitForConnected();
	//m_TcpSocket->waitForReadyRead();

	// Block the thread until a connection is established and data is ready to be read
	auto lock = std::unique_lock(m_Mutex);
	m_Cv.wait(lock, [&] {
		return m_TcpSocket->waitForConnected() && m_TcpSocket->waitForReadyRead();
	});
	lock.unlock();
}

void Client::FetchMessage() {
	std::cout << "Reading message.." << std::endl;

	m_ReceivedMessage.startTransaction();
	QString message;
	m_ReceivedMessage >> message; // Get the message from the data stream

	if (!m_ReceivedMessage.commitTransaction()) {
		return;
	}

	std::cout << "Message: " << message.toUtf8().toStdString() << std::endl;
}
