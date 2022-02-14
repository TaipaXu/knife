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

#include "combineRange.hpp"
#include <QHBoxLayout>
#include <QLineEdit>

namespace Ui
{
    CombineRange::CombineRange(QWidget *parent)
        : QWidget(parent)
    {
        QHBoxLayout *mainLayout = new QHBoxLayout();
        mainLayout->setContentsMargins(0, 0, 0, 0);

        startPageInput = new QLineEdit(this);
        startPageInput->setPlaceholderText(tr("Start page"));
        mainLayout->addWidget(startPageInput);

        endPageInput = new QLineEdit(this);
        endPageInput->setPlaceholderText(tr("End page"));
        mainLayout->addWidget(endPageInput);

        setLayout(mainLayout);
    }

    Model::CombinePdf::Range CombineRange::getRange() const
    {
        const unsigned int start = startPageInput->text().toInt() - 1;
        const unsigned int end = endPageInput->text().toInt() - 1;
        const Model::CombinePdf::Range range{start, end};
        return range;
    }

    void CombineRange::setRange(const Model::CombinePdf::Range &range)
    {
        startPageInput->setText(QString::number(range.startPage + 1));
        endPageInput->setText(QString::number(range.endPage + 1));
    }
} // namespace Ui
