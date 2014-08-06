#include "lincollectiondialog.h"
#include "ui_lincollectiondialog.h"

LinCollectionDialog::LinCollectionDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LinCollectionDialog)
{
  ui->setupUi(this);
}

LinCollectionDialog::~LinCollectionDialog()
{
  delete ui;
}
