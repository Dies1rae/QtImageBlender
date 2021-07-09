#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QStyle>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->path_img_one_ = ("./photo1.jpg");
    this->path_img_two_ = ("./photo2.png");
    this->path_img_res_ = ("./photo3.png");

    ui->vIMG1->setMinimumSize(441,399);
    ui->vIMG1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    ui->vIMG2->setMinimumSize(441,399);
    ui->vIMG2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    ui->vIMGMIX->setMinimumSize(441,399);
    ui->vIMGMIX->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    ui->vIMG1->setPixmap(this->path_img_one_);
    ui->vIMG2->setPixmap(this->path_img_two_);
    ui->vIMGMIX->setPixmap(this->path_img_res_);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bExit_clicked()
{
    QCoreApplication::quit();
}

void MainWindow::on_bLoadOne_clicked()
{
    this->path_img_one_ = QFileDialog::getOpenFileName(this, tr("Open File"), "c:/", "JPEG/JPG Files (*.jpeg, *.jpg);; PNG File (*.png);; BMP File (*.bmp);; All files (*.*)");
    if (!this->path_img_one_.isNull() && !this->path_img_one_.isEmpty()) {
        QImage tmp_img(this->path_img_one_);
        if (this->size_mode_) {
            this->img_one_ = std::move(tmp_img);
            this->img_one_ = this->img_one_.convertToFormat(QImage::Format_ARGB32);
            ui->vIMG1->setPixmap(QPixmap::fromImage(this->img_one_));
        } else if (!this->SizeChecker(tmp_img)) {
            this->img_one_ = std::move(tmp_img);
            this->img_one_ = this->img_one_.convertToFormat(QImage::Format_ARGB32);
            ui->vIMG1->setPixmap(QPixmap::fromImage(this->img_one_));
        } else if (this->SizeChecker(tmp_img)) {
            QMessageBox::information(this, tr("Info"),  "Image is too big");
        }
    }
    if (this->img_one_.isNull()) {
        QMessageBox::information(this, tr("Info"),  "No image loaded");
    }
}

void MainWindow::on_bLoadTwo_clicked()
{
    this->path_img_two_ = QFileDialog::getOpenFileName(this, tr("Open File"), "c:/", "JPEG/JPG Files (*.jpeg, *.jpg);; PNG File (*.png);; BMP File (*.bmp);; All files (*.*)");
    if (!this->path_img_two_.isNull() && !this->path_img_two_.isEmpty()) {
        QImage tmp_img(this->path_img_one_);
        if (this->size_mode_) {
            this->img_two_.load(this->path_img_two_);
            this->img_two_ = this->img_two_.convertToFormat(QImage::Format_ARGB32);
            ui->vIMG2->setPixmap(QPixmap::fromImage(this->img_two_));
        } else if (!this->SizeChecker(tmp_img)) {
            this->img_two_.load(this->path_img_two_);
            this->img_two_ = this->img_two_.convertToFormat(QImage::Format_ARGB32);
            ui->vIMG2->setPixmap(QPixmap::fromImage(this->img_two_));
        } else if (this->SizeChecker(tmp_img)) {
            QMessageBox::information(this, tr("Info"),  "Image is too big");
        }
    }
    if (this->img_two_.isNull()) {
        QMessageBox::information(this, tr("Info"),  "No image loaded");
    }
}

void MainWindow::on_bMIXIT_clicked()
{
    if(!this->img_one_.isNull() && !this->img_two_.isNull()) {
        int h_max = std::max(this->img_one_.height(), this->img_two_.height());
        int w_max = std::max(this->img_one_.width(), this->img_two_.width());
        QImage result({w_max, h_max}, QImage::Format_ARGB32);
        QPainter painter(&result);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        if (this->com_mode_) {
            painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
        }

        //This is pixel by pixel example, how i understand it
        /*for (int r = 0; r < h_max; r++) {
            for (int c = 0; c < w_max; c++) {
                QRgb pixA = (0,0,0);
                QRgb pixB = (0,0,0);
                QRgb pixR = (0,0,0,0);
                if (c < this->img_one_.width() && r < this->img_one_.height()) {
                    pixA = this->img_one_.pixel(c, r);
                }
                if (c < this->img_two_.width() && r < this->img_two_.height()) {
                    pixB = this->img_two_.pixel(c, r);
                }

                if (pixA != (0,0,0) && pixB != (0,0,0)) {
                    if (qAlpha(pixA) != UCHAR_MAX || qAlpha(pixB) != UCHAR_MAX) {
                        pixR = pixA & pixB;
                    } else {
                        pixR = pixB;
                    }
                } else if (pixA == (0,0,0) && pixB != (0,0,0)) {
                     pixR = pixB;
                }  else if (pixA != (0,0,0) && pixB == (0,0,0)) {
                    pixR = pixA;
                }

                result.setPixel(c, r, pixR);
            }
        }*/

        painter.drawImage(0, 0, this->img_one_);
        painter.drawImage(0, 0, this->img_two_);

        this->img_res_ = std::move(result);
        ui->vIMGMIX->setPixmap(QPixmap::fromImage(this->img_res_));
    } else {
        QMessageBox::information(this, tr("Error"),  "No images loaded");
    }
}

void MainWindow::on_bSave_clicked()
{

    if (!this->img_res_.isNull()) {
        this->path_img_res_ = QFileDialog::getSaveFileName(this, tr("Save File"), "c:/", "PNG File (*.png);;");
        QFile saved_png(this->path_img_res_);
        saved_png.open(QIODevice::WriteOnly);

        QDataStream out(&saved_png);
        this->img_res_.save(&saved_png, "PNG");
        out << this->img_res_;
    } else {
        QMessageBox::information(this, tr("Error"),  "No image mixed");
    }
}

void MainWindow::on_actionAbout_triggered()
{

    QString message = "It's small project to blend images by alpha channel\nDone by Poltavskiy Nick\nWith overall of 3 days QT experience";
    QMessageBox::information(this, tr("About"), message);
}

void MainWindow::on_bModeSource_clicked()
{
    this->com_mode_ = false;
}

void MainWindow::on_bModeDest_clicked()
{
    this->com_mode_ = true;
}

bool MainWindow::SizeChecker(const QImage& img)
{
    if(img.width() > 1024 || img.height() > 1024) {
        return true;
    }
    return false;
}

void MainWindow::on_cLimitSize_stateChanged(int arg1)
{
    if(arg1 > 0){
        this->size_mode_ = false;
    } else {
        this->size_mode_ = true;
    }
}
