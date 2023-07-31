#include "mainwindow.hpp"
#include "ui_decentralchat.h"

MainWindow::MainWindow(QWidget* parent):
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	Server server;

	Client client;
	client.ConnectToHost(server.GetHostname(), server.GetPort());
}

MainWindow::~MainWindow() {
    delete ui;
}

