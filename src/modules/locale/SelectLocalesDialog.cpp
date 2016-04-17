/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
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

#include "SelectLocalesDialog.h"
#include "ui_SelectLocalesDialog.h"

SelectLocalesDialog::SelectLocalesDialog( QWidget* parent ) :
    QDialog( parent ),
    ui( new Ui::SelectLocalesDialog )
{
    ui->setupUi( this );

    // Connect signals and slots
    connect( ui->languageListView, &QListView::clicked,
             this, &SelectLocalesDialog::hideLocaleComboBox );
    connect( ui->countryListView, &QListView::clicked,
             this, &SelectLocalesDialog::showLocaleComboBox );
    connect( ui->buttonCancel, &QPushButton::clicked,
             this, &SelectLocalesDialog::close );
    connect( ui->buttonAdd, &QPushButton::clicked,
             this, &SelectLocalesDialog::buttonAdd_clicked );
}


SelectLocalesDialog::~SelectLocalesDialog()
{
    delete ui;
    delete m_supportedLocalesModel;
    delete m_languageSortProxy;
    delete m_countrySortProxy;
    delete m_languageSelectionProxy;
    delete m_countrySelectionProxy;
}


bool
SelectLocalesDialog::localeAdded()
{
    return m_accepted;
}


QString
SelectLocalesDialog::getLocale()
{
    if ( ui->localeComboBox->count() <= 0 )
        return QString();
    return ui->localeComboBox->currentText();
}


int
SelectLocalesDialog::exec()
{
    m_accepted = false;
    m_supportedLocalesModel = new SupportedLocalesModel();

    m_languageSortProxy = new QSortFilterProxyModel();
    m_languageSortProxy->setSourceModel( m_supportedLocalesModel );
    m_languageSortProxy->setSortLocaleAware( true );
    m_languageSortProxy->sort( 0, Qt::AscendingOrder );
    ui->languageListView->setModel( m_languageSortProxy );

    m_languageSelectionProxy = new KSelectionProxyModel( ui->languageListView->selectionModel(), this );
    m_languageSelectionProxy->setSourceModel( m_supportedLocalesModel );
    m_languageSelectionProxy->setFilterBehavior( KSelectionProxyModel::ChildrenOfExactSelection );

    m_countrySortProxy = new QSortFilterProxyModel();
    m_countrySortProxy->setSourceModel( m_languageSelectionProxy );
    m_countrySortProxy->setSortLocaleAware( true );
    m_countrySortProxy->sort( 0, Qt::AscendingOrder );
    ui->countryListView->setModel( m_countrySortProxy );

    m_countrySelectionProxy = new KSelectionProxyModel( ui->countryListView->selectionModel(), this );
    m_countrySelectionProxy->setSourceModel( m_supportedLocalesModel );
    m_countrySelectionProxy->setFilterBehavior( KSelectionProxyModel::ChildrenOfExactSelection );
    ui->localeComboBox->setModel( m_countrySelectionProxy );

    ui->localeComboBox->hide();
    updateApplyEnabledState();

    return QDialog::exec();
}


void
SelectLocalesDialog::updateApplyEnabledState()
{
    ui->buttonAdd->setEnabled( ui->localeComboBox->isVisible() );
}


void
SelectLocalesDialog::hideLocaleComboBox( const QModelIndex& index )
{
    if ( index.isValid() )
    {
        ui->localeComboBox->hide();
        updateApplyEnabledState();
    }
}


void
SelectLocalesDialog::showLocaleComboBox( const QModelIndex& index )
{
    if ( index.isValid() )
    {
        /* Select locale with UTF-8 encoding by default */
        QAbstractItemModel* model = ui->localeComboBox->model();
        QModelIndexList localeIndexList = model->match( model->index( 0,0 ),
                                          SupportedLocalesModel::ValueRole,
                                          "UTF-8",
                                          -1,
                                          Qt::MatchContains );
        if ( localeIndexList.size() > 0 )
        {
            QModelIndex modelIndex = localeIndexList.first();
            ui->localeComboBox->setCurrentIndex( modelIndex.row() );
        }
        ui->localeComboBox->show();
        updateApplyEnabledState();
    }
}


void
SelectLocalesDialog::buttonAdd_clicked()
{
    m_accepted = true;
    close();
}
