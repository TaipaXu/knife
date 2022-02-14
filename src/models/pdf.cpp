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

#include "models/pdf.hpp"

namespace Model
{
    Pdf::Pdf(const QString &name, const QString &location, unsigned int pageCount)
        : name{name},
          location{location},
          pageCount{pageCount},
          enabled{true}
    {
    }

    const QString &Pdf::getName() const
    {
        return name;
    }

    const QString &Pdf::getLocation() const
    {
        return location;
    }

    unsigned int Pdf::getPageCount() const
    {
        return pageCount;
    }

    bool Pdf::getEnabled() const
    {
        return enabled;
    }

    void Pdf::setEnabled(bool enabled)
    {
        this->enabled = enabled;
    }
} // namespace Model
