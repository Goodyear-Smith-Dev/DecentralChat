#pragma once
#include <QMainWindow>
#include <QStackedWidget>

enum Page {
    CREATE_ACCOUNT = 0,
    LOGIN,
    MAIN
};

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    QStackedWidget* getPages();

private:
	Ui::MainWindow *ui;
    QStackedWidget *pages;
};
