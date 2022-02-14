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

#include "combineDelegate.hpp"
#include <QSpinBox>
#include <QPushButton>
#include "widgets/combineRange.hpp"
#include "models/combinePdf.hpp"

namespace QModel
{
    CombineDelegate::CombineDelegate(QObject *parent)
        : QStyledItemDelegate(parent)
    {
    }

    QWidget *CombineDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        switch (index.column())
        {
        case 2:
        {
            Ui::CombineRange *combineRange = new Ui::CombineRange(parent);
            combineRange->setAutoFillBackground(true);
            return combineRange;
        }

        default:
        {
            return QStyledItemDelegate::createEditor(parent, option, index);
        }
        }
    }

    void CombineDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        switch (index.column())
        {
        case 2:
        {
            Ui::CombineRange *combineRange = qobject_cast<Ui::CombineRange *>(editor);
            Model::CombinePdf::Range range = index.data(Qt::EditRole).value<Model::CombinePdf::Range>();
            combineRange->setRange(range);
            break;
        }

        default:
        {
            QStyledItemDelegate::setEditorData(editor, index);
        }
        }
    }

    void CombineDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        switch (index.column())
        {
        case 2:
        {
            Ui::CombineRange *combineRange = qobject_cast<Ui::CombineRange *>(editor);
            model->setData(index, QVariant::fromValue(combineRange->getRange()), Qt::EditRole);
            break;
        }

        default:
        {
            QStyledItemDelegate::setModelData(editor, model, index);
        }
        }
    }
} // namespace QModel
