#ifndef SECONDWIDGET_H
#define SECONDWIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QPainter>
namespace Ui {
class SecondWidget;
}

class SecondWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SecondWidget(QWidget *parent = nullptr);
    ~SecondWidget();

private slots:
    void on_pushButton_SendMsg_clicked();
    void udpInit();
    void dealMsg();
    void on_lineEdit_Port_editingFinished();
    void paintEvent(QPaintEvent *event);


private:
    Ui::SecondWidget *ui;
    QUdpSocket *myUdpSocket;
    quint16 guestPort;
    QString chatContent;
};

#endif // SECONDWIDGET_H
