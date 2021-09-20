#include "dialog.h"
#include "ui_dialog.h"

static int fd;
static double limitTemp;
static double validlimitTemp=-1;
static double validlimitHum=-1;
static double validlimitLight=-1;
static double currentTemp = 0;
static int adcVal = 0;
static double light = 0;
static double humidity = 0;
const char PCF8591 = 0x48;
const int LED = 22;
static int buzzCounter = 0;
static int alarm = 0;
static int tempExcl = 0;
static int humExcl = 0;
static int brighExcl = 0;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    happy = new QPixmap("/home/pi/EE54_EE101/ee101/utils/daisyHappy2.png");
    sad = new QPixmap("/home/pi/EE54_EE101/ee101/utils/daisySad2.png");
    excl2 = new QPixmap("/home/pi/EE54_EE101/ee101/utils/excl2.png");

    ui->label_2->setAttribute(Qt::WA_TranslucentBackground);
    ui->TempLcdNumber->setAttribute(Qt::WA_TranslucentBackground);
    ui->HumLcdNumber->setAttribute(Qt::WA_TranslucentBackground);
    ui->BrighLcdNumber->setAttribute(Qt::WA_TranslucentBackground);
    ui->VlaznostLabel->setAttribute(Qt::WA_TranslucentBackground);
    ui->TempLabel->setAttribute(Qt::WA_TranslucentBackground);
    ui->OsvetljenostLabel->setAttribute(Qt::WA_TranslucentBackground);
    ui->label->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_3->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_4->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_5->setAttribute(Qt::WA_TranslucentBackground);
    ui->excl1->setAttribute(Qt::WA_TranslucentBackground);
    ui->excl2->setAttribute(Qt::WA_TranslucentBackground);
    ui->excl3->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_2->setPixmap(*happy);
    ui->TempLcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->HumLcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->BrighLcdNumber->setSegmentStyle(QLCDNumber::Flat);

    setFixedSize(size());
    setStyleSheet( "background-image:url(/home/pi/EE54_EE101/ee101/utils/background2.png);" );
    dsTemp = new DS18B20("28-00000cfb2279");
    QTimer *timer = new QTimer(this);
                connect(timer, &QTimer::timeout, this, QOverload<>::of(&Dialog::update));
                timer->start(1000);
    if (wiringPiSetup() == -1) exit(1);

    fd = wiringPiI2CSetup(PCF8591);
    ui->lineEdit->setInputMask("00.0;0");
    ui->lineEdit_2->setInputMask("00;0");
    ui->lineEdit_3->setInputMask("00;0");

    if (softPwmCreate(LED, 0, 10) != 0)
            exit (2);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked(){
    if (ui->lineEdit->hasAcceptableInput() && ui->lineEdit_2->hasAcceptableInput() && ui->lineEdit_3->hasAcceptableInput()){

        QString textTemp=ui->lineEdit->displayText();//get text from textEdit
        limitTemp=textTemp.toDouble(); //convert textTemp to double
        validlimitTemp=limitTemp;

        textTemp=ui->lineEdit_2->displayText();
        limitTemp=textTemp.toDouble();
        validlimitHum=limitTemp;

        textTemp=ui->lineEdit_3->displayText();
        limitTemp=textTemp.toDouble();
        validlimitLight=limitTemp;

            ui->label->setText("OK!");

    }else{
        ui->label->setText("Greska u formatu!");
    }
}

void Dialog::update(){

    currentTemp = dsTemp->getTemp();
    ui->TempLcdNumber->display(currentTemp);

    wiringPiI2CReadReg8(fd, PCF8591 + 0) ; // dummy
    adcVal = wiringPiI2CReadReg8(fd, PCF8591 + 0) ;
    light = 100 - adcVal*100/255;
    ui->BrighLcdNumber->display(light);

    wiringPiI2CReadReg8(fd, PCF8591 + 3) ; // dummy
    adcVal = wiringPiI2CReadReg8(fd, PCF8591 + 3) ;
    humidity = 100 - adcVal*100/255;
    ui->HumLcdNumber->display(humidity);

    if(currentTemp<validlimitTemp || light<validlimitLight || humidity<validlimitHum){
        if(validlimitHum != -1 && validlimitTemp != -1 && validlimitLight != -1){
            alarm = 1;
            if(currentTemp<validlimitTemp)
                ui->excl1->setPixmap(*excl2);
            else
                ui->excl1->clear();
            if(light<validlimitLight)
                ui->excl3->setPixmap(*excl2);
            else
                ui->excl3->clear();
            if(humidity<validlimitHum)
                ui->excl2->setPixmap(*excl2);
            else
                ui->excl2->clear();

            ui->label_2->setPixmap(*sad);
        }
    }else{
        alarm = 0;
        ui->excl1->clear();
        ui->excl2->clear();
        ui->excl3->clear();
        ui->label_2->setPixmap(*happy);
    }

    buzzCounter++;
    buzzCounter = buzzCounter % 3;
    if(buzzCounter == 0 && alarm == 1){
        softPwmWrite(LED, 9);
    }else{
        softPwmWrite(LED, 0);
    }

}
