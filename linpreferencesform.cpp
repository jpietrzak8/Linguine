//
// linpreferencesform.cpp
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

#include "linpreferencesform.h"
#include "ui_linpreferencesform.h"

#include "linnewsfeedwidgetitem.h"

#include <QSettings>

LinPreferencesForm::LinPreferencesForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::LinPreferencesForm),
  openExternalPlayerFlag(false),
  openExternalBrowserFlag(true)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);
  setWindowFlags(windowFlags() | Qt::Window);

  // For now, leave the native prefs out:
  ui->nativePrefsFrame->hide();

  QSettings settings("pietrzak.org", "Linguine");

  if (settings.contains("OpenExternalPlayer"))
  {
    openExternalPlayerFlag = settings.value("OpenExternalPlayer").toBool();
    ui->playerCheckBox->setChecked(openExternalPlayerFlag);
  }

  if (settings.contains("OpenExternalBrowser"))
  {
    openExternalBrowserFlag = settings.value("OpenExternalBrowser").toBool();
    ui->browserCheckBox->setChecked(openExternalBrowserFlag);
  }

  if (settings.contains("HideImages"))
  {
    hideImagesFlag = settings.value("HideImages").toBool();
    ui->imagesCheckBox->setChecked(hideImagesFlag);
  }

  if (settings.contains("NativeSortType"))
  {
    QString sortTypeStr = settings.value("NativeSortType").toString();

    ui->nativeSortComboBox->setCurrentIndex(
      ui->nativeSortComboBox->findText(sortTypeStr));

    if (sortTypeStr == "Name")
    {
      LinNewsfeedWidgetItem::sortType = Name_Sort;
    }
    else if (sortTypeStr == "Timestamp")
    {
      LinNewsfeedWidgetItem::sortType = Timestamp_Sort;
    }
  }
}


LinPreferencesForm::~LinPreferencesForm()
{
  QSettings settings("pietrzak.org", "Linguine");

  settings.setValue("OpenExternalPlayer", openExternalPlayerFlag);
  settings.setValue("OpenExternalBrowser", openExternalBrowserFlag);
  settings.setValue("HideImages", hideImagesFlag);
  settings.setValue("NativeSortType", ui->nativeSortComboBox->currentText());

  delete ui;
}


void LinPreferencesForm::on_playerCheckBox_toggled(bool checked)
{
  openExternalPlayerFlag = checked;
}


void LinPreferencesForm::on_browserCheckBox_toggled(bool checked)
{
  openExternalBrowserFlag = checked;
}

void LinPreferencesForm::on_imagesCheckBox_toggled(bool checked)
{
  hideImagesFlag = checked;
}

void LinPreferencesForm::on_nativeSortComboBox_currentIndexChanged(
  const QString &arg1)
{
  if (arg1 == "Name")
  {
    LinNewsfeedWidgetItem::sortType = Name_Sort;
  }
  else if (arg1 == "Timestamp")
  {
    LinNewsfeedWidgetItem::sortType = Timestamp_Sort;
  }
}
