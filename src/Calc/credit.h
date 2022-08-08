#ifndef CREDIT_H
#define CREDIT_H

#include <QDialog>
#include <s21_calc.h>

namespace Ui {
class Credit;
}

class Credit : public QDialog
{
    Q_OBJECT

public:
    explicit Credit(QWidget *parent = nullptr);
    ~Credit();

private:
    Ui::Credit *ui;

private slots:
    void calc();
};

#endif // CREDIT_H
