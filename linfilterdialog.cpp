//
// linfilterdialog.cpp
//
// Copyright 2014 by John Pietrzak (jpietrzak8@gmail.com)
//
// This file is part of Linguine.
//
// Linguine is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// Linguine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Linguine; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#include "linfilterdialog.h"
#include "ui_linfilterdialog.h"


LinFilterDialog::LinFilterDialog(
  QWidget *parent)
  : QDialog(parent),
    ui(new Ui::LinFilterDialog)
{
  ui->setupUi(this);

  ui->freqComboBox->addItem("Any", QVariant(Any_Rate));
  ui->freqComboBox->addItem("Hourly", QVariant(Hourly_Rate));
  ui->freqComboBox->addItem("Daily", QVariant(Daily_Rate));
  ui->freqComboBox->addItem("Weekly", QVariant(Weekly_Rate));

  ui->mediaComboBox->addItem("Any", QVariant(Any_Media));
  ui->mediaComboBox->addItem("Audio", QVariant(Audio_Media));
  ui->mediaComboBox->addItem("Video", QVariant(Video_Media));

  ui->contentComboBox->addItem("Any", QVariant(Any_Content));
  ui->contentComboBox->addItem("News", QVariant(News_Content));
  ui->contentComboBox->addItem("Politics", QVariant(Politics_Content));

  ui->langComboBox->addItem("Any", QVariant(Any_Content));
  ui->langComboBox->addItem("English", QVariant(English_Language));
  ui->langComboBox->addItem("Japanese", QVariant(Japanese_Language));
  ui->langComboBox->addItem("French", QVariant(French_Language));
  ui->langComboBox->addItem("Spanish", QVariant(Spanish_Language));
  ui->langComboBox->addItem("German", QVariant(German_Language));
  ui->langComboBox->addItem("Russian", QVariant(Russian_Language));
}


LinFilterDialog::~LinFilterDialog()
{
  delete ui;
}


void LinFilterDialog::on_buttonBox_accepted()
{
  // Store the current types:
  frequency = (FrequencyType) ui->freqComboBox->itemData(
    ui->freqComboBox->currentIndex()).toInt();

  media = (MediaType) ui->mediaComboBox->itemData(
    ui->mediaComboBox->currentIndex()).toInt();

  content = (ContentType) ui->contentComboBox->itemData(
    ui->contentComboBox->currentIndex()).toInt();

  language = (LanguageType) ui->langComboBox->itemData(
    ui->langComboBox->currentIndex()).toInt();
  
  emit refilter();
}
