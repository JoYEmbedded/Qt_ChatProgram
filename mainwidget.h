#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QColorDialog>
#include <QTime>
#include <QShortcut>
#include <QUdpSocket>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    QColor fontColor;
    QString chatContent;
    QUdpSocket *myUdpSocket;
    quint16 guestPort;
    quint16 serverPort;
    //重绘，实现窗口自由缩放
    void paintEvent(QPaintEvent *event) override;
    void fontsizeInit();
    void shortcutInit();
    void udpInit();
    void dealMsg();
protected:

private slots:
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_comboBox_Fontsize_activated(int index);

    //实现选中文字或接下来打的文字加减粗
    void on_pushButton_Bold_clicked();
    void on_pushButton_Italic_clicked();
    void on_pushButton_Underline_clicked();
    void on_pushButton_Color_clicked();
    void on_pushButton_emit_clicked();
    void on_pushButton_exit_clicked();
    void on_lineEdit_Port_editingFinished();
};
#endif // MAINWIDGET_H
