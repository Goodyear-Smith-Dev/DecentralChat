#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QStackedWidget>

#include "enums.hpp"

using decentralchat::enums::Page;

class Link: public QLabel
{
    Q_OBJECT

public:

    Link(QWidget* parent = nullptr);

    void mouseReleaseEvent(QMouseEvent* event);

	Page getPage() const;
    void setPage(Page value);
    void attachTo(QStackedWidget* p);
    QStackedWidget* getAttached() const;
    void goTo();

private:
    QStackedWidget* pages = nullptr;
	static constexpr const char* PAGE_PROPNAME = "page";
};