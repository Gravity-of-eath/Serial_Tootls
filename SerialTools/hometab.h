#ifndef HOMETAB_H
#define HOMETAB_H

#include <QWidget>

#include <QtSerialPort/QSerialPort>

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui {
    class HomeTab;
}
class QIntValidator;

class HomeTab : public QWidget {
    Q_OBJECT

public:
    struct SerialConfig {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };
    explicit HomeTab(QWidget *parent = 0);
    ~HomeTab();

signals:
    onNewSerial(SerialConfig *sc);
private:
    Ui::HomeTab *ui;
    QIntValidator *intValidator;
    void resizeEvent(QResizeEvent *event);
    SerialConfig *makeCurrentConfig();
    void initView();
    void  showPortInfo(int idx);
    void  checkCustomDevicePathPolicy(int idx);
    void  checkCustomBaudRatePolicy(int idx);

};

#endif // HOMETAB_H
