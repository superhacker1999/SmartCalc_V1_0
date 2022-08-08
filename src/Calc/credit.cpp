#include "credit.h"
#include "ui_credit.h"

Credit::Credit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Credit)
{
    ui->setupUi(this);
    connect(ui->button_calc, SIGNAL(clicked()), this, SLOT(calc()));

}

Credit::~Credit()
{
    delete ui;
}

void Credit::calc() {
    QString expression;
    double pay_for_month = 0;
    double overpay = 0;
    double summary_pay = 0;
    double credit_sum = (double)ui->spin_sum->value();
    int month_time;
    double percent = (double)ui->spin_percent->value();
    if (ui->combo_type->currentText() == "Мес.") {
        month_time = ui->spin_time->value();
    } else {
        month_time = ui->spin_time->value() * 12;
    }
    if (ui->radio_anuitet->isChecked() == true) {
        expression = "сумма_кредита*(ПС/(1-(1+ПС)^(0-ПП)))";
        expression.replace("сумма_кредита", QString::number(credit_sum));
        expression.replace("ПС", QString::number(percent/(100*12)));
        expression.replace("ПП", QString::number(month_time));
        QByteArray qa = expression.toLocal8Bit();
        char* array = qa.data();
        get_calculated(array, &pay_for_month);
        summary_pay = pay_for_month * month_time;
        overpay = summary_pay - credit_sum;
        expression = QString::number(pay_for_month, 'f', 2);
        ui->res_pay->setText(expression);
        expression = QString::number(summary_pay, 'f', 2);
        ui->res_sum->setText(expression);
        expression = QString::number(overpay, 'f', 2);
        ui->res_overpay->setText(expression);
    } else {
        double tmp_result;
        double ostatok = credit_sum;
        expression = "S/N+s*p*31/365/100";
        QString tmp_expression = expression;
        double pay_min = 0;
        double pay_max = 0;
        int i = month_time;
        while(i > 0) {
            tmp_expression = expression;
            tmp_expression.replace('S', QString::number(credit_sum));
            tmp_expression.replace('s', QString::number(ostatok));
            tmp_expression.replace('N', QString::number(month_time));
            tmp_expression.replace('p', QString::number(percent));
            QByteArray qa = tmp_expression.toLocal8Bit();
            char* array = qa.data();
            get_calculated(array, &tmp_result);
            summary_pay += tmp_result;
            if (i == month_time) {
                pay_max = tmp_result;
                pay_min = tmp_result;
            }
            if (tmp_result > pay_max) pay_max = tmp_result;
            if (tmp_result < pay_min) pay_min = tmp_result;
            i--;
            ostatok = ostatok - tmp_result;
        }
        overpay = summary_pay - credit_sum;
        expression = QString::number(pay_min, 'f', 2) + " ... " + QString::number(pay_max, 'f', 2);
        ui->res_pay->setText(expression);
        expression = QString::number(summary_pay, 'f', 2);
        ui->res_sum->setText(expression);
        expression = QString::number(overpay, 'f', 2);
        ui->res_overpay->setText(expression);
    }
}
