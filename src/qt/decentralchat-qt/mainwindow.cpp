#include "link.hpp"
#include "mainwindow.hpp"
#include "ui_decentralchat.h"

#include <QList>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget* parent):
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pages = this->findChild<QStackedWidget*>("pages");

    //attach this window to all links
    //https://forum.qt.io/post/670111

    QList<Link*> links = this->findChildren<Link*>();
    foreach (Link* link, links) {
        link->setWindow(this);
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

QStackedWidget* MainWindow::getPages() { return pages; }

