/***************************************************************************
 *   Copyright (C) 2022 by Ir. Johan H van Breda                           *
 *   Johan.van.Breda@protonmail.com                                        *
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QMap>

class QLineEdit;
class QWebEngineView;
class CustomWebEnginePage;
class QLabel;
class QTranslator;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(const QMap<QString,QList<QTranslator*>>& translators, QWidget *parent = 0);
  ~MainWindow();

protected:
  void setupGui();
  void applySettings();
  void saveSettings();

protected slots:
  void slotAbout();

  void slotLanguage();
  void slotMode();
  void slotLoadStarted();
  void slotLoadProgress(int progress);
  void slotLoadFinished(bool isOk );
  void slotUrlChanged( const QUrl& url );
  void slotSelectionChanged();

private:
  QMap<QString,QString> m_locales;
  QMap<QString,QString> m_modes;
  QMap<QString,QList<QTranslator*>> m_translators;

  QString m_partcpUrl;
  QString m_mode;
  QString m_language;
  QWebEngineView*  m_webView;
  CustomWebEnginePage* m_webPage;
};

#endif // MAINWINDOW_H
