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

#include "splitDelegate.hpp"
#include <QLineEdit>
#include <QHBoxLayout>
#include "widgets/combineRange.hpp"
#include "widgets/splitMode.hpp"
#include "models/splitPdf.hpp"

namespace QModel
{
    SplitDelegate::SplitDelegate(QObject *parent)
        : QStyledItemDelegate(parent)
    {
    }

    QWidget *SplitDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        switch (index.column())
        {
        case 2:
        {
            Ui::SplitMode *splitMode = new Ui::SplitMode(parent);
            return splitMode;
        }

        default:
        {
            return QStyledItemDelegate::createEditor(parent, option, index);
        }
        }
    }

    void SplitDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        switch (index.column())
        {
        case 2:
        {
            Model::SplitPdf::Mode mode = index.data(Qt::EditRole).value<Model::SplitPdf::Mode>();
            Ui::SplitMode *splitMode = qobject_cast<Ui::SplitMode *>(editor);
            splitMode->setMode(mode);
            break;
        }

        default:
        {
            QStyledItemDelegate::setEditorData(editor, index);
        }
        }
    }

    void SplitDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        switch (index.column())
        {
        case 2:
        {
            Ui::SplitMode *splitMode = qobject_cast<Ui::SplitMode *>(editor);
            Model::SplitPdf::Mode mode = splitMode->getMode();

            model->setData(index, QVariant::fromValue(mode), Qt::EditRole);
            break;
        }

        default:
        {
            QStyledItemDelegate::setModelData(editor, model, index);
        }
        }
    }
} // namespace QModel
