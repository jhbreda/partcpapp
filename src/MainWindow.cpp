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

#include "MainWindow.h"
#include "CustomWebEnginePage.h"
#include <QGuiApplication>
#include <QApplication>
#include <QScreen>
#include <QFile>
#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QUrl>
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>

MainWindow::MainWindow(const QMap<QString,QList<QTranslator*>>& translators,QWidget *parent) :
  QMainWindow(parent),
  m_translators(translators)
{
    applySettings();

    setupGui();

    m_webView = new QWebEngineView();
    m_webPage = new CustomWebEnginePage(new QWebEngineProfile());
    m_webView->setPage(m_webPage);

    setCentralWidget( m_webView );

    connect(m_webView,SIGNAL(loadStarted()),this,SLOT(slotLoadStarted()));
    connect(m_webView,SIGNAL(loadProgress(int)),this,SLOT(slotLoadProgress(int)));
    connect(m_webView,SIGNAL(loadFinished(bool)),this,SLOT(slotLoadFinished(bool)));
    connect(m_webView,SIGNAL(selectionChanged()),this,SLOT(slotSelectionChanged()));
    connect(m_webView,SIGNAL(urlChanged(QUrl)),this,SLOT(slotUrlChanged(QUrl)));

    m_webView->setUrl(QUrl(m_partcpUrl));
}

namespace
{
    void applyStyle(QString style)
    {
        QFile f(":" + style + "/" + style + ".qss");

        if (f.exists())
        {
            f.open(QFile::ReadOnly | QFile::Text);
            QTextStream ts(&f);
            qApp->setStyleSheet(ts.readAll());
            f.close();
        }
        else
        {
            qApp->setStyleSheet(QString());
        }
    }
}

void MainWindow::setupGui()
{
    menuBar()->clear();

    applyStyle(m_mode);

    foreach( QTranslator* translator, m_translators.value(m_language))
    {
        QCoreApplication::installTranslator(translator);
    }

    m_locales.clear();
    m_modes.clear();

    m_locales = {{"nl_NL",tr("nl_NL")}, {"de_DE",tr("de_DE")}, {"en_GB",tr("en_GB")}, {"fr_FR",tr("fr_FR")},{"sv_SV",tr("sv_SV")}};
    m_modes = {{"lightsoff",tr("lightsoff")},{"lightson",tr("lightson")},{"system",tr("system")}};

    QMenu* menu = menuBar()->addMenu(tr("File"));
    menu->addAction(tr("Quit"),this,SLOT(close()));
    QMenu* options = menuBar()->addMenu(tr("Options"));
    QMenu* languageMenu = new QMenu(tr("Language"));
    foreach( const auto& key, m_locales.keys())
    {
        QAction* action = languageMenu->addAction(m_locales.value(key),this,SLOT(slotLanguage()));
        action->setObjectName(key);
        action->setCheckable(true);
        action->setChecked(key == m_language);
    }
    options->addMenu(languageMenu);
    QMenu* modeMenu = new QMenu(tr("gui-mode"));
    foreach( const auto& key, m_modes.keys())
    {
        QAction* action = modeMenu->addAction(m_modes.value(key),this,SLOT(slotMode()));
        action->setObjectName(key);
        action->setCheckable(true);
        action->setChecked(key == m_mode);
    }
    options->addMenu(modeMenu);
    menu = menuBar()->addMenu(tr("Help"));
    menu->addAction(tr("About"),this,SLOT(slotAbout()));
    menu->addAction(tr("About Qt"),qApp,SLOT(aboutQt()));
}

MainWindow::~MainWindow()
{
    delete m_webPage;  // muss sein, wird nicht von Qt gemacht.
    saveSettings();
}

void MainWindow::slotAbout()
{
    QMessageBox::information( this, "partcp-client", tr("<strong>About partcp-client</strong><BR><BR> "
                                    "This Application is a wrapper for the partcp-webapplication<BR><BR>"
                                    "The program is provided AS IS with NO WARRANTY OF ANY KIND, "
                                    "INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE"));
}

namespace
{
    QString getLocale(const QStringList locales)
    {
        const QStringList uiLanguages = QLocale::system().uiLanguages();
        for (const QString &locale : uiLanguages)
        {
            if ( locales.contains(QLocale(locale).name()))
            {
                return QLocale(locale).name();
            }
        }
        return QStringLiteral("de_DE");
    }
}

void MainWindow::applySettings()
{
    QSettings settings;

    settings.beginGroup("partcp");
    m_partcpUrl = settings.value("partcl_url","https://ogx.de/partcp/").toString();
    m_language = settings.value("language", getLocale(m_locales.keys())).toString();
    m_mode = settings.value("mode", "system").toString();
    settings.endGroup();
    settings.beginGroup("window");
    QRect r = QGuiApplication::primaryScreen()->availableGeometry();
    setGeometry( settings.value( "geometry", r.adjusted(r.width()/5,r.height()/5,r.width()/5,-r.height()/5) ).toRect() );
    settings.endGroup();
}

void MainWindow::saveSettings()
{
    QSettings settings;

    settings.beginGroup("partcp");
    settings.setValue("partcp_url", m_partcpUrl);
    settings.setValue("language",m_language);
    settings.setValue("mode",m_mode);
    settings.endGroup();
    settings.beginGroup("window");
    settings.setValue( "geometry", geometry() );
    settings.endGroup();
}

void MainWindow::slotLoadProgress(int progress)
{
    if ( progress != 100 )
    {
        statusBar()->showMessage(tr("Load progress: ") + QString::number( progress ) + tr(" percent.") );
    }
}

void MainWindow::slotLoadFinished(bool isOk )
{
    if ( ! isOk )
    {
        statusBar()->showMessage(tr("Could not load partcp application (") + m_webView->url().toString() + ")" );
    }
    else
    {
        statusBar()->showMessage(tr("Partcp application has been loaded."));
    }
}

void MainWindow::slotLoadStarted()
{
    statusBar()->showMessage(tr("Loading of the partcp application has started..."));
}

void MainWindow::slotUrlChanged( const QUrl& url )
{
    statusBar()->showMessage(tr("Current url has changed to: ") + url.toString() );
}

void MainWindow::slotLanguage()
{
    foreach( QTranslator* translator, m_translators.value(m_language))
    {
        QCoreApplication::removeTranslator(translator);
    }

    m_language = sender()->objectName();

    setupGui();
}

void MainWindow::slotMode()
{
    m_mode = sender()->objectName();
    setupGui();
}

void MainWindow::slotSelectionChanged()
{
    statusBar()->showMessage(tr("Selected text has changed to: ") + m_webView->selectedText() );
}




