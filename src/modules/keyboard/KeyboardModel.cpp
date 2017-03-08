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
    // Root item for layouts
    m_layoutsRoot = new KeyboardItem( "layouts", "keyboard layouts", parent );
    parent->appendChild( m_layoutsRoot );

    m_modelsRoot = new KeyboardItem( "models", "keyboard models", parent );
    parent->appendChild( m_modelsRoot );

    QList<QString> xkbFileList;
    xkbFileList << "base.xml" << "base.extras.xml" << "evdev.xml" << "evdev.extras.xml";

    for ( int i = 0; i < xkbFileList.count(); ++i )
    {
        QFile xmlFile( "/usr/share/X11/xkb/rules/" + xkbFileList[ i ] );
        if( !xmlFile.open( QIODevice::ReadOnly ) )
        {
            qDebug() << "X11 " + xkbFileList[ i ] + "Keyboard layout and models definitions not found.";
            continue;
        }

        xml.setDevice( &xmlFile );

        if ( xml.readNextStartElement() && xml.name() == "xkbConfigRegistry" )
        {
            while ( xml.readNextStartElement( ))
            {

                if ( xml.name() == "modelList" )
                    processModelList();
                else if ( xml.name() == "layoutList" )
                    processLayoutLists();
                else if ( xml.name() == "optionList" )
                    xml.skipCurrentElement();  // TODO: implement the day we want manage options.
            }
        }

        if ( xml.tokenType() == QXmlStreamReader::Invalid )
            xml.readNext();

        if ( xml.hasError() )
        {
            xml.raiseError();
            qDebug() << errorString();
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

void
KeyboardModel::processLayoutLists()
{
    if ( !xml.isStartElement() || xml.name() != "layoutList" )
        return;

    while ( xml.readNextStartElement() )
    {
        if ( xml.name() == "layout" )
            processLayout();
        else
            xml.skipCurrentElement();
    }
}


void
KeyboardModel::processLayout()
{
    if ( !xml.isStartElement() || xml.name() != "layout" )
        return;

    QString name;
    QString description;

    while ( xml.readNextStartElement() )
    {
        if ( xml.name() == "configItem" )
        {
            while ( xml.readNextStartElement() )
            {
                if ( xml.name() == "name" )
                    name = readNextText();
                else if ( xml.name() == "description" )
                    description = readNextText();
                else
                    xml.skipCurrentElement();
            }

            if ( name.isEmpty() || description.isEmpty() )
                return;

            QModelIndexList layoutIndexList = match( index( 0,0 ).child( 0,0 ),
                                              KeyRole,
                                              name,
                                              -1,
                                              Qt::MatchExactly );

            if ( layoutIndexList.isEmpty() )
            {
                m_currentlayout = new KeyboardItem( name, description, m_layoutsRoot );
                m_currentlayout->appendChild( new KeyboardItem( QString( "default" ), QString( tr( "Default" ) ), m_currentlayout ) );
                m_layoutsRoot->appendChild( m_currentlayout );
            }
            else
            {
                m_currentlayout = static_cast<KeyboardItem*>( layoutIndexList.first().internalPointer() );
            }
        }
        else if ( xml.name() == "variantList" )
            processVariantList();
    }
}

void
KeyboardModel::processVariantList()
{
    if ( !xml.isStartElement() || xml.name() != "variantList" )
        return;
    while ( xml.readNextStartElement() )
    {
        if ( xml.name() == "variant" )
            processVariant();
        else
            xml.skipCurrentElement();
    }
}

void
KeyboardModel::processVariant()
{
    if ( !xml.isStartElement() || xml.name() != "variant" )
        return;

    QString name;
    QString description;

    while ( xml.readNextStartElement() )
    {
        if ( xml.name() == "configItem" )
        {
            while ( xml.readNextStartElement() )
            {
                if ( xml.name() == "name" )
                    name = readNextText();
                else if ( xml.name() == "description" )
                    description = readNextText();
                else
                    xml.skipCurrentElement();
            }

            if ( name.isEmpty() || description.isEmpty() )
                return;

            bool variantExist = false;
            for( int i = 0; i < m_currentlayout->childCount(); ++i )
            {
                if( m_currentlayout->child( i )->key() == name )
                {
                    variantExist = true;
                    break;
                }
            }
            if( !variantExist )
            {
                m_currentlayout->appendChild( new KeyboardItem( name, description, m_currentlayout ) );
            }
        }
    }
}

void
KeyboardModel::processModelList()
{
    if ( !xml.isStartElement() || xml.name() != "modelList" )
        return;
    while ( xml.readNextStartElement() )
    {
        if ( xml.name() == "model" )
            processModel();
        else
            xml.skipCurrentElement();
    }
}


void
KeyboardModel::processModel()
{
    if ( !xml.isStartElement() || xml.name() != "model" )
        return;

    QString name;
    QString description;
    QString vendor;

    while ( xml.readNextStartElement() )
    {
        if ( xml.name() == "configItem" )
        {
            while ( xml.readNextStartElement() )
            {
                if ( xml.name() == "name" )
                    name = readNextText();
                else if ( xml.name() == "description" )
                    description = readNextText();
                else if ( xml.name() == "vendor" )
                    vendor = readNextText();
                else
                    xml.skipCurrentElement();
            }

            if ( name.isEmpty() || description.isEmpty() )
                return;

            if ( vendor.isEmpty() )
                vendor = tr( "Unknown" );

            if ( name == "pc105" )
                description += " - " + QString( tr( "Default Keyboard Model" ) );

            QModelIndexList layoutIndexList = match( index( 1,0 ).child( 0,0 ),
                                              KeyRole,
                                              name,
                                              -1,
                                              Qt::MatchExactly );

            if ( layoutIndexList.isEmpty() )
            {
                m_modelsRoot->appendChild(new KeyboardItem( name, vendor + " | " + description, m_modelsRoot ) );
            }
        }
    }
}

QString
KeyboardModel::readNextText()
{
    return xml.readElementText();
}

QString
KeyboardModel::errorString()
{
    return QObject::tr( "%1\nLine %2, column %3" )
            .arg( xml.errorString() )
            .arg( xml.lineNumber() )
            .arg( xml.columnNumber() );
}
