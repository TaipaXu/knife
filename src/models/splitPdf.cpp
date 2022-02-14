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

#include "splitPdf.hpp"

namespace Model
{
    SplitPdf::SplitPdf(const QString &name, const QString &location, unsigned int pageCount)
        : Pdf(name, location, pageCount),
          mode{0, pageCount - 1, 0, 1}
    {
    }

    const SplitPdf::Mode &SplitPdf::getMode() const
    {
        return mode;
    }

    void SplitPdf::setMode(const SplitPdf::Mode &mode)
    {
        this->mode = mode;
    }
} // namespace Model
