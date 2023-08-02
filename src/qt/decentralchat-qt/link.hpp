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

    void setWindow(MainWindow* w);

    void goTo();

private:
    MainWindow* window;
};