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

#pragma once

#include <QObject>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace Model
{
    enum class Language;
} // namespace Model

namespace Persistence
{
    class Settings : public QObject
    {
        Q_OBJECT

    public:
        Settings(const Settings &) = delete;
        Settings &operator=(const Settings &) = delete;
        ~Settings() = default;
        static Settings *getInstance();

    public:
        const Model::Language &getLanguage() const;
        void setLanguage(Model::Language language);

    signals:
        void settingsChanged();

    private:
        Settings();
        void readSettings();

    private:
        QSettings *settings;
        Model::Language language;
    };
} // namespace Persistence
