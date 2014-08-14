#include "linauthenticationdialog.h"
#include "ui_linauthenticationdialog.h"

LinAuthenticationDialog::LinAuthenticationDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LinAuthenticationDialog)
{
  ui->setupUi(this);
}

LinAuthenticationDialog::~LinAuthenticationDialog()
{
  delete ui;
}


QString LinAuthenticationDialog::getEmail()
{
  return ui->emailLineEdit->text();
}


QString LinAuthenticationDialog::getPasswd()
{
  return ui->passwdLineEdit->text();
}


void LinAuthenticationDialog::hideLabel()
{
  ui->failedLoginLabel->hide();
}


void LinAuthenticationDialog::showLabel()
{
  ui->failedLoginLabel->show();
}


void LinAuthenticationDialog::on_pwChecBox_toggled(bool checked)
{
  if (checked)
  {
    ui->passwdLineEdit->setEchoMode(QLineEdit::Normal);
  }
  else
  {
    ui->passwdLineEdit->setEchoMode(QLineEdit::Password);
  }
}
