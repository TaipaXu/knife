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

#include "loading.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QMovie>

namespace Ui
{
    Loading::Loading(QWidget *parent)
        : QWidget(parent)
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setAlignment(Qt::AlignCenter);

        movie = new QMovie(this);
        movie->setFileName(":/images/loading");
        movie->setScaledSize({22, 22});

        QLabel *label = new QLabel(this);
        label->setMovie(movie);
        label->setFixedSize(80, 80);
        label->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(label);

        QPalette pal = palette();
        pal.setColor(QPalette::Window, QColor(255, 255, 255, 120));
        setAutoFillBackground(true);
        setPalette(pal);

        setLayout(mainLayout);
    }

    void Loading::start()
    {
        movie->start();
        show();
    }

    void Loading::stop()
    {
        hide();
        movie->stop();
    }
} // namespace Ui
