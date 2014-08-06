//
// linmaemy5theme.cpp
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

#include "linmaemo5theme.h"

#include <QColor>
#include <QFont>
#include <QMaemo5Style>

LinMaemo5Theme::LinMaemo5Theme()
{
  QColor color;
  QString red, green, blue;

  color = QMaemo5Style::standardColor("DefaultBackgroundColor");

  red.setNum(color.red());
  green.setNum(color.green());
  blue.setNum(color.blue());

  backgroundColor = "rgb(" + red + ", " + green + ", " + blue + ")";

  color = QMaemo5Style::standardColor("DefaultTextColor");

  red.setNum(color.red());
  green.setNum(color.green());
  blue.setNum(color.blue());

  defaultTextColor = "rgb(" + red + ", " + green + ", " + blue + ")";

  color = QMaemo5Style::standardColor("ActiveTextColor");

  red.setNum(color.red());
  green.setNum(color.green());
  blue.setNum(color.blue());

  activeTextColor = "rgb(" + red + ", " + green + ", " + blue + ")";

  color = QMaemo5Style::standardColor("SecondaryTextColor");

  red.setNum(color.red());
  green.setNum(color.green());
  blue.setNum(color.blue());

  secondaryTextColor = "rgb(" + red + ", " + green + ", " + blue + ")";

  color = QMaemo5Style::standardColor("AccentColor1");

  red.setNum(color.red());
  green.setNum(color.green());
  blue.setNum(color.blue());

  accentColor = "rgb(" + red + ", " + green + ", " + blue + ")";

  QFont font = QMaemo5Style::standardFont("SystemFont");

  systemFontFamily = font.family();
}
