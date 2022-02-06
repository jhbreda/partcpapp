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

#ifndef CUSTOMWEBENGINEPAGE_H
#define CUSTOMWEBENGINEPAGE_H

#include <QWebEnginePage>

class QWebEngineView;

class CustomWebEnginePage : public QWebEnginePage
{
Q_OBJECT
public:
    CustomWebEnginePage(QWebEngineProfile *profile, QObject *parent = nullptr);
    ~CustomWebEnginePage();

protected:
    bool acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame) override;

protected slots:
    void slotNewWindowRequested(QWebEngineNewWindowRequest &request);
    void slotWindowClosed();

private:
    QWebEngineView* m_view;
};

#endif // CUSTOMWEBENGINEPAGE_H
