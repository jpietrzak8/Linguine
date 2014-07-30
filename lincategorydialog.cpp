#include "lincategorydialog.h"
#include "ui_lincategorydialog.h"

LinCategoryDialog::LinCategoryDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LinCategoryDialog)
{
  ui->setupUi(this);
}

LinCategoryDialog::~LinCategoryDialog()
{
  delete ui;
}
