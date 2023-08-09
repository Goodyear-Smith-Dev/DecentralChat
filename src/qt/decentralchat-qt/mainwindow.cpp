#include "mainwindow.hpp"
#include "ui_decentralchat.h"

MainWindow::MainWindow(QWidget* parent):
	QMainWindow(parent),
	m_UI(new Ui::MainWindow),
	m_Server(new Server),
	m_Client(new Client)
{
	m_UI->setupUi(this);
}

MainWindow::~MainWindow() {
    delete m_UI;
}

