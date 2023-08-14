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

#pragma once

#include <QDialog>
#include <Qt>

QT_BEGIN_NAMESPACE
namespace Ui {
	class ChatDialog;
}
QT_END_NAMESPACE

class ChatDialog: public QDialog {
	Q_OBJECT

public:
	explicit ChatDialog(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
	~ChatDialog();

private:
	Ui::ChatDialog* m_ChatDialog;
};