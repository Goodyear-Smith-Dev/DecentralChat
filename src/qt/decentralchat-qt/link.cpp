#include "link.hpp"
#include "mainwindow.hpp"

#include <QLabel>
#include <QStackedWidget>
#include <QWidget>

Link::Link(QWidget* parent):
	QLabel(parent)
{}

void Link::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        goTo();
    }
}

Page Link::getPage() {
    return static_cast<Page>(property(PAGE_PROPNAME).toInt());
}

void Link::setPage(Page value) {
    if (getPage() != value) {
        setProperty(PAGE_PROPNAME, static_cast<int>(value));
        update();
    }
}

void Link::attachTo(QStackedWidget* p) {
	pages = p;
}

void Link::goTo() {
    pages->setCurrentIndex(static_cast<int>(getPage()));
}