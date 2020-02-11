#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <api.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


public slots:
    void warningSlot(QString);
    void productMessageInit(QJsonObject);

private slots:

    void on_loginBtn_clicked();
    void on_productEdit_returnPressed();
    void on_cancelBtn_clicked();
    void on_radioButton_2_toggled(bool checked);
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;

    Api *api;
};
#endif // WIDGET_H
