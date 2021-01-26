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

#include "playlistdownloader.h"
#include "tabmanager.h"

playlistdownloader::playlistdownloader()
{
}

void playlistdownloader::init( settings * settings,Ui::MainWindow * ui,QWidget * )
{
	m_ui = ui ;
	m_settings = settings ;

	this->resetMenu() ;

	connect( m_ui->pbPLDownload,&QPushButton::clicked,[ this ](){

		auto options = m_ui->lineEditPLUrlOptions->text() ;

		if( options.isEmpty() ){

			options = utility::selectedAction::bestText() ;
		}

		options += " --yes-playlist" ;

		auto items = m_ui->lineEditPLDownloadRange->text() ;

		if( !items.isEmpty() ){

			options += " --playlist-items " + items ;
		}

		auto url = m_ui->lineEditPLUrl->text() ;

		tabManager::instance().basicDownloader().download( options,url ) ;
	} ) ;

	connect( m_ui->pbPLQuit,&QPushButton::clicked,[](){

		tabManager::instance().basicDownloader().appQuit() ;
	} ) ;
}

void playlistdownloader::enableAll()
{
	m_ui->lineEditPLUrl->setEnabled( true ) ;
	m_ui->labelPLEnterOptions->setEnabled( true ) ;
	m_ui->labelPLEnterUrlRange->setEnabled( true ) ;
	m_ui->lineEditPLDownloadRange->setEnabled( true ) ;
	m_ui->lineEditPLUrl->setEnabled( true ) ;
	m_ui->lineEditPLUrlOptions->setEnabled( true ) ;
	m_ui->pbPLDownload->setEnabled( true ) ;
	m_ui->pbPLOptions->setEnabled( true ) ;
	m_ui->pbPLQuit->setEnabled( true ) ;
	m_ui->labelPLEnterUrl->setEnabled( true ) ;
}

void playlistdownloader::disableAll()
{
	m_ui->lineEditPLUrl->setEnabled( false ) ;
	m_ui->labelPLEnterOptions->setEnabled( false ) ;
	m_ui->labelPLEnterUrlRange->setEnabled( false ) ;
	m_ui->lineEditPLDownloadRange->setEnabled( false ) ;
	m_ui->lineEditPLUrl->setEnabled( false ) ;
	m_ui->lineEditPLUrlOptions->setEnabled( false ) ;
	m_ui->pbPLDownload->setEnabled( false ) ;
	m_ui->pbPLOptions->setEnabled( false ) ;
	m_ui->pbPLQuit->setEnabled( false ) ;
	m_ui->labelPLEnterUrl->setEnabled( false ) ;
}

void playlistdownloader::resetMenu()
{
	utility::setMenuOptions( m_settings,true,m_ui->pbPLOptions,[ this ]( QAction * aa ){

		utility::selectedAction ac( aa ) ;

		if( ac.clearOptions() ){

			m_ui->lineEditPLUrlOptions->clear() ;

		}else if( ac.clearScreen() ){

		}else{
			if( ac.best() ){

				m_ui->lineEditPLUrlOptions->setText( ac.bestText() ) ;
			}else{
				m_ui->lineEditPLUrlOptions->setText( ac.objectName() ) ;
			}
		}
	} ) ;
}