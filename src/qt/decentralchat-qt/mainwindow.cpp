#include "mainwindow.hpp"
#include "ui_decentralchat.h"

MainWindow::MainWindow(QWidget* parent):
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_Server(new Server),
	m_Client(new Client)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

