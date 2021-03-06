/***************************************************************************
*   Copyright (C) 2005 by wolfgang loeffler   *
*   wolfgang.loeffler@entropia.biz   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/


#include "MainWindow.h"

#include "ui_MainWindow.h"
#include "getterapod.h"
#include "getterepod.h"
#include "getter.h"
#include <QString>
#include <QImage>
#include <QSettings>
#include <QFileDialog>
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QStringList>
#include <QStringListModel>
#include <QItemDelegate>
#include <QAbstractItemDelegate>
#include <QPainter>
#include <QProcess>
#include <QtGui>

#include "web.h"
#include "qeeventloop.h"

#ifdef WIN32
#include <windows.h>
#endif


MyModel::MyModel( QObject *parent ) : QAbstractListModel( parent ) {
  //      nameList = QStringList();
}

int MyModel::rowCount( const QModelIndex &parent ) const {
  return nameList.count();
}

void MyModel::addItem( QString sss ) {
  nameList.append( sss );
}

void MyModel::clearAll() {
  nameList.clear();
}

QVariant MyModel::data( const QModelIndex &index, int role ) const {
  if ( !index.isValid() )
    return QVariant();

  if ( index.row() < 0 || index.row() >= nameList.size() )
    return QVariant();

  if ( role == Qt::DisplayRole ) {
    return nameList.at( index.row() );
  } else return QVariant();
}

MyDelegate::MyDelegate( QObject * parent ) : QAbstractItemDelegate( parent ) {}

void MyDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
  QString name = index.model() ->data( index, Qt::DisplayRole ).toString();

  // selection
  if ( option.state & QStyle::State_Selected )
    painter->setBrush( option.palette.highlight() );
  else
    painter->setBrush( QBrush( Qt::white ) );
  painter->drawRect( option.rect );

  // image
  QImage img( basedir + "/" + name + "-thumb.jpg" );
  painter->drawImage( option.rect.topLeft().x() + ICONWIDTH / 2 - img.width() / 2,
                      option.rect.topLeft().y() + ICONHEIGHT / 2 - img.height() / 2,
                      img );
}

QSize MyDelegate::sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const {
  return QSize( ICONWIDTH, ICONHEIGHT );
}

void MyDelegate::setDir( QString dir ) {
  basedir = dir;
}

MainWindow::MainWindow( int argc, char *argv[], QWidget *parent ) : QMainWindow( parent ) {
  eventLoop = NULL;
  modeAuto = false;
  ui.setupUi( this );
  ui.desktopType->addItem( "KDE", "kde" );
  ui.desktopType->addItem( "w2k/xp", "winxp" );

  ui.sourceType->addItem( "Astronomic POD", "apod" );
  ui.sourceType->addItem( "Earth Science POD", "epod" );

  connect( ui.updateNow, SIGNAL( clicked() ), this, SLOT( updateImage() ) );
  connect( ui.updateAll, SIGNAL( clicked() ), this, SLOT( updateAll() ) );
  connect( ui.pbSaveSettings, SIGNAL( clicked() ), this, SLOT( saveSettings() ) );
  connect( ui.pbBrowseImageLocation, SIGNAL( clicked() ), this, SLOT( browseImageLocation() ) );
  connect( ui.pbDeleteSelected, SIGNAL( clicked() ), this, SLOT( deleteSelected() ) );
  connect( ui.pbSetBackground, SIGNAL( clicked() ), this, SLOT( setAsBackground() ) );
  connect( ui.installDesktop, SIGNAL( clicked() ), this, SLOT( installDesktop() ) );
  connect( ui.installAutostart, SIGNAL( clicked() ), this, SLOT( installAutostart() ) );
  connect( ui.removeDesktop, SIGNAL( clicked() ), this, SLOT( removeDesktop() ) );
  connect( ui.removeAutostart, SIGNAL( clicked() ), this, SLOT( removeAutostart() ) );
  connect( ui.textBrowser, SIGNAL( anchorClicked ( const QUrl & ) ), this, SLOT( anchorClicked ( const QUrl & ) ) );


  QCoreApplication::setOrganizationName( "MySoft" );
  QCoreApplication::setOrganizationDomain( "wollewolle.de" );
  QCoreApplication::setApplicationName( "qapod" );


  settings = new QSettings( QCoreApplication::applicationDirPath () + "/qapod.ini", QSettings::IniFormat );
  if ( !settings->contains( "imagelocation" ) ) {
    settings->setValue( "imagelocation", "/tmp/apod" );
    ui.tabWidget->setCurrentIndex( 2 );
  }
  ui.tabWidget->setCurrentIndex( 0 );
  ui.imageLocation->setText( settings->value( "imagelocation" ).toString() );
  if ( !settings->contains( "sourcetype" ) ) settings->setValue( "sourcetype", "apod" );
  ui.sourceType->setCurrentIndex( ui.sourceType->findData( settings->value( "sourcetype" ).toString() ) );
  if ( !settings->contains( "bgwidth" ) ) settings->setValue( "bgwidth", "1280" );
  ui.bgWidth->setText( settings->value( "bgwidth" ).toString() );
  if ( !settings->contains( "bgheight" ) ) settings->setValue( "bgheight", "1024" );
  ui.bgHeight->setText( settings->value( "bgheight" ).toString() );
  if ( !settings->contains( "thumbwidth" ) ) settings->setValue( "thumbwidth", "100" );
  ui.thumbWidth->setText( settings->value( "thumbwidth" ).toString() );
  if ( !settings->contains( "thumbheight" ) ) settings->setValue( "thumbheight", "100" );
  ui.thumbHeight->setText( settings->value( "thumbheight" ).toString() );
  if ( !settings->contains( "desktop" ) ) settings->setValue( "desktop", "kde" );
  ui.desktopType->setCurrentIndex( ui.desktopType->findData( settings->value( "desktop" ).toString() ) );
  if ( !settings->contains( "autoupdateimage" ) ) settings->setValue( "autoupdateimage", "0" );
  ui.autoUpdateImage->setCheckState( ( Qt::CheckState ) settings->value( "autoupdateimage" ).toInt() );
  if ( !settings->contains( "autobackground" ) ) settings->setValue( "autobackground", "0" );
  ui.autoBackground->setCheckState( ( Qt::CheckState ) settings->value( "autobackground" ).toInt() );
  if ( !settings->contains( "alwayssetbackground" ) ) settings->setValue( "alwayssetbackground", "0" );
  ui.alwaysSetBackground->setCheckState( ( Qt::CheckState ) settings->value( "alwayssetbackground" ).toInt() );
  if ( !settings->contains( "autoclose" ) ) settings->setValue( "autoclose", "0" );
  ui.autoClose->setCheckState( ( Qt::CheckState ) settings->value( "autoclose" ).toInt() );


  ui.progressBar->setValue( 0 );

  // command line parsing
  QStringList args;
  for ( int iii = 0; iii < argc; iii++ ) args.append( argv[ iii ] );
  if ( args.contains( "auto" ) ) modeAuto = true;
  if ( modeAuto ) qDebug() << "auto!";

  // update list, would be useless if autoclose
  if ( !( modeAuto && ( settings->value( "autoclose" ).toInt() == 2 ) ) ) updateList();

  // update image
  if ( modeAuto && ( settings->value( "autoupdateimage" ).toInt() == 2 ) ) updateImage();
  
  // set Help Text
  ui.helpText->setHtml("<html><H1>How to get started:</H1><br>"
      "<font size=4><ol>"
      "<li> Select the <b>Settings</b> tab:"
      "<ol>"
      "<li> Type in or browse for the <b>Image location</b>"
      "<li> Select your desired image <b>Source</b>"
      "<li> Install the <b>Autostart Entry</b>, then this Program is started on login."
      "<p>If you do so, check the appropriate Checkboxes what to do if it is auto-started (likely all three)"
      "<li> Install a <b>Desktop Icon</b> to easily start this program"
      "<li> Press <b>Save Settings</b>"
      "</ol>"
      "<br>"
      "<li> Select the <b>Main</b> tab:"
      "<ol>"
      "<li>Press <b>Update</b> to update the image!"
      "<li>Press <b>Set as Background</b> to set the actual selected image as background!"
      "</ol>"
      "</ol></font>"
      "<hr>http://qapod.berlios.de"
      "</html>");
}

void MainWindow::saveSettings() {
  settings->setValue( "imagelocation", ui.imageLocation->text() );
  settings->setValue( "sourcetype", ui.sourceType->itemData( ui.sourceType->currentIndex() ) );
  settings->setValue( "bgwidth", ui.bgWidth->text() );
  settings->setValue( "bgheight", ui.bgHeight->text() );
  settings->setValue( "thumbwidth", ui.thumbWidth->text() );
  settings->setValue( "thumbheight", ui.thumbHeight->text() );
  settings->setValue( "desktop", ui.desktopType->itemData( ui.desktopType->currentIndex() ).toString() );
  settings->setValue( "autoupdateimage", QString::number( ui.autoUpdateImage->checkState() ) );
  settings->setValue( "autobackground", QString::number( ui.autoBackground->checkState() ) );
  settings->setValue( "alwayssetbackground", QString::number( ui.alwaysSetBackground->checkState() ) );
  settings->setValue( "autoclose", QString::number( ui.autoClose->checkState() ) );
}

void MainWindow::listViewclicked( const QModelIndex & index ) {
  QString name = index.model() ->data( index, Qt::DisplayRole ).toString();
  ui.imageName->setText( name );
  ui.textBrowser->clear();
  ui.textBrowser->setSource( QUrl::fromLocalFile( settings->value( "imagelocation" ).toString() + "/" + name + ".txt" ) );
}


void MainWindow::browseImageLocation() {
  QFileDialog * fd = new QFileDialog();
  fd->setFileMode( QFileDialog::DirectoryOnly );
  fd->setAcceptMode( QFileDialog::AcceptOpen );
  if ( fd->exec() ) {
    QStringList sl = fd->selectedFiles();
    if ( sl.size() > 0 ) {
      ui.imageLocation->setText( sl.at( 0 ) );
    }

  }
}

void MainWindow::updateImage( QString st ) {
  qDebug() << "update " + st;
  if ( eventLoop == NULL ) {
    eventLoop = new QeEventLoop();
    qDebug() << "eventLoop started";
  }

  getter = NULL;

  if ( st == "" ) st = settings->value( "sourcetype" ).toString() ; // old mode

  if ( st == "apod" ) getter = new GetterAPOD( 0, settings , st );
  else if ( st == "epod" ) getter = new GetterEPOD( 0, settings , st );

  if ( getter != NULL ) {
    connect( getter, SIGNAL( updateProgress( const QString&, int, int ) ), this, SLOT( updateProgress( const QString&, int, int ) ) );
    connect( getter, SIGNAL( updateFinished( bool, QString, QString ) ), this, SLOT( updateImageDone( bool, QString, QString ) ) );
    getter->moveToThread( eventLoop );
  }
}

void MainWindow::updateAll() {
  for ( int iii = 0; iii < ui.sourceType->count(); iii++ ) {
    QString st = ui.sourceType->itemData( iii ).toString();
    updateImage( st );
  }

}


void MainWindow::updateList() {
  model.clearAll();
  QDir dir( settings->value( "imagelocation" ).toString() );
  dir.setFilter( QDir::Files | QDir::NoSymLinks );
  dir.setSorting( QDir::Name | QDir::Reversed );
  QStringList sl = dir.entryList();
  for ( int i = 0; i < sl.size(); ++i ) {
    if ( sl.at( i ).endsWith( ".jpg" ) && !sl.at( i ).endsWith( "thumb.jpg" ) ) {
      QString slentry = sl.at( i );
      model.addItem( slentry.left( slentry.indexOf( ".jpg" ) ) );
    }
  }
  ui.listView->setModel( &model );
  delegate.setDir( settings->value( "imagelocation" ).toString() );
  ui.listView->setItemDelegate( &delegate );
  ui.listView->repaint();
  
  // cannot be done before!
  connect( ui.listView->selectionModel(), SIGNAL(currentChanged ( const QModelIndex &, const QModelIndex &)), this, SLOT( listViewclicked( const QModelIndex & ) ) );

  QModelIndex mi = model.index(settings->value("recentimagerow").toInt(),0);
  ui.listView->setCurrentIndex(mi);
}

void MainWindow::updateImageDone( bool havenew, const QString& fn, const QString& updateResult ) {
  qDebug() << "updateImageDone";
  ui.logText->setHtml(ui.logText->toHtml() + updateResult);
  if ( havenew ) {
    // autoupdate?
    if ( modeAuto && ( settings->value( "autobackground" ).toInt() == 2 ) ) {
      setBackground( fn + ".jpg" );
      settings->setValue( "recentimagerow" , 0);
    }
  } else {
    // hack for network drives, w2k doesnt set background if img on network drive
    if ( modeAuto && ( settings->value( "autobackground" ).toInt() == 2 ) && ( settings->value( "alwayssetbackground" ).toInt() == 2 ) ) {
      setBackground( settings->value("currentwallpaper").toString() );
    }
  }
  // autoclose?
  if ( modeAuto && ( settings->value( "autoclose" ).toInt() == 2 ) ) close();

  updateList();
}


void MainWindow::updateProgress(const QString& st, int bytesRead, int totalBytes ) {
  ui.logText->setHtml(st + " progress: " + QString::number((bytesRead * 100) / totalBytes) );
  ui.progressBar->setMaximum( totalBytes );
  ui.progressBar->setValue( bytesRead );
}


void MainWindow::deleteSelected() {
  QStringList names;

  foreach ( QModelIndex index, ui.listView->selectionModel () ->selectedIndexes () ) {
    names.append( index.model() ->data( index, Qt::DisplayRole ).toString() + "*" );
  }

  QDir dir( settings->value( "imagelocation" ).toString() );
  dir.setFilter( QDir::Files | QDir::NoSymLinks );
  dir.setNameFilters( names );

  foreach ( QString name, dir.entryList() ) {
    QString pathname = settings->value( "imagelocation" ).toString() + "/" + qPrintable( name );
    qDebug() << "delete " << qPrintable( pathname );
    QFile::remove( pathname );
  }

  updateList();
}

void MainWindow::setAsBackground() {
  QModelIndex mi = ui.listView->selectionModel () ->selectedIndexes ().first();
  QString name = mi.model() ->data( mi, Qt::DisplayRole ).toString();

  setBackground( settings->value( "imagelocation" ).toString() + "/" + qPrintable( name ) + ".jpg" );
  
    // TODO:
  settings->setValue( "recentimagerow" , mi.row());
  

}

QString getShellRes( QString cmd, QStringList args ) {
  QProcess proc;
  proc.start( cmd, args );
  if ( !proc.waitForStarted() ) { qDebug() << "err1"; return ""; }
  if ( !proc.waitForFinished() ) { qDebug() << "err2"; return ""; }
  QByteArray result = proc.readAll();
  return QString( result );
}

#ifdef WIN32
QString systemErrorToString( DWORD lastErr ) {
  char buf[ 1025 ];

  DWORD lastError = lastErr;
  if ( lastErr == 0 )
    lastError = GetLastError();
  DWORD result = ::FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, NULL, lastError, LANG_USER_DEFAULT, buf, sizeof( buf ), NULL );
  return QString( "Error %1 : %2" ).arg( lastError ).arg( buf );
}
#endif

void MainWindow::setBackground( QString name ) {
  settings->setValue( "currentwallpaper", name );
  if ( settings->value( "desktop" ) == "kde" ) {
    getShellRes( "dcop", QStringList() << "kdesktop" << "KBackgroundIface" << "setWallpaper" << name << "6" );
  } else if ( settings->value( "desktop" ) == "winxp" ) {
#ifdef WIN32
    QString winfile = settings->value( "imagelocation" ).toString() + "/qapod.bmp";
    QImage img( name );
    img.save( winfile, "bmp", 0 );
    QByteArray local = QDir::convertSeparators( winfile ).toLocal8Bit();
    char *sss = local.data();
    long res = SystemParametersInfoA( SPI_SETDESKWALLPAPER, 0, sss, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE );   //SPIF_SENDCHANGE |
    if ( res == 0 )
      QMessageBox::information( this, "yoo", systemErrorToString( GetLastError() ) );
#endif

  }
}


void MainWindow::installDesktop() {
  if ( settings->value( "desktop" ) == "kde" ) {
    QString desktopPath = getShellRes( "kde-config", QStringList() << "--userpath" << "desktop" );
    desktopPath = desktopPath.simplified();
    QFile dfile( desktopPath + "/qapod.sh" );
    if ( dfile.open( QFile::WriteOnly | QFile::Truncate ) ) {
      QTextStream out( &dfile );
      out << qPrintable( "cd " + QCoreApplication::applicationDirPath () + " && ./qapod" );
      dfile.setPermissions( QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner );
    } else qDebug() << "unable to write to " << qPrintable( dfile.fileName() );
  } else if ( settings->value( "desktop" ) == "winxp" ) {
    QSettings registry( "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", QSettings::NativeFormat );
    QString path = registry.value( "Desktop" ).toString();
    QFile::link( QCoreApplication::applicationFilePath(), path + "/qapod.lnk" ); // lol
  }
}

void MainWindow::installAutostart() {
  if ( settings->value( "desktop" ) == "kde" ) {
    QString autoPath = getShellRes( "kde-config", QStringList() << "--userpath" << "autostart" );
    autoPath = autoPath.simplified();
    QFile dfile( autoPath + "/qapod.sh" );
    if ( dfile.open( QFile::WriteOnly | QFile::Truncate ) ) {
      QTextStream out( &dfile );
      out << qPrintable( "cd " + QCoreApplication::applicationDirPath () + " && ./qapod auto" );
      dfile.setPermissions( QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner );
    } else qDebug() << "unable to write to " << qPrintable( dfile.fileName() );
  } else if ( settings->value( "desktop" ) == "winxp" ) {
    QSettings registry( "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", QSettings::NativeFormat );
    QString path = registry.value( "Startup" ).toString();
    QFile::link( QCoreApplication::applicationFilePath() + "\" \"auto", path + "/qapod.lnk" ); // lol
    QMessageBox::information( this, "qapod", "Now go to " + path + " and right-click on qapod.lnk, select properties and press OK!" );
  }
}

void MainWindow::removeDesktop() {
  if ( settings->value( "desktop" ) == "kde" ) {
    QString desktopPath = getShellRes( "kde-config", QStringList() << "--userpath" << "desktop" );
    desktopPath = desktopPath.simplified();
    QFile dfile( desktopPath + "/qapod.sh" );
    dfile.remove();
  } else if ( settings->value( "desktop" ) == "winxp" ) {
    QSettings registry( "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", QSettings::NativeFormat );
    QString path = registry.value( "Desktop" ).toString();
    QFile::remove( path + "/qapod.lnk" );
  }
}

void MainWindow::removeAutostart() {
  if ( settings->value( "desktop" ) == "kde" ) {
    QString autoPath = getShellRes( "kde-config", QStringList() << "--userpath" << "autostart" );
    autoPath = autoPath.simplified();
    QFile dfile( autoPath + "/qapod.sh" );
    dfile.remove();
  } else if ( settings->value( "desktop" ) == "winxp" ) {
    QSettings registry( "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", QSettings::NativeFormat );
    QString path = registry.value( "Startup" ).toString();
    QFile::remove( path + "/qapod.lnk" ); // lol
  }
}

void MainWindow::anchorClicked ( const QUrl &link ) {
  ui.textBrowser->setSource( ui.textBrowser->source() ); // dont follow link here
  QString url = link.toString().mid( link.toString().indexOf( "http" ) );
  qDebug() << "open " << url;
  if ( settings->value( "desktop" ) == "kde" ) {
    QString sss = getShellRes( "kfmclient", QStringList() << "openURL" << url );
  } else if ( settings->value( "desktop" ) == "winxp" ) {
#ifdef WIN32
    ShellExecuteA( 0, "open", url.toAscii().data(), 0, 0, SW_SHOWNORMAL );
#endif

  }
}

