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

#include "CustomWebEnginePage.h"
#include <QWebEnginePage>
#include <QWebEngineNewWindowRequest>
#include <QWebEngineView>

CustomWebEnginePage::CustomWebEnginePage(QWebEngineProfile* profile, QObject* parent) :
    QWebEnginePage(profile,parent),
    m_view(nullptr)
{
    connect(this,SIGNAL(newWindowRequested(QWebEngineNewWindowRequest&)),this,SLOT(slotNewWindowRequested(QWebEngineNewWindowRequest&)));
}

CustomWebEnginePage::~CustomWebEnginePage()
{
    if ( m_view != nullptr ) {
        m_view->close();
        delete m_view;
    }
}

bool CustomWebEnginePage::acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame)
{
    // via navi no extra view
    return QWebEnginePage::acceptNavigationRequest(url,type,isMainFrame);
}

void CustomWebEnginePage::slotNewWindowRequested(QWebEngineNewWindowRequest &request)
{
    if ( m_view == nullptr )
    {  // only allow 1 extra view
        m_view = new QWebEngineView();
        connect(m_view,SIGNAL(signalClosed()),this,SLOT(slotWindowClosed()));
        m_view->setUrl(request.requestedUrl());
        m_view->show();
    }
}

void CustomWebEnginePage::slotWindowClosed()
{
    disconnect(m_view,SIGNAL(signalClosed));
    delete m_view;
    m_view = nullptr;
}
