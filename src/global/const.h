/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
 *
 *  Copyright (C) 2007 Free Software Foundation, Inc.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONST_H
#define CONST_H


#define LANGUAGE_PACKAGES_FILE  "/var/lib/manjaro-system-settings/language_packages.list"
#define LOCALE_GEN              "/etc/locale.gen"
#define LOCALE_CONF              "/etc/locale.conf"
#define LOCALESSUPPORTED "/usr/share/i18n/SUPPORTED"
#define LOCALESINFO "/usr/share/i18n/LOCALESINFO"
#define XKB_FILE "/usr/share/X11/xkb/rules/base.lst"
#define MIN_USER_UUID 1000
#define PASSWD "/etc/passwd"
#define GROUPCONF "/etc/group"
#define FACES_IMAGE_FOLDER "/usr/share/pixmaps/faces"
#define DEFAULT_USER_GROUPS "video,audio,power,disk,storage,optical,network,lp,scanner"
#define ADMIN_GROUP "wheel"
#define SUDOERSFILE "/etc/sudoers"


#endif // CONST_H
