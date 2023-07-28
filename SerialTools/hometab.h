#ifndef HOMETAB_H
#define HOMETAB_H

#include <QWidget>

namespace Ui {
class HomeTab;
}

class HomeTab : public QWidget
{
    Q_OBJECT

public:
    explicit HomeTab(QWidget *parent = 0);
    ~HomeTab();

private:
    Ui::HomeTab *ui;
};

#endif // HOMETAB_H
