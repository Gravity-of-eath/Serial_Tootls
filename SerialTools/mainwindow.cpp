#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "hometab.h"
#include "serialtab.h"
# include "QToolButton"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle(tr("SerialTools"));
    home =  new HomeTab(this);
    ui->tabWidget->addTab(home, tr("Home"));
    connect(home,  &HomeTab::onNewSerial,  this, &openNewSerialTab);
}
void MainWindow::resizeEvent(QResizeEvent *event) {
    ui->verticalLayoutWidget->resize(this->width(), this->height());
    ui->tabWidget->resize(this->width(), this->height());
    QListIterator<SerialTab *> iter(serials);
    while (iter.hasNext()) {
        iter.next()->resize(this->width(), this->height() - 40);
    }
}

void MainWindow::openNewSerialTab(HomeTab::SerialConfig *sc) {
    qDebug() << "sc= " << sc ;
    SerialTab *st =  new SerialTab(this, sc);
    ui->tabWidget->addTab(st, (sc->name));
    QToolButton *m_tabClosebutton  = new QToolButton(this);
    m_tabClosebutton->setIcon(QIcon(":/res/close"));
    connect(m_tabClosebutton, &QToolButton::clicked, st, &SerialTab::closeTab);
    serials.append(st);
    ((QTabBar *)(ui->tabWidget->tabBar()))->setTabButton(ui->tabWidget->indexOf(st), QTabBar::RightSide, m_tabClosebutton);
    ui->tabWidget->setCurrentWidget(st);
}
MainWindow::~MainWindow() {
    delete ui;
}
