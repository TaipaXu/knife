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

#include <QString>

namespace Model
{
    class Pdf
    {
    public:
        Pdf(const QString &name, const QString &location, unsigned int pageCount);
        virtual ~Pdf() = default;

        const QString &getName() const;
        const QString &getLocation() const;
        unsigned int getPageCount() const;
        bool getEnabled() const;
        void setEnabled(bool enabled);

    private:
        QString name;
        QString location;
        unsigned int pageCount;
        bool enabled;
    };
} // namespace Model
