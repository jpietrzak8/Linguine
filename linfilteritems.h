//
// linfilteritems.h
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

#ifndef LINFILTERITEMS_H
#define LINFILTERITEMS_H


enum FrequencyType
{
  Any_Rate,
  Hourly_Rate,
  Daily_Rate,
  Weekly_Rate,
  Other_Rate
};


enum MediaType
{
  Any_Media,
  Audio_Media,
  Video_Media,
  Text_Media
//  Image_Media
};


enum LanguageType
{
  Any_Language,
  English_Language,
  Japanese_Language,
  French_Language,
  Spanish_Language,
  German_Language,
  Russian_Language
};


#endif // LINFILTERITEMS_H
