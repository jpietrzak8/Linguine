//
// linwidgetuiform.cpp
//
// Copyright 2015 by John Pietrzak (jpietrzak8@gmail.com)
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

#include "linwidgetuiform.h"
#include "ui_linwidgetuiform.h"

#include "lindbus.h"
#include "linwidgetsettingsdialog.h"
//#include <QString>
#include "linrssparser.h"
#include "linatomparser.h"
#include <QTimer>

LinWidgetUIForm::LinWidgetUIForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::LinWidgetUIForm),
  dbus(0),
  settingsDialog(0),
  rssParser(0),
  atomParser(0),
  updateTimer(0)
{
  ui->setupUi(this);

  dbus = new LinDBus();
  settingsDialog = new LinWidgetSettingsDialog();
  settingsDialog->hide();

  currentItem = settingsDialog->getCurrentItem();

  updateItem();

  connect(
    settingsDialog,
    SIGNAL(newsfeedSelected(LinNewsfeedWidgetItem *)),
    this,
    SLOT(newsfeedChanged(LinNewsfeedWidgetItem *)));

  // Setup the update timer:
  updateTimer = new QTimer(this);

  connect(
    updateTimer,
    SIGNAL(timeout()),
    this,
    SLOT(updateItem()));

  updateTimer->start(1800000); // half hour
}


LinWidgetUIForm::~LinWidgetUIForm()
{
  // Don't delete currentNewsfeed, not owned by this class.
  if (updateTimer) delete updateTimer;
  if (atomParser) delete atomParser;
  if (rssParser) delete rssParser;
  if (settingsDialog) delete settingsDialog;
  if (dbus) delete dbus;

  delete ui;
}


void LinWidgetUIForm::showSettingsDialog()
{
  settingsDialog->exec();
}


void LinWidgetUIForm::newsfeedChanged(
  LinNewsfeedWidgetItem *item)
{
  // Sanity check:
  if (!item) return;

  currentItem = item;

  updateDisplay();

  // Start an update on the item:
  updateItem();
}


void LinWidgetUIForm::updateItem()
{
  if (currentItem->getFormat() == RSS_Format)
  {
    // remove any existing parsers:
    if (atomParser)
    {
      delete atomParser;
      atomParser = 0;
    }

    if (rssParser) delete rssParser;

    rssParser = new LinRSSParser(
      currentItem,
      currentItem->getSourceUrl(),
      currentItem->getQnam());

    connect(
      rssParser,
      SIGNAL(itemUpdated()),
      this,
      SLOT(updateDisplay()));

    rssParser->startParsing();
  }
  else
  {
    // delete existing parsers:
    if (rssParser)
    {
      delete rssParser;
      rssParser = 0;
    }

    if (atomParser) delete atomParser;

    atomParser = new LinAtomParser(
      currentItem,
      currentItem->getSourceUrl(),
      currentItem->getQnam());

    connect(
      atomParser,
      SIGNAL(itemUpdated()),
      this,
      SLOT(updateDisplay()));

    atomParser->startParsing();
  }
}


void LinWidgetUIForm::updateDisplay()
{
  // Set up the labels:
  ui->nameLabel->setText(currentItem->getName());
  ui->contentLabel->setText(currentItem->getItemSummary());
  ui->dateLabel->setText(currentItem->getItemPubDate());

  // Set up the icon:
  ui->newsfeedButton->setIcon(QIcon(currentItem->getImage()));
}


void LinWidgetUIForm::on_newsfeedButton_clicked()
{
  dbus->launchMedia(currentItem->getMediaUrl());
}
