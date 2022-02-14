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

#include "split.hpp"

namespace QModel
{
    Split::Split(QObject *parent)
        : ControlModel<Model::SplitPdf>(parent)
    {
    }

    QVariant Split::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
            {
                return tr("Name");
            }

            case 1:
            {
                return tr("Page Count");
            }

            case 2:
            {
                return tr("Range");
            }

            default:
            {
                break;
            }
            }
        }
        return QVariant();
    }

    QVariant Split::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
        {
            return QVariant();
        }

        if (role == Qt::TextAlignmentRole)
        {
            return Qt::AlignCenter;
        }

        auto it = files.begin();
        std::advance(it, index.row());

        const int column = index.column();

        if (role == Qt::CheckStateRole && index.column() == 0)
        {
            return it->getEnabled() ? Qt::Checked : Qt::Unchecked;
        }

        if (role == Qt::DisplayRole)
        {
            switch (column)
            {
            case 0:
            {
                return it->getName();
            }

            case 1:
            {
                return it->getPageCount();
            }

            case 2:
            {
                const Model::SplitPdf::Mode mode = it->getMode();
                return QStringLiteral("%1-%2(%3:%4)").arg(mode.startPage + 1).arg(mode.endPage + 1).arg(mode.spacing).arg(mode.length);
            }

            default:
            {
                break;
            }
            }
        }

        if (role == Qt::EditRole && column == 2)
        {
            return QVariant::fromValue(it->getMode());
        }

        return QVariant();
    }

    bool Split::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (!index.isValid())
        {
            return false;
        }

        auto it = files.begin();
        std::advance(it, index.row());

        const int column = index.column();

        if (role == Qt::CheckStateRole && column == 0)
        {
            it->setEnabled(!it->getEnabled());
            QModelIndex topLeft = createIndex(index.row(), 0);
            emit dataChanged(topLeft, topLeft, {Qt::DisplayRole});
            return true;
        }

        if (role == Qt::EditRole && column == 2)
        {
            Model::SplitPdf::Mode mode = value.value<Model::SplitPdf::Mode>();
            if (mode.startPage > mode.endPage)
            {
                std::swap(mode.startPage, mode.endPage);
            }
            const unsigned int pageCount = it->getPageCount();
            const unsigned int pageCountIndex = it->getPageCount() - 1;
            mode.startPage = std::min(mode.startPage, pageCountIndex);
            mode.endPage = std::min(mode.endPage, pageCountIndex);
            mode.spacing = std::min(mode.spacing, pageCount);
            mode.length = std::min(mode.length, pageCount);

            it->setMode(mode);
            QModelIndex topLeft = createIndex(index.row(), 0);
            emit dataChanged(topLeft, topLeft, {Qt::DisplayRole});
            return true;
        }

        return false;
    }
} // namespace QModel
