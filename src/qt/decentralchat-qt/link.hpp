#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QStackedWidget>

#include "mainwindow.hpp"
#include "decentralchat.hpp"

using decentralchat::Page;

class Link: public QLabel
{
    Q_OBJECT

public:

    Link(QWidget* parent = nullptr);

    void mouseReleaseEvent(QMouseEvent* event);

	Page getPage();
    void setPage(Page value);
    void attachTo(QStackedWidget* p);
    void goTo();

private:
    QStackedWidget* pages;
	static constexpr const char* PAGE_PROPNAME = "page";
};