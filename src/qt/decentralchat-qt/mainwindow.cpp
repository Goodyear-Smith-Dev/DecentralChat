#include "mainwindow.hpp"
#include "ui_decentralchat.h"

MainWindow::MainWindow(QWidget* parent):
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_Server(new Server)
{
	ui->setupUi(this);

	Client client;
	client.ConnectToHost(m_Server->GetHostname(), m_Server->GetPort());
}

MainWindow::~MainWindow() {
    delete ui;
}

