#include "serialtab.h"
#include "ui_serialtab.h"

SerialTab::SerialTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialTab)
{
    ui->setupUi(this);
}

SerialTab::~SerialTab()
{
    delete ui;
}
