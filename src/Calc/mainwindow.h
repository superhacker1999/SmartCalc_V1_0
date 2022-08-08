#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <credit.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVector<double> x, y;

private slots:
    void digits_numbers();
    void clear();
    void operations();
    void arithmetic();
    void funcs();
    void calculate();
    void x_enabled();
    void make_graph();
    void start_credit();
};

#endif // MAINWINDOW_H
