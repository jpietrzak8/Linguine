//
// linmaemy5theme.h
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

#ifndef LINMAEMO5THEME_H
#define LINMAEMO5THEME_H

#include <QString>

class LinMaemo5Theme
{
public:
  LinMaemo5Theme();

  QString getBackgroundColor() { return backgroundColor; }
  QString getDefaultTextColor() { return defaultTextColor; }
  QString getActiveTextColor() { return activeTextColor; }
  QString getSecondaryTextColor() { return secondaryTextColor; }
  QString getAccentColor() { return accentColor; }
  QString getSystemFontFamily() { return systemFontFamily; }

private:
  QString backgroundColor;
  QString defaultTextColor;
  QString activeTextColor;
  QString secondaryTextColor;
  QString accentColor;
  QString systemFontFamily;
};

#endif // LINMAEMO5THEME_H
