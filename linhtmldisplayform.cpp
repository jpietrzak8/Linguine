//
// linhtmldisplayform.cpp
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

#include "linhtmldisplayform.h"
#include "ui_linhtmldisplayform.h"

#include "mainwindow.h"
#include <QMaemo5InformationBox>
#include <QDesktopServices>

#include <QDebug>

#include "qwebviewselectionsuppressor.h"


LinHtmlDisplayForm::LinHtmlDisplayForm(
  MainWindow *mw)
  : QWidget(mw),
    useExternalBrowser(true),
    ui(new Ui::LinHtmlDisplayForm),
    mainWindow(mw),
    suppressor(0)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);
  setWindowFlags(windowFlags() | Qt::Window);

  suppressor = new QWebViewSelectionSuppressor(ui->htmlDisplay);

  blankHtmlPage = "<html><head><style type=\"text/css\">body{background-color: ";
  blankHtmlPage += mainWindow->getBackgroundColor();
  blankHtmlPage += ";}</style></head><body></body></html>";

  // Initialize the display:
  ui->htmlDisplay->setHtml(blankHtmlPage);

  // Set up the HTML boilerplate:
  htmlPrefix = constructHtmlPrefix();

  // Manage links:
  ui->htmlDisplay->page()->setLinkDelegationPolicy(
    QWebPage::DelegateExternalLinks);

  connect(
    ui->htmlDisplay->page(),
    SIGNAL(linkClicked(const QUrl &)),
    this,
    SLOT(onLinkClicked(const QUrl &)));
}


LinHtmlDisplayForm::~LinHtmlDisplayForm()
{
  delete ui;
}


void LinHtmlDisplayForm::closeEvent(
  QCloseEvent *event)
{
  QWidget::closeEvent(event);

  // The HTML browser can be very expensive, so don't let it continue running
  // in the background:
  ui->htmlDisplay->stop();

  // Reset the page:
  ui->htmlDisplay->setHtml(blankHtmlPage);
}


void LinHtmlDisplayForm::setHtml(
  QString htmlData)
{
  ui->htmlDisplay->setHtml(htmlData);
}


void LinHtmlDisplayForm::setHtml(
  QString htmlData,
  QString sourceUrl)
{
  ui->htmlDisplay->setHtml(htmlData, sourceUrl);
}


QWebFrame *LinHtmlDisplayForm::getFrame()
{
  return ui->htmlDisplay->page()->mainFrame();
}


void LinHtmlDisplayForm::onLinkClicked(
  const QUrl &url)
{
  if (useExternalBrowser)
  {
    QDesktopServices::openUrl(url);
  }
  else
  {
    ui->htmlDisplay->load(url);
  }
}


QString LinHtmlDisplayForm::constructHtmlPrefix()
{
  // Create the head portion of the HTML:
  htmlPrefix = "<html>\n <head>\n  <title>Linguine Output</title>\n";
  // Place some CSS in the head of the html:
  htmlPrefix += "  <style type=\"text/css\">\n";
  htmlPrefix += "   hr {color: " + mainWindow->getAccentColor() + ";}\n";
  htmlPrefix += "   img.main {max-width: 100%; height: auto; float: left; margin-right: 10px;}\n";
  htmlPrefix += "   img.footer {height: 20px; width: auto;}\n";
//  htmlPrefix += "   body {-webkit-user-select: none;}\n";
  htmlPrefix += "   body {\n";
  htmlPrefix += "    color: " + mainWindow->getDefaultTextColor() + ";\n";
  htmlPrefix += "    background-color: " + mainWindow->getBackgroundColor() + ";\n";
  htmlPrefix += "    font-family: \"" + mainWindow->getSystemFontFamily() + "\", sans-serif;\n";
  htmlPrefix += "    font-size: x-large;\n";
  htmlPrefix += "   }\n";
  htmlPrefix += "   a:link {\n";
  htmlPrefix += "    color: " + mainWindow->getActiveTextColor() + ";\n";
  htmlPrefix += "   }\n";
  htmlPrefix += "   a:visited {\n";
  htmlPrefix += "    color: " + mainWindow->getSecondaryTextColor() + ";\n";
  htmlPrefix += "   }\n";
  htmlPrefix += "  </style>\n";
  // Finish the head portion, start the body:
  htmlPrefix += " </head>\n <body>\n";

  return htmlPrefix;
}
