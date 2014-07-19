//
// lingstdatadialog.h
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

#ifndef LINGSTDATADIALOG_H
#define LINGSTDATADIALOG_H

#include <QDialog>

#include <gst/gst.h>

namespace Ui {
class LinGstDataDialog;
}

class LinGstDataDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit LinGstDataDialog(QWidget *parent = 0);
  ~LinGstDataDialog();

  int displayData(
    GstElement *pipeline);

  void setTitle(
    QString title);

  void setArtist(
    QString artist);

  void setAlbum(
    QString album);

private:
  Ui::LinGstDataDialog *ui;
};

#endif // LINGSTDATADIALOG_H
