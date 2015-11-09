/*
 *  Manjaro Settings Manager
 *  Ramon Buldó <ramon@manjaro.org>
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

#include "IconThemeImageProvider.h"
#include <QtQuick/QQuickImageProvider>
#include <QtGui/QIcon>

IconThemeImageProvider::IconThemeImageProvider( ImageType type, Flags flags ) :
    QQuickImageProvider( type, flags )
{
}

IconThemeImageProvider::~IconThemeImageProvider()
{
}

QPixmap
IconThemeImageProvider::requestPixmap( const QString& id, QSize* size, const QSize& requestedSize )
{
    int width = 64;
    int height = 64;

    if ( size )
        *size = QSize( width, height );

    QIcon icon = QIcon::fromTheme( id );
    QPixmap pixmap = icon.pixmap( requestedSize.width() > 0 ? requestedSize.width() : width,
                                  requestedSize.height() > 0 ? requestedSize.height() : height );
    return pixmap;
}
