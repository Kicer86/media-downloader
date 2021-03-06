/*
 *
 *  Copyright (c) 2021
 *  name : Francis Banyikwa
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#include "tableWidget.h"
#include "utility.h"
#include "downloadmanager.h"

#include <QHeaderView>

void tableWidget::setDownloadingOptions( int row,const QString& mm,const QString& title )
{
	auto& item = this->uiTextItem( row ) ;

	auto txt = item.text() ;

	auto m = [ & ](){

		if( title.isEmpty() ){

			return mm ;
		}else{
			if( title.size() > 32 ){

				return title.mid( 0,32 ) + " ..." ;
			}else{
				return title ;
			}
		}
	}() ;

	auto u = QObject::tr( "Download Options" ) + " : " ;

	if( txt.startsWith( u ) ){

		auto s = txt.indexOf( '\n' ) ;
		item.setText( u + m + "\n" + txt.mid( s + 1 ) ) ;
	}else{
		item.setText( u + m + "\n" + item.text() ) ;
	}

	this->downloadingOptionsItem( row ).setText( mm ) ;
}

void tableWidget::setTableWidget( const tableWidget::tableWidgetOptions& s )
{
	m_table.verticalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents ) ;

	m_table.verticalHeader()->setMinimumSectionSize( 30 ) ;

	m_table.horizontalHeader()->setStretchLastSection( true ) ;

	m_table.setMouseTracking( s.mouseTracking ) ;

	m_table.setContextMenuPolicy( s.customContextPolicy ) ;

	m_table.setEditTriggers( s.editTrigger ) ;
	m_table.setFocusPolicy( s.focusPolicy ) ;
	m_table.setSelectionMode( s.selectionMode ) ;
}

void tableWidget::replace( const QPixmap& p,const QStringList& list,int row,int alignment )
{
	auto label = new QLabel() ;
	label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter ) ;
	label ->setPixmap( p ) ;

	m_table.setCellWidget( row,0,label ) ;

	for( int i = 0 ; i < list.size() ; i++ ){

		auto item = m_table.item( row,i + 1 ) ;
		item->setText( list.at( i ) ) ;
		item->setTextAlignment( alignment ) ;
	}
}

int tableWidget::addRow()
{
	auto row = m_table.rowCount() ;

	m_table.insertRow( row ) ;

	for( int i = 0 ; i < m_table.columnCount() ; i++ ){

		auto item = new QTableWidgetItem() ;

		m_table.setItem( row,i,item ) ;
	}

	return row ;
}

int tableWidget::addItem( const QPixmap& p,const QStringList& list,int alignment )
{
	auto row = m_table.rowCount() ;

	m_table.insertRow( row ) ;

	m_table.setItem( row,m_table.columnCount() -1,new QTableWidgetItem() ) ;

	for( int i = 0 ; i < list.size() ; i++ ){

		auto item = new QTableWidgetItem() ;

		item->setText( list.at( i ) ) ;

		item->setTextAlignment( alignment ) ;

		m_table.setItem( row,i + 1,item ) ;
	}

	auto label = new QLabel() ;
	label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter ) ;
	label ->setPixmap( p ) ;

	m_table.setCellWidget( row,0,label ) ;

	return row ;
}

void tableWidget::selectRow( QTableWidgetItem * current,QTableWidgetItem * previous,int firstColumnNumber )
{
	if( current && previous && previous->row() == current->row() ){

		auto table = current->tableWidget() ;

		table->setCurrentCell( current->row(),table->columnCount() - 1 ) ;
	}else{
		auto _update_table_row = [ & ]( QTableWidgetItem * item,bool setSelected ){

			if( item ){

				auto table = item->tableWidget() ;

				auto row = item->row() ;
				auto col = table->columnCount() ;

				for( int i = firstColumnNumber ; i < col ; i++ ){

					table->item( row,i )->setSelected( setSelected ) ;
				}

				if( setSelected ){

					table->setCurrentCell( row,col - 1 ) ;
				}

				table->setFocus() ;
			}
		} ;

		_update_table_row( current,true ) ;
		_update_table_row( previous,false ) ;
	}
}

void tableWidget::addItem( const QString& text,int alignment )
{
	this->addItem( QStringList{ text },alignment ) ;
}

void tableWidget::clear()
{
	int m = m_table.rowCount() ;

	for( int i = 0 ; i < m ; i++ ){

		m_table.removeRow( 0 ) ;
	}
}

void tableWidget::setVisible( bool e )
{
	m_table.setVisible( e ) ;
}

int tableWidget::rowCount() const
{
	return m_table.rowCount() ;
}

void tableWidget::selectLast()
{
	if( m_table.rowCount() > 0 ){

		m_table.setCurrentCell( m_table.rowCount() - 1,m_table.columnCount() - 1 ) ;
	}
}

void tableWidget::setEnabled( bool e )
{
	m_table.setEnabled( e ) ;
}

int tableWidget::currentRow() const
{
	return m_table.currentRow() ;
}

void tableWidget::removeRow( int s )
{
	m_table.removeRow( s ) ;
}

bool tableWidget::noneAreRunning()
{
	for( int i = 0 ; i < m_table.rowCount() ; i++ ){

		if( downloadManager::finishedStatus::running( this->runningState( i ) ) ){

			return false ;
		}
	}

	return true ;
}

void tableWidget::selectMediaOptions( QStringList& optionsList,
				      QTableWidgetItem& item,
				      QLineEdit& opts )
{
	if( item.isSelected() ){

		auto text = this->item( item.row(),0 ).text() ;

		if( !optionsList.contains( text ) ){

			optionsList.append( text ) ;
		}
	}

	for( int row = 0 ; row < this->rowCount() ; row++ ){

		auto& item = this->item( row,0 ) ;

		if( !item.isSelected() ){

			optionsList.removeAll( item.text() ) ;
		}
	}

	if( optionsList.isEmpty() ){

		opts.clear() ;
	}else{
		opts.setText( optionsList.join( "+" ) ) ;
	}
}

void tableWidget::showOptions( const engines::engine& engine,const QList<QByteArray>& args )
{
	QStringList m ;

	utility::make_reverseIterator( args ).forEach( [ & ]( const QByteArray& s ){

		auto a = utility::split( s,' ',true ) ;

		if( a.size() > 1 ){

			if( engine.breakShowListIfContains( a ) ){

				return true ;
			}else{
				m.insert( 0,s ) ;
			}
		}

		return false ;
	} ) ;

	for( const auto& it : m ){

		auto a = utility::split( it,' ',true ) ;

		if( a.size() > 3 ){

			auto format     = a.takeAt( 0 ) ;
			auto extension  = a.takeAt( 0 ) ;
			auto resolution = a.takeAt( 0 ) ;
			auto notes      = a.join( " " ) ;

			QStringList args{ format,extension,resolution,notes } ;

			this->addItem( args ) ;
		}
	}
}

tableWidget::tableWidget( QTableWidget& t,const QFont& font,int init ) :
	m_table( t ),
	m_font( font ),
	m_init( init )
{
	this->setTableWidget( tableWidget::tableWidgetOptions() ) ;
}

QTableWidgetItem& tableWidget::item( int row,int column ) const
{
	return *m_table.item( row,column ) ;
}

QTableWidget& tableWidget::get()
{
	return m_table ;
}

void tableWidget::addItem( const QStringList& text,int alignment )
{
	auto row = m_table.rowCount() ;

	m_table.insertRow( row ) ;

	m_table.setItem( row,m_table.columnCount() -1,new QTableWidgetItem() ) ;

	for( int it = 0 ; it < text.size() ; it++ ){

		auto item = new QTableWidgetItem() ;

		item->setText( text.at( it ) ) ;
		item->setTextAlignment( alignment ) ;
		item->setFont( m_font ) ;

		m_table.setItem( row,it,item ) ;
	}
}
