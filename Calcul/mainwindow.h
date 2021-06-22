#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_NumButton_clicked(int cif = -1);

    void on_CalcEdit_textChanged(const QString &arg1);

    void on_DotButton_clicked();

    void operations();

    void math_operations();

    void on_ResultButton_clicked();

signals:

protected:
    bool eventFilter(QObject *watched, QEvent *event); //Переопределяем функцию захватчика событий
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
