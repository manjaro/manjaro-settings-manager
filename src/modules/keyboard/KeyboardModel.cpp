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

#include "KeyboardModel.h"
#include "KeyboardItem.h"

#include <KAuth>
#include <KAuthAction>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtWidgets/QMessageBox>

#include <QDebug>

#include <fstream>
#include <iostream>

KeyboardModel::KeyboardModel( QObject* parent )
    : QAbstractItemModel( parent )
{
    m_rootItem = new KeyboardItem( QString( "key" ), QString( "description" ) );
    initModel( m_rootItem );
    initLayout();
    initRateAndDelay();
}


KeyboardModel::~KeyboardModel()
{
    delete m_rootItem;
}


QVariant
KeyboardModel::data( const QModelIndex& index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    KeyboardItem* item = static_cast<KeyboardItem*>( index.internalPointer() );

    switch ( role )
    {
    case Qt::DisplayRole :
        switch ( index.column() )
        {
        case 0:
            return item->description();
        case 1:
            return item->key();
        }
        break;
    case KeyRole :
        return item->key();
    case DescriptionRole :
        return item->description();
    }

    return QVariant();
}


Qt::ItemFlags
KeyboardModel::flags( const QModelIndex& index ) const
{
    if ( !index.isValid() )
        return 0;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant
KeyboardModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
    {
        if ( section == 0 )
            return m_rootItem->key();
        else if ( section == 1 )
            return m_rootItem->description();
    }

    return QVariant();
}


QModelIndex
KeyboardModel::index( int row, int column, const QModelIndex& parent ) const
{
    if ( !hasIndex( row, column, parent ) )
        return QModelIndex();

    KeyboardItem* parentItem;

    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<KeyboardItem*>( parent.internalPointer() );

    KeyboardItem* childItem = parentItem->child( row );
    if ( childItem )
        return createIndex( row, column, childItem );
    else
        return QModelIndex();
}


QModelIndex
KeyboardModel::parent( const QModelIndex& index ) const
{
    if ( !index.isValid() )
        return QModelIndex();

    KeyboardItem* childItem = static_cast<KeyboardItem*>( index.internalPointer() );
    KeyboardItem* parentItem = childItem->parent();

    if ( parentItem == m_rootItem )
        return QModelIndex();

    return createIndex( parentItem->row(), 0, parentItem );
}


int
KeyboardModel::rowCount( const QModelIndex& parent ) const
{
    if ( parent.column() > 0 )
        return 0;

    KeyboardItem* parentItem;
    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<KeyboardItem*>( parent.internalPointer() );

    return parentItem->childCount();
}


int
KeyboardModel::columnCount( const QModelIndex& parent ) const
{
    if ( parent.isValid() )
        return static_cast<KeyboardItem*>( parent.internalPointer() )->columnCount();
    else
        return m_rootItem->columnCount();
}


QHash<int, QByteArray>
KeyboardModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[KeyRole] = "key";
    roles[DescriptionRole] = "description";
    return roles;
}


void
KeyboardModel::initModel( KeyboardItem* parent )
{
    const QString xkbFile( "/usr/share/X11/xkb/rules/base.lst" );

    QFile fh( xkbFile );
    fh.open( QIODevice::ReadOnly );

    if ( !fh.isOpen() )
        qDebug() << "X11 Keyboard layout and models definitions not found.";

    // Root item for layouts
    KeyboardItem* layoutsRoot = new KeyboardItem( "layouts", "keyboard layouts", parent );
    parent->appendChild( layoutsRoot );

    // Get layouts
    KeyboardItem* currentLayout;
    bool layoutsFound = false;

    while ( !fh.atEnd() )
    {
        QByteArray line = fh.readLine();

        // Find the layout section otherwhise continue. If the layout section is at it's
        // end, break the loop
        if ( !layoutsFound && line.startsWith( "! layout" ) )
            layoutsFound = true;
        else if ( layoutsFound && line.startsWith ( "!" ) )
            break;
        else if ( !layoutsFound )
            continue;

        QRegExp rx;
        rx.setPattern( "^\\s+(\\S+)\\s+(\\w.*)\n$" );

        // insert into the layout model
        if ( rx.indexIn( line ) != -1 )
        {
            QString layoutKey = rx.cap( 1 );
            QString layoutDescription = rx.cap( 2 );
            currentLayout = new KeyboardItem( layoutKey, layoutDescription, layoutsRoot );
            layoutsRoot->appendChild( currentLayout );
            currentLayout->appendChild( new KeyboardItem( QString( "default" ), QString( tr( "Default" ) ), currentLayout ) );
        }
    }


    // Get Variants
    bool variantsFound = false;
    // Start from the beginning
    fh.reset();
    while ( !fh.atEnd() )
    {
        QByteArray line = fh.readLine();

        // Continue until we found the variant section. If found, read until the
        // next section is found
        if ( !variantsFound && line.startsWith( "! variant" ) )
        {
            variantsFound = true;
            continue;
        }
        else if ( variantsFound && line.startsWith ( "!" ) )
            break;
        else if ( !variantsFound )
            continue;

        QRegExp rx;
        rx.setPattern( "^\\s+(\\S+)\\s+(\\S+): (\\w.*)\n$" );

        if ( rx.indexIn( line ) != -1 )
        {
            QString variantKey = rx.cap( 1 );
            QString layoutKey = rx.cap( 2 );
            QString variantDescription = rx.cap( 3 );

            QModelIndexList layoutIndexList = match( index( 0,0 ).child( 0,0 ),
                                              KeyRole,
                                              layoutKey,
                                              -1,
                                              Qt::MatchExactly );

            if ( layoutIndexList.isEmpty() )
            {
                currentLayout = new KeyboardItem( layoutKey, layoutKey, layoutsRoot );
                layoutsRoot->appendChild( currentLayout );
                currentLayout->appendChild( new KeyboardItem( QString( "default" ), QString( tr( "Default" ) ), currentLayout ) );
                currentLayout->appendChild( new KeyboardItem( variantKey, variantDescription, currentLayout ) );
            }
            else
            {
                currentLayout = static_cast<KeyboardItem*>( layoutIndexList.first().internalPointer() );
                currentLayout->appendChild( new KeyboardItem( variantKey, variantDescription, currentLayout ) );
            }

        }
    }


    // Root item for models
    KeyboardItem* modelsRoot = new KeyboardItem( "models", "keyboard models", parent );
    parent->appendChild( modelsRoot );

    // Get models
    bool modelsFound = false;
    // Start from the beginning
    fh.reset();
    while ( !fh.atEnd() )
    {
        QByteArray line = fh.readLine();

        // Continue until we found the model section. If found, read until the next section is found
        if ( !modelsFound && line.startsWith( "! model" ) )
            modelsFound = true;
        else if ( modelsFound && line.startsWith ( "!" ) )
            break;
        else if ( !modelsFound )
            continue;

        QRegExp rx;
        rx.setPattern( "^\\s+(\\S+)\\s+(\\w.*)\n$" );

        // Insert into the model
        if ( rx.indexIn( line ) != -1 )
        {
            QString modelKey = rx.cap( 1 );
            QString modelDescription = rx.cap( 2 );
            if ( modelKey == "pc105" )
                modelDescription += " - " + QString( tr( "Default Keyboard Model" ) );
            modelsRoot->appendChild( new KeyboardItem( modelKey, modelDescription, modelsRoot ) );
        }
    }
}


void
KeyboardModel::initLayout()
{
    m_layout = "us";
    m_variant = "default";
    m_model = "pc105";

    QProcess process;
    process.start( "setxkbmap", QStringList() << "-print" << "-verbose" << "10" );

    if ( !process.waitForFinished() )
        return;

    /*
     * Example output
     * ...
     * model:      pc105,pc104
     * layout:     es,us
     * variant:    cat,euro
     * ...
     */
    QString layout;
    QString variant;
    QString model;
    QStringList list = QString( process.readAll() ).split( "\n", QString::SkipEmptyParts );
    for ( QString line: list )
    {
        line = line.trimmed();
        if ( line.startsWith( "layout" ) )
        {
            QStringList split = line.split( ":", QString::SkipEmptyParts );
            split = split.value( 1 ).trimmed().split( ",", QString::SkipEmptyParts );
            layout = split.value( 0 ).trimmed();
        }
        if ( line.startsWith( "variant" ) )
        {
            QStringList split = line.split( ":", QString::SkipEmptyParts );
            split = split.value( 1 ).trimmed().split( ",", QString::SkipEmptyParts );
            variant = split.value( 0 ).trimmed();
        }
        if ( line.startsWith( "model" ) )
        {
            QStringList split = line.split( ":", QString::SkipEmptyParts );
            split = split.value( 1 ).trimmed().split( ",", QString::SkipEmptyParts );
            model = split.value( 0 ).trimmed();
        }
    }
    if ( !layout.isEmpty() )
    {
        m_layout = layout;
        if ( !variant.isEmpty() )
            m_variant = variant;
    }
    if ( !model.isEmpty() )
        m_model = model;

    m_newLayout = m_layout;
    m_newVariant = m_variant;
    m_newModel = m_model;
}


void
KeyboardModel::initRateAndDelay()
{
    m_delay = 600;
    m_rate = 25;
    FILE* file = popen( "xset q | grep rate", "r" );
    int delay, rate;
    fscanf( file, "%*[^0123456789]%d%*[^0123456789]%d", &delay, &rate );
    pclose( file );
    m_delay = delay;
    m_rate = rate;
    m_newDelay = m_delay;
    m_newRate = m_rate;
}


void KeyboardModel::setNewDelay( int newDelay )
{
    if ( m_newDelay != newDelay )
    {
        m_newDelay = newDelay;
        emit changed();
    }
}


void KeyboardModel::setNewRate( int newRate )
{
    if ( m_newRate != newRate )
    {
        m_newRate = newRate;
        emit changed();
    }
}


void KeyboardModel::setNewModel( const QString& newModel )
{
    if ( m_newModel != newModel )
    {
        m_newModel = newModel;
        emit changed();
    }
}


void KeyboardModel::setNewVariant( const QString& newVariant )
{
    if ( m_newVariant != newVariant )
    {
        m_newVariant = newVariant;
        emit changed();
    }
}


void KeyboardModel::setNewLayout( const QString& newLayout )
{
    if ( m_newLayout != newLayout )
    {
        m_newLayout = newLayout;
        emit changed();
    }
}


int KeyboardModel::delay() const
{
    return m_delay;
}


int KeyboardModel::rate() const
{
    return m_rate;
}


QString KeyboardModel::model() const
{
    return m_model;
}


QString KeyboardModel::variant() const
{
    return m_variant;
}


QString KeyboardModel::layout() const
{
    return m_layout;
}


QString KeyboardModel::newVariant() const
{
    return m_newVariant;
}


QString KeyboardModel::newLayout() const
{
    return m_newLayout;
}


bool
KeyboardModel::saveKeyboardLayout()
{
    QVariantMap args;
    args["model"] = m_newModel;
    args["layout"] = m_newLayout;

    if ( QString::compare( m_newVariant, "default" ) == 0 )
    {
        args["variant"] = "";
        system( QString( "setxkbmap -model \"%1\" -layout \"%2\"" ).arg( m_newModel, m_newLayout ).toUtf8() );
    }
    else
    {
        system( QString( "setxkbmap -model \"%1\" -layout \"%2\" -variant \"%3\"" ).arg( m_newModel, m_newLayout, m_newVariant ).toUtf8() );
        args["variant"] = m_newVariant;
    }

    KAuth::Action saveAction( QLatin1String( "org.manjaro.msm.keyboard.save" ) );
    saveAction.setHelperId( QLatin1String( "org.manjaro.msm.keyboard" ) );
    saveAction.setArguments( args );
    KAuth::ExecuteJob* job = saveAction.execute();
    if ( job->exec() )
    {
        m_model = m_newModel;
        m_layout = m_newLayout;
        m_variant = m_newVariant;
        return true;
    }
    else
    {
        QMessageBox::warning( nullptr,
                              tr( "Error!" ),
                              QString( tr( "Failed to set keyboard layout" ) ),
                              QMessageBox::Ok, QMessageBox::Ok );
        return false;
    }
}


void
KeyboardModel::saveRateAndDelay()
{
    std::string command = "xset r rate " + std::to_string( m_newDelay ) + " " + std::to_string( m_newRate );
    system( command.c_str() );

    // Make the changes persistant throughout the reboot usin ~/.xinitrc
    QString filePath = QDir::homePath() + "/.xinitrc";
    QFile fileIn( filePath );
    if ( !fileIn.open( QIODevice::ReadOnly | QIODevice::Text ) )
        qDebug() << "can't open '~/.xinitrc' to read";

    bool addedToXinitrc = false;
    QByteArray newXinitRc;
    while ( !fileIn.atEnd() )
    {
        QByteArray line = fileIn.readLine();
        if ( line.startsWith( "xset r rate" ) )
        {
            line = QByteArray::fromStdString( command ) + "\n";
            addedToXinitrc = true;
        }
        newXinitRc = newXinitRc + line;
    }
    fileIn.close();

    if ( !addedToXinitrc )
        newXinitRc = newXinitRc + QByteArray::fromStdString( command ) + "\n";

    QFile fileOut( filePath );
    if ( !fileOut.open( QIODevice::WriteOnly | QIODevice::Text ) )
        qDebug() << "can't open '~/.xinitrc' to write";
    fileOut.write( newXinitRc );
    fileOut.close();

    m_delay = m_newDelay;
    m_rate = m_newRate;
}
