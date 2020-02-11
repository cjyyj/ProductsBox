#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , api(new Api)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);


    //请求错误
    void(Api::*waring)(QString)= &Api::waring;
    void(Widget::*warningSlot)(QString)= &Widget::warningSlot;
    connect(api,waring,this,warningSlot);

    //登陆成功
    connect(api,&Api::signInSignal,[=](QJsonObject obj){
       ui->stackedWidget->setCurrentIndex(1);
    });

    //根据轮号获取数据
    void(Api::*getProductMessageSignal)(QJsonObject)= &Api::getProductMessageSignal;
    void(Widget::*productMessageInit)(QJsonObject)= &Widget::productMessageInit;
    connect(api,getProductMessageSignal,this,productMessageInit);

    //首页按钮
    QPushButton *idxBtns[] ={ui->idxBtn1};
    unsigned idxBtnLen = std::extent<decltype(idxBtns)>::value;
    for(unsigned i = 0;i<idxBtnLen;i++)
    {
        connect(idxBtns[i],&QPushButton::clicked,[=](){
            ui->stackedWidget->setCurrentIndex(i+2);
        });
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::productMessageInit(QJsonObject obj)
{
//    qDebug() << obj;

    ui->product_label_1->setText(obj.value("specifications").toString());
    ui->product_label_2->setText(obj.value("deviceCode").toString());
    ui->product_label_4->setText(QString::number(obj.value("footWallWeight").toDouble(),10,2));
    ui->product_label_5->setText(obj.value("noodles").toString());
    ui->product_label_6->setText(QString::number(obj.value("torque").toDouble(),10,1));
    ui->product_label_7->setText(QString::number(obj.value("bowHeight").toDouble(),10,1));
    ui->product_label_8->setText(obj.value("flatness").toString());
}


void Widget::warningSlot(QString message)
{
    QMessageBox::warning(this,"警告",message);
}

//点击登陆
void Widget::on_loginBtn_clicked()
{
    QString nameText = ui->nameInput->text();
    QString passText = ui->passInput->text();
    api->signIn(nameText,passText);
}

//轮号输入框回车
void Widget::on_productEdit_returnPressed()
{
    api->getProductMessage(ui->productEdit->text());
}
void Widget::on_pushButton_clicked()
{
    api->getProductMessage(ui->productEdit->text());
}

//新增检验取消
void Widget::on_cancelBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_radioButton_2_toggled(bool checked)
{
    ui->widget_5->setEnabled(checked);
}

