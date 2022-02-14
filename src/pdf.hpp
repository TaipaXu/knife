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
#include <QString>
#include <vector>
#include "models/combinePdf.hpp"
#include "models/splitPdf.hpp"

namespace Tool
{
    class Pdf : public QObject
    {
        Q_OBJECT

    public:
        explicit Pdf(QObject *parent = nullptr);
        ~Pdf() = default;

        static int getPageCount(const QString &file);
        void combine(const std::list<Model::CombinePdf> &files, const QString &outputFile) const;
        void split(const std::list<Model::SplitPdf> &files, const QString &outputDir) const;

    signals:
        void combineSuccess() const;
        void splitSuccess() const;
    };
} // namespace Tool
