/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Ramon Buldó <ramon@manjaro.org>
 *
 *  Manjaro Settings Manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Manjaro Settings Manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Manjaro Settings Manager.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LanguageListViewDelegate.h"

#include "EnabledLocalesModel.h"

#include <QDebug>

LanguageListViewDelegate::LanguageListViewDelegate( QObject* parent ) :
    QStyledItemDelegate( parent )
{
}


LanguageListViewDelegate::~LanguageListViewDelegate()
{

}


void
LanguageListViewDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    QStyledItemDelegate::paint( painter, option, QModelIndex() );

    painter->save();

    // Draw central text
    const QString key = qvariant_cast<QString>( index.data( EnabledLocalesModel::LocaleCodeRole ) );
    const QString country = qvariant_cast<QString>( index.data( EnabledLocalesModel::CountryRole ) );
    const QString language = qvariant_cast<QString>( index.data( EnabledLocalesModel::LanguageRole ) );

    QRectF* boundingRect( new QRectF );

    QFont textFont = option.font;
    textFont.setBold( true );
    textFont.setPointSize( option.font.pointSize() * 1.30 );
    painter->setPen( option.palette.color( QPalette::Normal, QPalette::WindowText ) );
    painter->setFont( textFont );
    painter->drawText( option.rect,
                       Qt::AlignCenter,
                       QString( "%1 (%2)\n" ).arg( language ).arg( country ),
                       boundingRect );

    // Store the right bounding rectangle to use when drawing the displayLanguageText and FormatsText
    qreal rightLimit = boundingRect->right();

    textFont.setBold( false );
    textFont.setPointSize( option.font.pointSize() );
    painter->setFont( textFont );
    painter->drawText( option.rect,
                       Qt::AlignCenter,
                       QString( "\n%1" ).arg( key ),
                       boundingRect );

    if ( boundingRect->right() > rightLimit )
        rightLimit = boundingRect->right();
    delete boundingRect;

    // Draw right text
    const QString address = qvariant_cast<QString>( index.data( EnabledLocalesModel::AddressRole ) );
    const QString collate = qvariant_cast<QString>( index.data( EnabledLocalesModel::CollateRole ) );
    const QString ctype = qvariant_cast<QString>( index.data( EnabledLocalesModel::CtypeRole ) );
    const QString identification = qvariant_cast<QString>( index.data( EnabledLocalesModel::IdentificationRole ) );
    const QString lang = qvariant_cast<QString>( index.data( EnabledLocalesModel::LangRole ) );
    // const QString languageLc = qvariant_cast<QString>(index.data(EnabledLocalesModel::LanguageLcRole));
    const QString measurement = qvariant_cast<QString>( index.data( EnabledLocalesModel::MeasurementRole ) );
    const QString monetary = qvariant_cast<QString>( index.data( EnabledLocalesModel::MonetaryRole ) );
    const QString messages = qvariant_cast<QString>( index.data( EnabledLocalesModel::MessagesRole ) );
    const QString name = qvariant_cast<QString>( index.data( EnabledLocalesModel::NameRole ) );
    const QString numeric = qvariant_cast<QString>( index.data( EnabledLocalesModel::NumericRole ) );
    const QString paper = qvariant_cast<QString>( index.data( EnabledLocalesModel::PaperRole ) );
    const QString telephone = qvariant_cast<QString>( index.data( EnabledLocalesModel::TelephoneRole ) );
    const QString time = qvariant_cast<QString>( index.data( EnabledLocalesModel::TimeRole ) );


    QString displayLanguageText;
    if ( ctype == key && collate == key && lang == key /*&& languageLc == key*/ && messages == key )
        displayLanguageText = QString( tr( "Display Language" ) );
    else
    {
        QString langText = QString( tr( "Language" ) );
        QString ctypeText = QString( "CType" );
        QString collateText = QString( tr( "Collation and Sorting" ) );
        QString messagesText = QString( tr( "Messages" ) );
        if ( lang == key )
        {
            if ( !displayLanguageText.isEmpty() )
                displayLanguageText.append( ", " );
            displayLanguageText.append( langText );
        }
        if ( ctype == key )
        {
            if ( !displayLanguageText.isEmpty() )
                displayLanguageText.append( ", " );
            displayLanguageText.append( ctypeText );
        }
        if ( collate == key )
        {
            if ( !displayLanguageText.isEmpty() )
                displayLanguageText.append( ", " );
            displayLanguageText.append( collateText );
        }
        if ( messages == key )
        {
            if ( !displayLanguageText.isEmpty() )
                displayLanguageText.append( ", " );
            displayLanguageText.append( messagesText );
        }
    }

    QString formatsText;
    if ( address == key && identification == key && measurement == key && monetary == key &&
            name == key && numeric == key && paper == key && telephone == key && time == key )
        formatsText = QString( tr( "Formats" ) );
    else
    {
        QString addressText = QString( tr( "Address" ) );
        QString identificationText = QString( tr( "Identification" ) );
        QString measurementText = QString( tr( "Measurement Units" ) );
        QString monetaryText = QString( tr( "Currency" ) );
        QString nameText = QString( tr( "Names" ) );
        QString numericText = QString( tr( "Numbers" ) );
        QString paperText = QString( tr( "Paper" ) );
        QString telephoneText = QString( tr( "Telephone" ) );
        QString timeText = QString( tr( "Time" ) );
        if ( address == key )
        {
            if ( !formatsText.isEmpty() )
                formatsText.append( ", " );
            formatsText.append( addressText );
        }
        if ( identification == key )
        {
            if ( !formatsText.isEmpty() )
                formatsText.append( ", " );
            formatsText.append( identificationText );
        }
        if ( measurement == key )
        {
            if ( !formatsText.isEmpty() )
                formatsText.append( ", " );
            formatsText.append( measurementText );
        }
        if ( monetary == key )
        {
            if ( !formatsText.isEmpty() )
                formatsText.append( ", " );
            formatsText.append( monetaryText );
        }
        if ( name == key )
        {
            if ( !formatsText.isEmpty() )
                formatsText.append( ", " );
            formatsText.append( nameText );
        }
        if ( numeric == key )
        {
            if ( !formatsText.isEmpty() )
                formatsText.append( ", " );
            formatsText.append( numericText );
        }
        if ( paper == key )
        {
            if ( !formatsText.isEmpty() )
                formatsText.append( ", " );
            formatsText.append( paperText );
        }
        if ( telephone == key )
        {
            if ( !formatsText.isEmpty() )
                formatsText.append( ", " );
            formatsText.append( telephoneText );
        }
        if ( time == key )
        {
            if ( !formatsText.isEmpty() )
                formatsText.append( ", " );
            formatsText.append( timeText );
        }
    }

    QRectF displayAndFormatsRectF( QRectF( option.rect ).adjusted( rightLimit + 20, 0, -20, 0 ) );
    if ( !displayLanguageText.isEmpty() && formatsText.isEmpty() )
    {
        painter->drawText( displayAndFormatsRectF,
                           Qt::AlignVCenter | Qt::AlignRight | Qt::TextWordWrap,
                           displayLanguageText );
    }
    else if ( !formatsText.isEmpty() && displayLanguageText.isEmpty() )
    {
        painter->drawText( displayAndFormatsRectF,
                           Qt::AlignVCenter | Qt::AlignRight | Qt::TextWordWrap,
                           formatsText );
    }
    else if ( !displayLanguageText.isEmpty() && !formatsText.isEmpty() )
    {
        painter->drawText( displayAndFormatsRectF,
                           Qt::AlignVCenter | Qt::AlignRight | Qt::TextWordWrap,
                           QString().append( displayLanguageText ).append( "\n" ).append( formatsText ) );
    }

    painter->restore();
}


QSize
LanguageListViewDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    Q_UNUSED( index )
    Q_UNUSED( option )
    return QSize( 200, 60 );
}
