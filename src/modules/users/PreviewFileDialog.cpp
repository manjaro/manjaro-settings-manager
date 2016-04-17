/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
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

#include "PreviewFileDialog.h"

PreviewFileDialog::PreviewFileDialog( QWidget* parent ) :
    QFileDialog( parent )
{
    QGridLayout* gridLayout = dynamic_cast<QGridLayout*>( this->layout() );

    if ( gridLayout )
    {
        QLayoutItem* layoutItem = gridLayout->itemAtPosition( 1,0 );
        QSplitter* splitter;
        splitter = dynamic_cast<QSplitter*>( layoutItem->widget() );

        if ( splitter )
        {
            QFrame* framePreview = new QFrame( splitter );
            splitter->addWidget( framePreview );

            QVBoxLayout* vboxPreviewLayout = new QVBoxLayout( framePreview );
            QGridLayout* gridPreviewLayout = new QGridLayout();

            m_labelPreview.setFrameShape( QFrame::NoFrame );
            m_labelPreview.setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
            m_labelPreview.setMinimumSize( 100,100 );

            gridPreviewLayout->addWidget( &m_labelPreviewWidthText, 0, 0, 1, 1, Qt::AlignLeft );
            gridPreviewLayout->addWidget( &m_labelPreviewWidth, 0, 1, 1, 1, Qt::AlignLeft );
            gridPreviewLayout->addWidget( &m_labelPreviewHeightText, 1, 0, 1, 1, Qt::AlignLeft );
            gridPreviewLayout->addWidget( &m_labelPreviewHeight, 1, 1, 1, 1, Qt::AlignLeft );
            gridPreviewLayout->addWidget( &m_labelPreviewRatioText, 2, 0, 1, 1, Qt::AlignLeft );
            gridPreviewLayout->addWidget( &m_labelPreviewRatio, 2, 1, 1, 1, Qt::AlignLeft );
            gridPreviewLayout->setColumnStretch( 1,1 );

            vboxPreviewLayout->addWidget( &m_labelPreview, 1, Qt::AlignHCenter );
            vboxPreviewLayout->addLayout( gridPreviewLayout );
            vboxPreviewLayout->addStretch( 1 );

            framePreview->setLayout( vboxPreviewLayout );

            connect( this, SIGNAL( currentChanged( QString ) ), this, SLOT( setPreviewPicture( QString ) ) );
        }
    }
}


void
PreviewFileDialog::setPreviewPicture( const QString& picture )
{
    QPixmap pixmapPicture;

    if ( pixmapPicture.load( picture ) )
    {
        int x, y;
        double ratio;
        x = pixmapPicture.width();
        y = pixmapPicture.height();
        ratio = double( x ) / double( y );

        if ( ( x > m_labelPreview.width() ) || ( y > m_labelPreview.height() ) )
        {
            m_labelPreview.setScaledContents( true );
            if ( x > y )
                m_labelPreview.setMinimumHeight( int( m_labelPreview.size().width() / ratio ) );
            else
                m_labelPreview.setMinimumWidth( int( m_labelPreview.size().height() * ratio ) );
        }
        else
        {
            m_labelPreview.setScaledContents( false );
            m_labelPreview.setMinimumSize( 100,100 );
        }

        m_labelPreview.setPixmap( picture );
        m_labelPreviewWidthText.setText( tr( "Width:" ) );
        m_labelPreviewHeightText.setText( tr( "Height:" ) );
        m_labelPreviewRatioText.setText( tr( "Ratio:" ) );
        m_labelPreviewWidth.setText( tr( "%1 px" ).arg( x ) );
        m_labelPreviewHeight.setText( tr( "%1 px" ).arg( y ) );
        m_labelPreviewRatio.setText( tr( "%1" ).arg( ratio,0,'f',3 ) );
    }
    else
    {
        m_labelPreview.clear();
        m_labelPreviewWidthText.clear();
        m_labelPreviewHeightText.clear();
        m_labelPreviewRatioText.clear();
        m_labelPreviewWidth.clear();
        m_labelPreviewHeight.clear();
        m_labelPreviewRatio.clear();
    }
}
