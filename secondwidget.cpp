#include "secondwidget.h"
#include "ui_secondwidget.h"

SecondWidget::SecondWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SecondWidget)
{
    ui->setupUi(this);
    udpInit();
}

SecondWidget::~SecondWidget()
{
    delete ui;
}

void SecondWidget::paintEvent(QPaintEvent *event)
{
    QPainter *myPainter = new QPainter(this);
    myPainter->drawPixmap(rect(),QPixmap(":/new/prefix1/images/guestBG.jpg"));
    myPainter->end();
}

void SecondWidget::on_pushButton_SendMsg_clicked()
{
    QString ip = ui->lineEdit_IP->text();
    QByteArray sendMsg;
    sendMsg = ui->textEdit_send->toPlainText().toUtf8();
    myUdpSocket->writeDatagram(sendMsg,QHostAddress(ip),guestPort);
    ui->textEdit_send->setText("");

}

void SecondWidget::udpInit()
{
    myUdpSocket = new QUdpSocket(this);
    myUdpSocket->bind(9999);
    connect(myUdpSocket,&QUdpSocket::readyRead,this,&SecondWidget::dealMsg);
}

void SecondWidget::dealMsg()
{
    char buf[10240] = {0};
    QHostAddress cliAddr;
    cliAddr.setAddress(ui->lineEdit_IP->text());

    qint64 len = myUdpSocket->readDatagram(buf,sizeof(buf),&cliAddr,&guestPort);
    QTextEdit *mytextEdit = new QTextEdit(this);
    if(len>0)
    {
        QString msg = QString("[%1:%2]%3")
                          .arg(cliAddr.toString())
                          .arg(guestPort)
                          .arg(buf);
        // mytextEdit->setText(msg);
        ui->textBrowser->setHtml(msg);
    }
}

void SecondWidget::on_lineEdit_Port_editingFinished()
{
    guestPort = ui->lineEdit_Port->text().toInt();
}

