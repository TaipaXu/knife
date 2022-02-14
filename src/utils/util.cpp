/*
 * This file is part of the Knife (https://github.com/TaipaXu/knife).
 * Copyright (c) 2022 Taipa Xu.
 *
 * Knife is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Knife is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Knife. If not, see <http://www.gnu.org/licenses/>.
 */

#include "util.hpp"
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <initializer_list>

namespace Util
{
    QString getDocumentPath()
    {
        return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }

    QString getSafeName(const QString &name)
    {
        QString safeName(name);
        const std::initializer_list<const char *const> characters
#ifdef _WIN32
            {"<", ">", ":", "\"", "/", "\\", "|", "?", "*"};
#else
            {"/"};
#endif
        for (auto &&i : characters)
        {
            safeName.replace(i, "_");
        }

        return safeName;
    }

    bool allPdf(const QList<QUrl> &urls)
    {
        return std::all_of(urls.begin(), urls.end(), [](QUrl url) {
            return isPdf(url.path());
        });
    }

    bool isPdf(const QUrl &url)
    {
        return isPdf(url.path());
    }

    bool isPdf(const QString &path)
    {
        QFile file(QUrl::fromLocalFile(path).toLocalFile());
        if (!file.open(QIODevice::ReadOnly))
        {
            return false;
        }

        QTextStream in(&file);
        QString line = in.readLine();
        file.close();

        return line.startsWith("%PDF");
    }
} // namespace Util
