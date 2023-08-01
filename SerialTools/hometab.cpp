#include "hometab.h"
#include "ui_hometab.h"
#include "qdebug.h"

#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>

QT_USE_NAMESPACE

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");
HomeTab::HomeTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeTab) {
    ui->setupUi(this);
    intValidator = new QIntValidator(0, 4000000, this);
    connect(ui->openButton, &QPushButton::clicked, [&] {
        SerialConfig *sc = makeCurrentConfig();
        emit onNewSerial(sc);
        qDebug() << "HomeTab onNewSerial" <<  sc;
    });
    connect(ui->refreshButton, &QPushButton::clicked, [&] {
        initView();
    });
    connect(ui->serialPortInfoListBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &HomeTab::showPortInfo);
    connect(ui->baudRateBox,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &HomeTab::checkCustomBaudRatePolicy);
    connect(ui->serialPortInfoListBox,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &HomeTab::checkCustomDevicePathPolicy);
    initView();
}

HomeTab::SerialConfig  *HomeTab::makeCurrentConfig() {
    SerialConfig *sc =  new SerialConfig();
    sc->name = ui->serialPortInfoListBox->currentText();
    if (ui->baudRateBox->currentIndex() == 4) {
        sc->baudRate = ui->baudRateBox->currentText().toInt();
    } else {
        sc->baudRate = static_cast<QSerialPort::BaudRate>(
                           ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }
    sc->stringBaudRate = QString::number(sc->baudRate);
    sc->dataBits = static_cast<QSerialPort::DataBits>(
                       ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    sc->stringDataBits = ui->dataBitsBox->currentText();
    sc->parity = static_cast<QSerialPort::Parity>(
                     ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    sc->stringParity = ui->parityBox->currentText();
    sc->stopBits = static_cast<QSerialPort::StopBits>(
                       ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    sc->stringStopBits = ui->stopBitsBox->currentText();
    sc->flowControl = static_cast<QSerialPort::FlowControl>(
                          ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    sc->stringFlowControl = ui->flowControlBox->currentText();
    sc->localEchoEnabled = ui->localEchoCheckBox->isChecked();
    return sc;
}
void HomeTab::showPortInfo(int idx) {
    if (idx == -1) {
        return;
    }
    QStringList list = ui->serialPortInfoListBox->itemData(idx).toStringList();
    ui->descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    ui->manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    ui->serialNumberLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
    ui->locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
    ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
    ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}
void HomeTab::checkCustomBaudRatePolicy(int idx) {
    bool isCustomBaudRate = !ui->baudRateBox->itemData(idx).isValid();
    ui->baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        ui->baudRateBox->clearEditText();
        QLineEdit *edit = ui->baudRateBox->lineEdit();
        edit->setValidator(intValidator);
    }
}

void HomeTab::checkCustomDevicePathPolicy(int idx) {
    bool isCustomPath = !ui->serialPortInfoListBox->itemData(idx).isValid();
    ui->serialPortInfoListBox->setEditable(isCustomPath);
    if (isCustomPath) {
        ui->serialPortInfoListBox->clearEditText();
    }
}
void HomeTab::initView() {
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(tr("Custom"));
    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);
    ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);
    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);
    ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
    ui->serialPortInfoListBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    QList<QSerialPortInfo>infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);
        ui->serialPortInfoListBox->addItem(list.first(), list);
    }
//    ui->serialPortInfoListBox->addItem(tr("Custom"));
}
void HomeTab::resizeEvent(QResizeEvent *event) {
    qDebug() << "HomeTab event" << event;
}
HomeTab::~HomeTab() {
    delete ui;
}
