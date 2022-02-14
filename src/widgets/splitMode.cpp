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

#include "splitMode.hpp"
#include <QLineEdit>
#include <QHBoxLayout>

namespace Ui
{
    SplitMode::SplitMode(QWidget *parent)
        : QWidget(parent)
    {
        startPageInput = new QLineEdit(this);
        startPageInput->setPlaceholderText(tr("Start page"));
        endPageInput = new QLineEdit(this);
        endPageInput->setPlaceholderText(tr("End page"));
        spacingInput = new QLineEdit(this);
        spacingInput->setPlaceholderText(tr("Spacing"));
        lengthInput = new QLineEdit(this);
        lengthInput->setPlaceholderText(tr("Length"));

        QHBoxLayout *layout = new QHBoxLayout();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(startPageInput);
        layout->addWidget(endPageInput);
        layout->addWidget(spacingInput);
        layout->addWidget(lengthInput);
        setLayout(layout);

        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setAutoFillBackground(true);
    }

    const Model::SplitPdf::Mode SplitMode::getMode() const
    {
        const unsigned int start = startPageInput->text().toInt() - 1;
        const unsigned int end = endPageInput->text().toInt() - 1;
        const unsigned int spacing = spacingInput->text().toInt();
        const unsigned int length = lengthInput->text().toInt();
        const Model::SplitPdf::Mode mode{start, end, spacing, length};
        return mode;
    }

    void SplitMode::setMode(const Model::SplitPdf::Mode &mode)
    {
        startPageInput->setText(QString::number(mode.startPage + 1));
        endPageInput->setText(QString::number(mode.endPage + 1));
        spacingInput->setText(QString::number(mode.spacing));
        lengthInput->setText(QString::number(mode.length));
    }
} // namespace Ui
