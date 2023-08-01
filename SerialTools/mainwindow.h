#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <serialtab.h>
#include <hometab.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    HomeTab *home;
    QList<SerialTab *> serials;
    void resizeEvent(QResizeEvent *event);
public slots:
    void openNewSerialTab(HomeTab::SerialConfig *sc);
};

#endif // MAINWINDOW_H
