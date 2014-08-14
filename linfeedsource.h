//
// linfeedsource.h
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

#ifndef LINFEEDSOURCE_H
#define LINFEEDSOURCE_H

#include <QListWidgetItem>

enum SourceType
{
  Native_Source,
  TheOldReader_Source
//  Facebook_Source
};

class LinFeedSource: public QListWidgetItem
{
public:
  LinFeedSource(
    SourceType type);

  SourceType getType()
    {return type;}

private:
  SourceType type;
};


class LinNativeFeedSource: public LinFeedSource
{
public:
  LinNativeFeedSource();
};


class LinTORFeedSource: public LinFeedSource
{
public:
  LinTORFeedSource();
};


/*
class LinFacebookFeedSource: public LinFeedSource
{
public:
  LinFacebookFeedSource();
};
*/

#endif // LINFEEDSOURCE_H
