#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    fontsizeInit();
    shortcutInit();
    udpInit();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::paintEvent(QPaintEvent *event)
{
    QPainter *myPainter = new QPainter(this);
    myPainter->drawPixmap(rect(),QPixmap(":/new/prefix1/images/bg.jpg"));
    myPainter->end();
}

void MainWidget::on_fontComboBox_currentFontChanged(const QFont &f)
{
    QFont currentFont = ui->fontComboBox->currentFont();
    ui->textEdit_Input->setFont(currentFont);
    ui->textBrowser->setFont(currentFont);

    //防止改字号后变字体导致字号又缩小的情况
    QFont textFont = ui->textEdit_Input->font();
    QFont textbrowserFont = ui->textBrowser->font();
    textFont.setPointSize(ui->comboBox_Fontsize->currentData().toInt());
    textbrowserFont.setPointSize(ui->comboBox_Fontsize->currentData().toInt());
    ui->textEdit_Input->setFont(textFont);
    ui->textBrowser->setFont(textbrowserFont);
}

void MainWidget::fontsizeInit()
{
    int minSize = 8, maxSize = 72;
    for (int size = minSize; size <= maxSize; size++)
    {
        //第二项为关联数据，与currentData对应
        ui->comboBox_Fontsize->addItem(QString::number(size),size);
    }

}
void MainWidget::shortcutInit()
{
    QShortcut *exit_shortcut = new QShortcut(QKeySequence("ESC"),this);
    connect(exit_shortcut,&QShortcut::activated,this,&MainWidget::on_pushButton_exit_clicked);

    QShortcut *emit_shortcut = new QShortcut(QKeySequence("Ctrl+E"),this);
    connect(emit_shortcut,&QShortcut::activated,this,&MainWidget::on_pushButton_emit_clicked);
}
void MainWidget::on_comboBox_Fontsize_activated(int index)
{
    QFont textFont = ui->textEdit_Input->font();
    QFont textbrowserFont = ui->textBrowser->font();
    textFont.setPointSize(ui->comboBox_Fontsize->currentData().toInt());
    textbrowserFont.setPointSize(ui->comboBox_Fontsize->currentData().toInt());
    ui->textEdit_Input->setFont(textFont);
    ui->textBrowser->setFont(textbrowserFont);
}


void MainWidget::on_pushButton_Bold_clicked()
{
    if (!ui->textEdit_Input->currentFont().bold())
    {
        ui->textEdit_Input->setFontWeight(QFont::Bold);
    }
    else
    {
        ui->textEdit_Input->setFontWeight(QFont::Normal);
    }


}


void MainWidget::on_pushButton_Italic_clicked()
{
    ui->textEdit_Input->setFontItalic(!ui->textEdit_Input->currentFont().italic());
}


void MainWidget::on_pushButton_Underline_clicked()
{
    ui->textEdit_Input->setFontUnderline(!ui->textEdit_Input->currentFont().underline());
}


void MainWidget::on_pushButton_Color_clicked()
{
    fontColor = QColorDialog::getColor(fontColor,this);
    if(fontColor.isValid())
    {
        ui->textEdit_Input->setTextColor(fontColor);
        ui->textEdit_Input->setFocus();
    }
}


void MainWidget::on_pushButton_emit_clicked()
{
    if(ui->textEdit_Input->document())
    {
        //借助新的textedit生成默认格式的html text string格式
        QTextEdit *timeTextEdit = new QTextEdit;
        QString timeContent = QTime::currentTime().toString();
        timeTextEdit->setText(QString("[%1]").arg(serverPort) + timeContent);
        QString timeContentToHtml = timeTextEdit->document()->toHtml();

        QString htmlContent = ui->textEdit_Input->document()->toHtml();
        chatContent = timeContentToHtml + htmlContent;

        ui->textBrowser->append(chatContent);

        //通信

        QString ip = ui->lineEdit_IP->text();
        QByteArray sendMsg;
        sendMsg = chatContent.toUtf8();
        myUdpSocket->writeDatagram(sendMsg,QHostAddress(ip),guestPort);
        ui->textEdit_Input->setText("");
    }
}


void MainWidget::on_pushButton_exit_clicked()
{
    QApplication::quit();
}

void MainWidget::udpInit()
{
    myUdpSocket = new QUdpSocket(this);
    myUdpSocket->bind(8888);
    connect(myUdpSocket,&QUdpSocket::readyRead,this,&MainWidget::dealMsg);
}

void MainWidget::dealMsg()
{
    char buf[10240] = {0};
    QHostAddress cliAddr;
    cliAddr.setAddress(ui->lineEdit_IP->text());
    qint64 len = myUdpSocket->readDatagram(buf,sizeof(buf),&cliAddr,&guestPort);
    QString html_sentence = QString::fromUtf8(buf);
    if(len>0)
    {
        ui->textBrowser->append(html_sentence);

    }
}

void MainWidget::on_lineEdit_Port_editingFinished()
{
    guestPort = ui->lineEdit_otherPort->text().toInt();
}


void MainWidget::on_lineEdit_myPort_editingFinished()
{
    serverPort = ui->lineEdit_myPort->text().toInt();
    myUdpSocket->bind(serverPort);
    MainWidget::setWindowTitle(QString("MicroQQ（端口:%1)").arg(serverPort));
}


void MainWidget::on_lineEdit_otherPort_editingFinished()
{
    guestPort = ui->lineEdit_otherPort->text().toInt();
}

