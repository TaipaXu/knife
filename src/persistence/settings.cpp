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

#include "settings.hpp"
#include <QSettings>
#include "models/language.hpp"

namespace Persistence
{
    Settings::Settings()
    {
        settings = new QSettings(this);
        readSettings();
    }

    Settings *Settings::getInstance()
    {
        static Settings instance;
        return &instance;
    }

    const Model::Language &Settings::getLanguage() const
    {
        return language;
    }

    void Settings::setLanguage(Model::Language language)
    {
        this->language = language;
        settings->setValue("language", static_cast<int>(language));
        emit settingsChanged();
    }

    void Settings::readSettings()
    {
        language = static_cast<Model::Language>(settings->value("language", 0).toInt());
    }
} // namespace Persistence
