#pragma once

#include "mainwindow.hpp"

#include <QLabel>
#include <QMouseEvent>
#include <QStackedWidget>

#define PAGE_PROPNAME "page"

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
};