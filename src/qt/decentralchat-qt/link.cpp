#include "link.hpp"
#include "mainwindow.hpp"

#include <iostream>
#include <QLabel>
#include <QStackedWidget>
#include <QWidget>

Link::Link(QWidget* parent) : QLabel(parent) { }

void Link::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        goTo();
    }
}

Page Link::getPage() {
    return (Page)property(PAGE_PROPNAME).toInt();
}

void Link::setPage(Page value) {
    if (getPage()!=value) {
        setProperty(PAGE_PROPNAME, (int)value);
        update();
    }
}

void Link::setWindow(MainWindow* w) { window = w; }

void Link::goTo() {
    window->getPages()->setCurrentIndex((int)getPage());
}