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

#include <QApplication>
#include <QLocale>
#include <QFileInfo>
#include <QTranslator>
#include <QMessageBox>

namespace
{
    const QString qtBaseName = QStringLiteral("qtbase");
    QTranslator* getTranslator( QString locale, QString path, QString baseName)
    {
        QTranslator* translator = new QTranslator();
        if ( ! translator->load(QLocale(locale), baseName, QStringLiteral("_"), path,QStringLiteral(".qm")))
        {
            delete translator;
            translator =  nullptr;
            QMessageBox::warning(nullptr,QApplication::applicationName(),"Could not find translation file " + baseName + " for locale " + locale);
        }
        return translator;
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("partcp-client");
    QApplication::setApplicationVersion("0.1");
    QApplication::setOrganizationName("Die Basis");
    QApplication::setOrganizationDomain("diebasis-partei.de");

    QFileInfo fi(QCoreApplication::applicationFilePath());
    QStringList locales = {"nl_NL", "de_DE", "en_GB", "fr_FR","sv_SV"};
    QMap<QString,QList<QTranslator*>> translators;
    foreach( const auto& locale, locales )
    {
        QTranslator* translator = getTranslator(locale,fi.absolutePath(), fi.baseName());
        if ( translator )
        {
            translators[locale].append(translator);
        }
        translator = getTranslator(locale,fi.absolutePath(), qtBaseName);
        if ( translator )
        {
            translators[locale].append(translator);
        }
    }

    MainWindow window(translators);
    window.show();
    return app.exec();
}
