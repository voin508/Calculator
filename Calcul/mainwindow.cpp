#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QMessageBox>
#include <QKeyEvent>

QChar div_sign = QChar(247);  //Символ деления в UTF
QChar mult_sign = QChar(215); //Символ умножения в UTF

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->DivideButton->setText(div_sign); // Устанавливаем знак деления для кнопки DivideButton
    ui->MultButton->setText(mult_sign);   // Устанавливаем знак умножения для кнопки MultButton
    ui->CalcEdit->setStyleSheet("QLineEdit { color: white; background-color: #1d6899 }"); //Устанавливаем цвет для строки ввода и для текста
    qApp->installEventFilter(this); //События главного окна будем перехватывать


    connect(ui->Num0Button, SIGNAL(clicked()), this, SLOT(on_NumButton_clicked()));
    connect(ui->Num1Button, SIGNAL(clicked()), this, SLOT(on_NumButton_clicked()));
    connect(ui->Num2Button, SIGNAL(clicked()), this, SLOT(on_NumButton_clicked()));
    connect(ui->Num3Button, SIGNAL(clicked()), this, SLOT(on_NumButton_clicked()));
    connect(ui->Num4Button, SIGNAL(clicked()), this, SLOT(on_NumButton_clicked()));
    connect(ui->Num5Button, SIGNAL(clicked()), this, SLOT(on_NumButton_clicked()));
    connect(ui->Num6Button, SIGNAL(clicked()), this, SLOT(on_NumButton_clicked()));
    connect(ui->Num7Button, SIGNAL(clicked()), this, SLOT(on_NumButton_clicked()));
    connect(ui->Num8Button, SIGNAL(clicked()), this, SLOT(on_NumButton_clicked()));
    connect(ui->Num9Button, SIGNAL(clicked()), this, SLOT(on_NumButton_clicked()));
    connect(ui->PlusMinusButton, SIGNAL(clicked()), this, SLOT(operations()));
    connect(ui->PercentButton, SIGNAL(clicked()), this, SLOT(operations()));
    connect(ui->PlusButton, SIGNAL(clicked()), this, SLOT(math_operations()));
    connect(ui->MinusButton, SIGNAL(clicked()), this, SLOT(math_operations()));
    connect(ui->MultButton, SIGNAL(clicked()), this, SLOT(math_operations()));
    connect(ui->DivideButton, SIGNAL(clicked()), this, SLOT(math_operations()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

// Клавиатура
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    //Если объект события CalcEdit, а событие - нажатая клавиша
    if (watched == ui->CalcEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        
        //Если была нажата цифра, математические знаки, процент, backspace или delete
        if ((keyEvent->key() >= Qt::Key_0 && keyEvent->key() <= Qt::Key_9) ||
                keyEvent->key() == Qt::Key_Slash ||
                keyEvent->key() == Qt::Key_Backspace ||
                keyEvent->key() == Qt::Key_Delete ||
                keyEvent->key() == Qt::Key_Minus ||
                keyEvent->key() == Qt::Key_Plus ||
                keyEvent->key() == Qt::Key_Asterisk ||
                keyEvent->key() == Qt::Key_Percent ||
                keyEvent->key() == Qt::Key_Return ||
                keyEvent->key() == Qt::Key_Equal ||
                keyEvent->key() == Qt::Key_Period)
        {
            //Если введен знак деления "/", на экране должен появиться особый символ div_sign
            if (keyEvent->key() == Qt::Key_Slash)
            {
                ui->DivideButton->click();
                return true; //Обработали событие
            }
            // Если ввели "*", на экране должен появиться особый символ mult_sign
            else if (keyEvent->key() == Qt::Key_Asterisk)
            {
                ui->MultButton->click();
                return true; //Обработали событие
            }
            //Если ввели "%"
            else if (keyEvent->key() == Qt::Key_Percent)
            {
                ui->PercentButton->click();
                return true; //Обработали событие
            }
            //Если ввели "-"
            else if (keyEvent->key() == Qt::Key_Minus)
            {
                ui->MinusButton->click();
                return true; //Обработали событие
            }
            //Если ввели "+"
            else if (keyEvent->key() == Qt::Key_Plus)
            {
                ui->PlusButton->click();
                return true; //Обработали событие
            }
            //Если нажали Enter или "="
            else if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Equal)
            {
                ui->ResultButton->click();
                return true; //Обработали событие
            }
            //Если это цифры, тогда вызываем функцию-обработчик вставки цифр
            else if (keyEvent->key() >= Qt::Key_0 && keyEvent->key() <= Qt::Key_9)
            {
                on_NumButton_clicked(keyEvent->key() - 48);
                return true; //Обработали событие
            }
            //Если это точка "."
            else if (keyEvent->key() == Qt::Key_Period)
            {
                on_DotButton_clicked();
                return true; //Обработали событие
            }
        }
        else
            return true; //Иначе если это отличающийся символ, передаем окну, что это событие было обработано
    }
    return false;
}


void MainWindow::on_DotButton_clicked()
{
    QString str(ui->CalcEdit->text());
    if (str.size() != 0) //Точка не может быть первым символом в числе
    {
        int curs_pos = ui->CalcEdit->cursorPosition(); //Получаем позицию курсора
        int flag = 1;
        int i = curs_pos - 1;

        //Пока не встретили мат.знак или начало строки проверяем, есть ли уже в числе точка
        while (i > 0)
        {
            if (str[i] == '.')
            {
                flag = 0;
                break;
            }
            else if (str[i] == mult_sign || str[i] == div_sign || str[i] == '-' || str[i] == '+')
            {
                if (curs_pos - i == 1)
                    flag = 0;
                break;
            }
            i--;
        }
        i = curs_pos;
        // Проверяем справа от курсора, нет ли в нашем числе уже точки
        while (i < str.size() && flag)
        {
            if (str[i] == '.')
            {
                flag = 0;
                break;
            }
            else if (str[i] == mult_sign || str[i] == div_sign || str[i] == '-' || str[i] == '+')
            {
                if (i - curs_pos == 1)
                    flag = 0;
                break;
            }
            i++;
        }
        if (flag)
            ui->CalcEdit->insert(".");
    }
}

void MainWindow::on_NumButton_clicked(int cif)
{
    QString num;
    QString str(ui->CalcEdit->text());


    if (cif == -1) //Если цифра была напечатана при нажатии одной из pushButton
    {
        QPushButton *button = (QPushButton *)sender(); //Получаем указатель на кнопку-отправитель сигнала
        num = button->text();
    }
    else //Иначе если цифра была напечатана с клавиатуры
    {
        num = QString::number(cif);
    }


    if (str.size() == 0)
    {
        ui->CalcEdit->insert(num);
        return;
    }
    int curs_pos = ui->CalcEdit->cursorPosition();
    int cnt = 0; //Счетчик цифр в числе
    int fract_cnt = 0; //Счетчик цифр в дробной части
    int flag = 1;
    bool fract = false; //true - место, куда хотим вставить цифру, это дробная часть,
                        //false - место, куда хотим вставить цифру, это целая часть

    // Посимвольно проверяем слева от позиции курсора
    int i = curs_pos;
    while (i >= 0)
    {
        if (cnt == 15) // Если цифр в числе уже 15
        {
            flag = 0;
            QMessageBox::about(this, NULL, "В числе не может быть более 15 цифр!");
            break;
        }
        else if (str[i] == '.')
        {
            fract = true;
            if (cnt == 10) // Если в место после запятой, куда хотим вставить цифру, уже есть 10 цифр
            {
                QMessageBox::about(this, NULL, "После запятой может быть только 10 чисел!");
                flag = 0;
            }
            fract_cnt = cnt;
            i--;
            continue;
        }
        else if (str[i] == mult_sign || str[i] == div_sign || str[i] == '-' || str[i] == '+') // Мат.знаки отделяют число
        {
            break;
        }
        cnt++;
        i--;
    }
    i = curs_pos + 1;

    // Посимвольно проверяем справа от позиции курсора
    while (i < str.size() && flag)
    {
        if (fract_cnt == 10 && fract) //Уже есть 10 цифр после запятой
        {
            QMessageBox::about(this, NULL, "После запятой может быть только 10 чисел!");
            flag = 0;
        }
        if (cnt == 15 || (cnt == 14 && fract))
        {
            flag = 0;
            QMessageBox::about(this, NULL, "В числе не может быть более 15 цифр!");
            break;
        }
        else if (str[i] == mult_sign || str[i] == div_sign || str[i] == '-' || str[i] == '+') //Мат.знаки отделяют число
        {
            break;
        }
        if (fract)
            fract_cnt++;
        if (str[i] != '.')
            cnt++;
        i++;
    }
    //qInfo() << cnt;
    if (flag)
        ui->CalcEdit->insert(num);

}

void MainWindow::operations()
{
    QPushButton *button = (QPushButton *)sender();

    double all_numbers;
    QString new_text;

    on_ResultButton_clicked(); //Если пользователь ввел какое-то выражение, но не посчитал, тогда для начала его стоит посчитать
    if (button->text() == "+/-")
    {
        all_numbers = ui->CalcEdit->text().toDouble();
        all_numbers *= -1;
        new_text = QString::number(all_numbers, 'g', 15);
        ui->CalcEdit->setText(new_text);
    }
    else if (button->text() == "%")
    {
        all_numbers = ui->CalcEdit->text().toDouble();
        all_numbers *= 0.01;
        new_text = QString::number(all_numbers, 'g', 15);
        ui->CalcEdit->setText(new_text);
    }
}

void MainWindow::math_operations()
{
    QPushButton *button = (QPushButton *)sender();

    QString oper = button->text(); //Символ операции
    QString str(ui->CalcEdit->text()); //Строка в калькуляторе
    int curs_pos = ui->CalcEdit->cursorPosition();

    if (curs_pos == 0)
        return;

    if (curs_pos != str.size())
    {
        //Если мат.знак находится не в конце строки, тогда справа и слева должны быть цифры, чтобы вставить знак
        if (!(str.size() == 0) && str[curs_pos - 1] <= '9' && str[curs_pos - 1] >= '0' && str[curs_pos] <= '9' && str[curs_pos] >= '0')
            ui->CalcEdit->insert(oper);
    }
    else if (!(str.size() == 0) && str[curs_pos - 1] <= '9' && str[curs_pos - 1] >= '0') // Если мат.знак в конце строки, слева должна быть цифра
        ui->CalcEdit->insert(oper);
}

void MainWindow::on_ResultButton_clicked()
{
    QString str(ui->CalcEdit->text());         //Строка в калькуляторе
    QVector<QString> str_nums (150);           //Вектор строк, содержащих числа в выражении
    QList<double> nums;                        //Список преобразованных в double чисел в выражении
    int cnt = 0;                               //Количество чисел в выражении
    QString opers;                             //Задает порядок операций

    if (str.size() == 0)
        return;

    //Если пользователь ввел операцию в конце строки или точку
    if (str.back() == div_sign || str.back() == mult_sign || str.back() == '+' || str.back() == '-' || str.back() == '.')
    {
        str.remove(str.size() - 1, 1);
    }
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == div_sign || str[i] == mult_sign || (str[i] == '+' && str[i - 1] != 'e') || str[i] == '-')
        {
            opers.push_back(str[i]);
            cnt++;
        }
        else
            str_nums[cnt].push_back(str[i]);
    }

    //Преобразуем числа, записанные строкой, в обычные числа
    for (int i = 0; i <= cnt; i++)
    {
        nums.append(str_nums[i].toDouble());
    }

    int cnt_opers = 0; //Счетчик операций, необходимый для определения чисел, между которыми будет производиться операция
    //Цикл поиска и выполнения приоритетных операций (деление и умножение)
    for (int i = 0; i < opers.size(); i++)
    {
        if (opers[i] == '0') //Если операция уже была произведена
            continue;
        cnt_opers++;
        if (opers[i] == div_sign)
        {
            int k = 0; //Счетчик для определения второго числа в операции
            QList<double>::iterator prev_it; //Итератор для первого числа
            for (QList<double>::iterator it = nums.begin(); it != nums.end(); it++)
            {
                if (k == cnt_opers)
                {
                    if (*it == 0)
                    {
                        QMessageBox::about(this, NULL, "Деление на ноль!");
                        return;
                    }
                    double result = *prev_it / *it;
                    *it = result;
                    nums.erase(prev_it); //Удаляем первое число
                    opers[i] = '0'; //Операция деления выполнена
                    cnt--;

                    // Заново пройдемся по opers в поисках приоритетных операций (деление и умножение)
                    i = 0;
                    cnt_opers = 0;
                    break;
                }
                k++;
                prev_it = it;
            }
            continue;
        }
        if (opers[i] == mult_sign)
        {
            int k = 0; //Счетчик для определения второго числа в операции
            QList<double>::iterator prev_it; //Итератор для первого числа
            for (QList<double>::iterator it = nums.begin(); it != nums.end(); it++)
            {
                if (k == cnt_opers)
                {

                    double result = *prev_it * *it;
                    *it = result;
                    nums.erase(prev_it); //Удаляем первое число
                    opers[i] = '0'; //Операция умножения выполнена
                    cnt--;

                    // Заново пройдемся по opers в поисках приоритетных операций (деление и умножение)
                    i = 0;
                    cnt_opers = 0;
                    break;
                }
                k++;
                prev_it = it;
            }
            continue;
        }

    }
    cnt_opers = 0;
    //Цикл поиска и выполнения неприоритетных операций (сложение и вычитание)
    for (int i = 0; i < opers.size(); i++)
    {
        if (opers[i] == '0')
            continue;
        cnt_opers++;
        if (opers[i] == '+')
        {
            int k = 0; //Счетчик для определения второго числа в операции
            QList<double>::iterator prev_it; //Итератор для первого числа
            for (QList<double>::iterator it = nums.begin(); it != nums.end(); it++)
            {
                if (k == cnt_opers)
                {
                    //qInfo() << *prev_it << " " << *it;
                    double result = *prev_it + *it;
                    *it = result;
                    nums.erase(prev_it); //Удаляем первое число
                    opers[i] = '0'; //Операция сложения выполнена
                    cnt--;

                    // Заново пройдемся по opers в поисках неприоритетных операций (сложение и вычитание)
                    i = 0;
                    cnt_opers = 0;
                    break;
                }
                k++;
                prev_it = it;
            }
            continue;
        }

        if (opers[i] == '-')
        {
            int k = 0; //Счетчик для определения второго числа в операции
            QList<double>::iterator prev_it; //Итератор для первого числа
            for (QList<double>::iterator it = nums.begin(); it != nums.end(); it++)
            {
                if (k == cnt_opers)
                {

                    double result = *prev_it - *it;
                    *it = result;
                    nums.erase(prev_it); //Удаляем первое число
                    opers[i] = '0'; //Операция деления выполнена
                    cnt--;

                    // Заново пройдемся по opers в поисках неприоритетных операций (сложение и вычитание)
                    i = 0;
                    cnt_opers = 0;
                    break;
                }
                k++;
                prev_it = it;
            }
            continue;
        }
    }

    //В списке nums осталось одно число - наш итоговый результат выполнения всего выражения, который мы выведем на экран
    QString result = QString::number(nums.front(), 'g', 15);
    ui->CalcEdit->setText(result);


}

