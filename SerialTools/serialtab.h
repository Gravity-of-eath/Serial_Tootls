#ifndef SERIALTAB_H
#define SERIALTAB_H

#include <QWidget>

namespace Ui {
class SerialTab;
}

class SerialTab : public QWidget
{
    Q_OBJECT

public:
    explicit SerialTab(QWidget *parent = 0);
    ~SerialTab();

private:
    Ui::SerialTab *ui;
};

#endif // SERIALTAB_H
