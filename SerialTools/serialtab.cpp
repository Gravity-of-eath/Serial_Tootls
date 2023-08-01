#include "serialtab.h"
#include "ui_serialtab.h"
#include "qdebug.h"
#include <QMessageBox>

SerialTab::SerialTab(QWidget *parent, HomeTab::SerialConfig *con):
    QWidget(parent),
    ui(new Ui::SerialTab) {
    ui->setupUi(this);
    this->config = con;
    console = new Console;
    console->setEnabled(false);
    ui->verticalLayout->addWidget(console);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::red);
    p.setColor(QPalette::Text, Qt::white);
    statuslabel = new QLabel(this);
    statuslabel->setPalette(p);
    ui->verticalLayout->addWidget(statuslabel);
    openSerial();
    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SerialTab::handleError);
    connect(serial, &QSerialPort::readyRead, this, &SerialTab::readData);
    connect(console, &Console::getData, this, &SerialTab::writeData);
}
void SerialTab::resizeEvent(QResizeEvent *event) {
    QSize sz = event->size();
    ui->verticalLayoutWidget->resize(this->width() - 20, this->height() - 40);
    console->resize(ui->verticalLayoutWidget->width(), ui->verticalLayoutWidget->height() - 20);
//    ui->statuslabel->move(20, console->height() + 15);
    qDebug() << "SerialTab event" << sz;
}

void SerialTab::openSerial() {
    serial = new QSerialPort(this);
    serial->setPortName(config->name);
    serial->setBaudRate(config->baudRate);
    serial->setDataBits(config->dataBits);
    serial->setParity(config->parity);
    serial->setStopBits(config->stopBits);
    serial->setFlowControl(config->flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        console->setEnabled(true);
        console->setLocalEchoEnabled(config->localEchoEnabled);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(config->name).arg(config->stringBaudRate).arg(config->stringDataBits)
                          .arg(config->stringParity).arg(config->stringStopBits).arg(config->stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
        showStatusMessage(tr("Open error"));
    }
}

void SerialTab::showStatusMessage(const QString &message) {
    statuslabel->setText(message);
}

void SerialTab::writeData(const QByteArray &data) {
    if (serial->isOpen()) {
        serial->write(data);
    } else {
        showStatusMessage(tr("Serial is closed!"));
    }
}

void SerialTab::readData() {
    QByteArray data = serial->readAll();
    console->putData(data);
}

void SerialTab::handleError(QSerialPort::SerialPortError error) {
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeTab();
    }
}
SerialTab::~SerialTab() {
    delete ui;
}

void SerialTab::putData(const QByteArray &data) {
}

void SerialTab::closeTab() {
    if (serial->isOpen()) {
        serial->close();
    }
    deleteLater();
    qDebug() << "closeTabs";
}
