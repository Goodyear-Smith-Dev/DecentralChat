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
// Filename: chat-dialog.hpp
// Description: The dialog for starting a new chat on DecentralChat
//

#include "chat-dialog.hpp"

#include "ui_createchat.h"

ChatDialog::ChatDialog(const std::vector<std::string>& searchList, QWidget* parent, Qt::WindowFlags flags):
	QDialog(parent, flags),
	m_ChatDialog(new Ui::ChatDialog),
	m_Trie(new Trie(searchList))
{
	m_ChatDialog->setupUi(this);

	setListContent(searchList);

	connect(m_ChatDialog->search, &QTextEdit::textChanged, this, [=] {
		auto matches = m_Trie->findMatches(this->m_ChatDialog->search->toPlainText().toStdString());
		this->setListContent(matches);
	});
}

ChatDialog::~ChatDialog() {
	delete m_ChatDialog;
	delete m_Trie;
}

void ChatDialog::setListContent(const std::vector<std::string>& content) {
	if (m_ChatDialog->friendsList->count() > 0) {
		m_ChatDialog->friendsList->clear();
	}

	if (content.empty()) {
		return;
	}

	for (const auto& item: content) {
		m_ChatDialog->friendsList->addItem(QString::fromStdString(item));
	}
}