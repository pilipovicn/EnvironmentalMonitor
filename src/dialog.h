#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include "ds18b20.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cmath>
#include <softPwm.h>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void update();
    
    void on_pushButton_clicked();
    
    void on_pushButton_2_clicked();

private:
    Ui::Dialog *ui;
    DS18B20 *dsTemp;
    QPixmap *happy;
    QPixmap *sad;
    QPixmap *excl2;
};

#endif // DIALOG_H
