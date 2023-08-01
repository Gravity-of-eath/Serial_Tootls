#ifndef SERIALTAB_H
#define SERIALTAB_H

#include <QWidget>
#include <hometab.h>
#include <console.h>
#include <qlabel.h>

namespace Ui {
    class SerialTab;
}

class SerialTab : public QWidget {
    Q_OBJECT

public:
    explicit  SerialTab(QWidget *parent = 0, HomeTab::SerialConfig *config = NULL);
    ~SerialTab();
    void putData(const QByteArray &data);

public slots:
    void closeTab();

private:
    Ui::SerialTab *ui;
    void resizeEvent(QResizeEvent *event);
    void openSerial();
    void showStatusMessage(const QString &message);
    HomeTab::SerialConfig *config;
    QSerialPort *serial;
    Console *console;
    void writeData(const QByteArray &data);
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    QLabel *statuslabel;

signals:
    void getData(const QByteArray &data);
};

#endif // SERIALTAB_H
