#include "linimagedisplayform.h"
#include "ui_linimagedisplayform.h"

LinImageDisplayForm::LinImageDisplayForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::LinImageDisplayForm)
{
  ui->setupUi(this);
}

LinImageDisplayForm::~LinImageDisplayForm()
{
  delete ui;
}


void LinImageDisplayForm::displayImage(
  something)
{
  imageLabel->setPixmap(
    currentPixmap.scaled(
      imageLabel->width(),
      imageLabel->height(),
      Qt::KeepAspectRatio));
}

void LinImageDisplayForm::on_actionZoom_In_triggered()
{

}

void LinImageDisplayForm::on_actionZoom_Out_triggered()
{

}

void LinImageDisplayForm::on_actionNative_Resolution_triggered()
{

}

void LinImageDisplayForm::on_actionFit_to_Window_triggered()
{

}
