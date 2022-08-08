/*
to do:
    1. После нажатия '=' проверить кол-во открывающих и закрывающих скобок
    2. Обработать унарный минус
    3. добавить кнопку "x"
    4.


*/


#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <stdlib.h>
#include <qmath.h>

extern "C" {
    #include "s21_calc.h"
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // digits
    connect(ui->button_0, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->button_1, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->button_2, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->button_3, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->button_4, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->button_5, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->button_6, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->button_7, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->button_8, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->button_9, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->button_dot, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->button_x, SIGNAL(clicked()), this, SLOT(digits_numbers()));

    // clear or delete
    connect(ui->button_clear, SIGNAL(clicked()), this, SLOT(clear()));
    connect(ui->button_delete, SIGNAL(clicked()), this, SLOT(clear()));

    // ()
    connect(ui->button_open, SIGNAL(clicked()), this, SLOT(operations()));
    connect(ui->button_close, SIGNAL(clicked()), this, SLOT(operations()));

    // arithmetic
    connect(ui->button_plus, SIGNAL(clicked()), this, SLOT(arithmetic()));
    connect(ui->button_minus, SIGNAL(clicked()), this, SLOT(arithmetic()));
    connect(ui->button_mult, SIGNAL(clicked()), this, SLOT(arithmetic()));
    connect(ui->button_div, SIGNAL(clicked()), this, SLOT(arithmetic()));
    connect(ui->button_pow, SIGNAL(clicked()), this, SLOT(arithmetic()));

    // funcs
    connect(ui->button_sin, SIGNAL(clicked()), this, SLOT(funcs()));
    connect(ui->button_cos, SIGNAL(clicked()), this, SLOT(funcs()));
    connect(ui->button_tan, SIGNAL(clicked()), this, SLOT(funcs()));
    connect(ui->button_asin, SIGNAL(clicked()), this, SLOT(funcs()));
    connect(ui->button_acos, SIGNAL(clicked()), this, SLOT(funcs()));
    connect(ui->button_atan, SIGNAL(clicked()), this, SLOT(funcs()));
    connect(ui->button_sqrt, SIGNAL(clicked()), this, SLOT(funcs()));
    connect(ui->button_ln, SIGNAL(clicked()), this, SLOT(funcs()));
    connect(ui->button_log, SIGNAL(clicked()), this, SLOT(funcs()));
    connect(ui->button_mod, SIGNAL(clicked()), this, SLOT(funcs()));

    // equal
    connect(ui->button_equal, SIGNAL(clicked()), this, SLOT(calculate()));

    // x_value
    connect(ui->radio_value, SIGNAL(toggled(bool)), this, SLOT(x_enabled()));
    connect(ui->radio_graph, SIGNAL(toggled(bool)), this, SLOT(x_enabled()));

    //graph
    connect(ui->button_make_graph, SIGNAL(clicked()), this, SLOT(make_graph()));

    //credit
    connect(ui->credit_button, SIGNAL(clicked()), this, SLOT(start_credit()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::digits_numbers() {  // добавление чисел и точки в лейбл
    QPushButton *button = (QPushButton *)sender();
    QString result_label = ui->result_show->text();
    if (result_label == "Ошибка ввода") {
        ui->result_show->setText("");
        result_label = ui->result_show->text();
    }
    QByteArray ba = result_label.toLocal8Bit();
    char* char_array = ba.data();
    if (button == ui->button_x) {
        ui->result_show->setText(result_label + "x");
    } else if (button == ui->button_dot && ((!(char_array[result_label.size() - 1] >= '0' && char_array[result_label.size() -1] <= '9')) || (result_label.size() == 0))) {
        ui->result_show->setText(result_label + "0.");
    } else {
        ui->result_show->setText(result_label + button->text());
    }
}

void MainWindow::clear() {  // удаление одного или всех чисел из лейбла
    QPushButton *button = (QPushButton *)sender();
    if (button == ui->button_delete) {
        ui->result_show->setText("");
    } else {
        QString result_label = ui->result_show->text();
        result_label.resize(result_label.size() - 1);
        ui->result_show->setText(result_label);
    }
}

void MainWindow::operations() {
    QPushButton *button = (QPushButton *)sender();
    QString result_label = ui->result_show->text();
    if (result_label == "Ошибка ввода") {
        ui->result_show->setText("");
        result_label = ui->result_show->text();
    }
    if (button == ui->button_open) {  // (
        ui->result_show->setText(result_label + "(");
    } else if (button == ui->button_close) {  // )
        ui->result_show->setText(result_label + ")");
    }
}

void MainWindow::arithmetic() {
    QPushButton *button = (QPushButton *)sender();
    QString result_label = ui->result_show->text();
    if (result_label == "Ошибка ввода") {
        ui->result_show->setText("");
        result_label = ui->result_show->text();
    }
    if (button == ui->button_plus) {
        ui->result_show->setText(result_label + "+");
    } else if (button == ui->button_minus) {
        ui->result_show->setText(result_label + "-");
    } else if (button == ui->button_mult) {
        ui->result_show->setText(result_label + "*");
    } else if (button == ui->button_div) {
        ui->result_show->setText(result_label + "/");
    } else if (button == ui->button_pow) {
        ui->result_show->setText(result_label + "^");
    }
}

void MainWindow::funcs() {
    QPushButton *button = (QPushButton *)sender();
    QString result_label = ui->result_show->text();
    if (result_label == "Ошибка ввода") {
        ui->result_show->setText("");
        result_label = ui->result_show->text();
    }
    if (button == ui->button_sin) {
        ui->result_show->setText(result_label + "sin(");
    } else if (button == ui->button_cos) {
        ui->result_show->setText(result_label + "cos(");
    } else if (button == ui->button_tan) {
        ui->result_show->setText(result_label + "tan(");
    } else if (button == ui->button_asin) {
        ui->result_show->setText(result_label + "asin(");
    } else if (button == ui->button_acos) {
        ui->result_show->setText(result_label + "acos(");
    } else if (button == ui->button_atan) {
        ui->result_show->setText(result_label + "atan(");
    } else if (button == ui->button_sqrt) {
        ui->result_show->setText(result_label + "sqrt(");
    } else if (button == ui->button_mod) {
        ui->result_show->setText(result_label + "mod");
    } else if (button == ui->button_ln) {
        ui->result_show->setText(result_label + "ln(");
    } else if (button == ui->button_log) {
        ui->result_show->setText(result_label + "log(");
    }
}

void MainWindow::calculate() {
    // отправка строки в бэкенд
    QString str = ui->result_show->text();
    if (ui->radio_value->isChecked()) {  // посчитать результат
        if (str.contains('x')) {
            str.replace("x", ui->x_value->toPlainText());
        }
        if (str.contains("(-")) {
            str.replace("(-", "(0-");
        }
        QByteArray ba = str.toLocal8Bit();
        char* char_array = ba.data();
        double result = 0;
        if (str == "") {
            ui->result_show->setText("Ошибка ввода");
        } else {
            if (get_calculated(char_array, &result)  == 1) {
               ui->result_show->setText("Ошибка ввода");
            } else {
                QString answer = QString::number(result, 'g', 10);
                ui->result_show->setText(answer);
            }
        }
    }
}

void MainWindow::x_enabled() {
    QRadioButton *button = (QRadioButton*)sender();
    if (button == ui->radio_graph) {
        ui->x_value->setEnabled(0);
        ui->button_make_graph->setEnabled(1);
        this->setFixedWidth(957);
        this->setFixedHeight(629);
        ui->spin_max->setEnabled(true);
        ui->spin_min->setEnabled(true);
    } else {
        ui->x_value->setEnabled(1);
        ui->button_make_graph->setEnabled(0);
        this->setFixedWidth(526);
        this->setFixedHeight(629);
        ui->spin_max->setEnabled(false);
        ui->spin_min->setEnabled(false);
    }
}

void MainWindow::make_graph() {
    QString str = ui->result_show->text();
    if (str.contains("x") == false) {
          ui->result_show->setText("Expression doesn't contain x");
    } else {
        x.clear();
        y.clear();
        QString str = ui->result_show->text();
        int Xmin = ui->spin_min->value();
        int Xmax = ui->spin_max->value();
        double step = 0.005 * (qFabs(Xmin) + qFabs(Xmax));
        double Y;
        double X = (double)Xmin;
        double Ymin = 0;
        double Ymax = 0;
        while (X < (double)Xmax) {
            x.push_back(X);
            str = ui->result_show->text();
            str.replace("x", QString::number(X));
            QByteArray ba = str.toLocal8Bit();
            char* char_array = ba.data();
            get_calculated(char_array, &Y);
            if (X == Xmin) {
               Ymin = Y;
               Ymax = Y;
            }
            if (Y < Ymin) Ymin = Y;
            if (Y > Ymax) Ymax = Y;
            y.push_back(Y);
            X += step;
        }
        ui->widget->addGraph();
        ui->widget->graph(0)->data()->clear();
        ui->widget->graph(0)->addData(x,y);
        ui->widget->xAxis->setRange(Xmin, Xmax);
        ui->widget->yAxis->setRange(Ymin, Ymax);
        ui->widget->replot();
    }
}

void MainWindow::start_credit() {
    Credit window;
    window.setModal(true);
    window.show();
    window.exec();
}
