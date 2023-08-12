#include "client.hpp"
#include "link.hpp"
#include "mainwindow.hpp"
#include "server.hpp"
#include "ui_decentralchat.h"

#include <QList>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget* parent):
	QMainWindow(parent),
	m_UI(new Ui::MainWindow),
	m_Server(new Server),
	m_Client(new Client)
{
    m_UI->setupUi(this);

    //check for account file

    if (0) { //placeholder for check
        //account file found
        m_UI->stackedWidget->setCurrentIndex(static_cast<int>(Page::LOGIN));
    }
    else {
        //no account file found
        m_UI->stackedWidget->setCurrentIndex(static_cast<int>(Page::CREATE_ACCOUNT));
    }

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

