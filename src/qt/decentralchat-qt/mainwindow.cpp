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
// Filename: mainwindow.cpp
// Description: The main application window
//

#include <QList>
#include <QStackedWidget>

#include "account.hpp"
#include "client.hpp"
#include "link.hpp"
#include "mainwindow.hpp"
#include "server.hpp"
#include "chat-dialog.hpp"

#include "ui_decentralchat.h"

#define LOCAL_ACCT_FILE "local.acct"

MainWindow::MainWindow(QWidget* parent):
	QMainWindow(parent),
	m_UI(new Ui::MainWindow),
	m_Server(new Server),
	m_Client(new Client)
{
    m_UI->setupUi(this);

    //connect slots to signals
    connect(m_UI->p_cA_createButton, &QPushButton::released, this, &MainWindow::handleCreateAccount);
    connect(m_UI->p_login_loginButton, &QPushButton::released, this, &MainWindow::handleLogin);
	connect(m_UI->p_main_addDest, &QPushButton::released, this, [] {
		// Populate the friends list with temporary values
		ChatDialog({"hello", "head", "apple"}).exec();
	});

    //check for account file
    decentralchat::accounts::Account* acct = nullptr;
    QFile localAcct(LOCAL_ACCT_FILE);
    if (QFile::exists(LOCAL_ACCT_FILE) && (acct = decentralchat::accounts::fromBytes(localAcct)) != nullptr) {
        //account file found
        m_UI->p_login_nameInput->setText(acct->getName());
        m_UI->stackedWidget->setCurrentIndex(static_cast<int>(Page::LOGIN));
    }
    else {
        //no account file found
        m_UI->stackedWidget->setCurrentIndex(static_cast<int>(Page::CREATE_ACCOUNT));
    }
    delete acct;

    //attach settings links for category navigation

    QList<Link*> sttnLinks = m_UI->p_sttn_categoryLinks->findChildren<Link*>();
    for (Link* link: sttnLinks) {
        if (link->getAttached() == nullptr) {
            link->attachTo(m_UI->p_sttn_categories); //attach to the QStackedWidget of settings categories
        }
    }


    //https://forum.qt.io/post/670111
    //attach this window to all unattached links

    QList<Link*> links = this->findChildren<Link*>();
    for (Link* link: links) {
        if (link->getAttached() == nullptr) {
            link->attachTo(m_UI->stackedWidget); //attach to the QStackedWidget for the entire window
        }
    }

}

MainWindow::~MainWindow() {
    delete m_UI;
	delete m_Server;
	delete m_Client;
}

void MainWindow::handleCreateAccount() {
    //get values
    QString name = m_UI->p_cA_nameInput->text();
    QString password = m_UI->p_cA_passwordInput->text();


    //reprompt if invalid
    if (name.isEmpty()) {
        m_UI->p_cA_invalid->setText("Account Name is required.");
        return;
    }
    //TODO check name for uniqueness across accounts (??)

    //clear all inputs
    m_UI->p_cA_invalid->clear();
    m_UI->p_login_invalid->clear();
    m_UI->p_login_nameInput->clear();
    m_UI->p_login_passwordInput->clear();

    decentralchat::accounts::Account* acct = decentralchat::accounts::create(name, "", password);
    QFile localAcct(LOCAL_ACCT_FILE); //TODO find a better place to put this than right next to the exe
    localAcct.open(QFile::WriteOnly);
    localAcct.write(acct->toBytes());
    delete acct;
    localAcct.close();

    m_UI->stackedWidget->setCurrentIndex(static_cast<int>(Page::MAIN));

}

void MainWindow::handleLogin() {
    QString name = m_UI->p_login_nameInput->text();
    QString password = m_UI->p_login_passwordInput->text();

    if (QFile::exists(LOCAL_ACCT_FILE)) {
        QFile localAcct(LOCAL_ACCT_FILE);
        decentralchat::accounts::Account* acct = decentralchat::accounts::fromBytes(localAcct);
        if (acct==nullptr) {
            //go to create account page, reprompt
            m_UI->p_cA_invalid->setText("No account found. Consider creating a new account.");
            m_UI->stackedWidget->setCurrentIndex(static_cast<int>(Page::CREATE_ACCOUNT));
            return;
        }
        decentralchat::accounts::DecryptedData decrypted;
        if (!(acct->getName() == name && acct->decrypt(password.toStdString(), decrypted))) {
            m_UI->p_login_invalid->setText("Invalid Account Name or Password.");
            return;
        }
    }

    //clear all inputs
    m_UI->p_cA_invalid->clear();
    m_UI->p_login_invalid->clear();
    m_UI->p_login_nameInput->clear();
    m_UI->p_login_passwordInput->clear();

    m_UI->stackedWidget->setCurrentIndex(static_cast<int>(Page::MAIN));
}