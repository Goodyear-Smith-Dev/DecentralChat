#include "link.hpp"
#include "mainwindow.hpp"
#include "ui_decentralchat.h"

#include <QList>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget* parent):
	QMainWindow(parent),
	ui(new Ui::MainWindow),
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

    //attach this window to all links
    //https://forum.qt.io/post/670111

    QList<Link*> links = this->findChildren<Link*>();
    for (Link* link: links) {
        link->attachTo(m_UI->stackedWidget);
    }
}

MainWindow::~MainWindow() {
    delete m_UI;
}

