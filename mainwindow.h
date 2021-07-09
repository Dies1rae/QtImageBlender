#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QPixmap>
#include <QImage>
#include <QPainter>

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
    void on_bExit_clicked();

    void on_bLoadOne_clicked();

    void on_bLoadTwo_clicked();

    void on_bMIXIT_clicked();

    void on_bSave_clicked();

    void on_actionAbout_triggered();

    void on_bModeSource_clicked();

    void on_bModeDest_clicked();

    void on_cLimitSize_stateChanged(int arg1);

private:
    bool SizeChecker(const QImage& img);

    Ui::MainWindow *ui;
    QString path_img_one_;
    QString path_img_two_;
    QString path_img_res_;
    QImage img_one_;
    QImage img_two_;
    QImage img_res_;
    bool com_mode_ = false;
    bool size_mode_ = false;
};
#endif // MAINWINDOW_H
