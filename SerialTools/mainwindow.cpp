#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "hometab.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->tabWidget->addTab(new HomeTab(this), tr("Home"));
    connect(ui->openButton, &QPushButton::clicked, [&] {
        ui->tabWidget->addTab(new HomeTab(this), tr("Home"));
    });
}
void MainWindow::resizeEvent(QResizeEvent *event) {
    ui->verticalLayoutWidget->resize(this->width(), this->height());
    ui->tabWidget->resize(this->width(), this->height());
    qDebug() << "event" << event;
}
MainWindow::~MainWindow() {
    delete ui;
}
