#include "linseekdialog.h"
#include "ui_linseekdialog.h"

LinSeekDialog::LinSeekDialog(
  QWidget *parent)
  : QDialog(parent),
    ui(new Ui::LinSeekDialog)
{
  ui->setupUi(this);
}


LinSeekDialog::~LinSeekDialog()
{
  delete ui;
}


void LinSeekDialog::setupAndExec(
  int percentagePlayed)
{
  ui->seekSlider->setValue(percentagePlayed);

  exec();
}


/*
void LinSeekDialog::on_seekSlider_valueChanged(int value)
{
  emit seekToPercentage(value);
}
*/


void LinSeekDialog::on_seekSlider_sliderMoved(int value)
{
  emit seekToPercentage(value);
}


void LinSeekDialog::reset()
{
  ui->seekSlider->setValue(0);
}
