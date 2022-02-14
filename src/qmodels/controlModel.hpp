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

#include <QAbstractTableModel>
#include <QString>
#include <QMimeData>
#include <list>

namespace QModel
{
    template <typename T>
    class ControlModel : public QAbstractTableModel
    {
    public:
        explicit ControlModel(QObject *parent = nullptr);
        virtual ~ControlModel() = default;

        const std::list<T> getFiles() const;
        void addFile(const T &file);
        void removeFile(int index);
        void removeFile(const QModelIndex &index);
        void clearFiles();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        QStringList mimeTypes() const override;
        QMimeData *mimeData(const QModelIndexList &indexes) const override;
        Qt::DropActions supportedDragActions() const override;
        Qt::DropActions supportedDropActions() const override;
        bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
        bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    protected:
        std::list<T> files;

    private:
        static const QString mimeType;
    };

    template <typename T>
    const QString ControlModel<T>::mimeType("application/knife-table-row");

    template <typename T>
    ControlModel<T>::ControlModel(QObject *parent)
        : QAbstractTableModel(parent)
    {
    }

    template <typename T>
    const std::list<T> ControlModel<T>::getFiles() const
    {
        return files;
    }

    template <typename T>
    void ControlModel<T>::addFile(const T &file)
    {
        const int size = files.size();
        beginInsertRows(QModelIndex(), size, size);
        files.push_back(file);
        endInsertRows();
    }

    template <typename T>
    void ControlModel<T>::removeFile(int index)
    {
        auto it = files.begin();
        std::advance(it, index);
        beginRemoveRows(QModelIndex(), index, index);
        files.erase(it);
        endRemoveRows();
    }

    template <typename T>
    void ControlModel<T>::removeFile(const QModelIndex &index)
    {
        const int row = index.row();
        removeFile(row);
    }

    template <typename T>
    void ControlModel<T>::clearFiles()
    {
        beginResetModel();
        files.clear();
        endResetModel();
    }

    template <typename T>
    int ControlModel<T>::rowCount(const QModelIndex &parent) const
    {
        return files.size();
    }

    template <typename T>
    int ControlModel<T>::columnCount(const QModelIndex &parent) const
    {
        return 3;
    }

    template <typename T>
    Qt::ItemFlags ControlModel<T>::flags(const QModelIndex &index) const
    {
        if (!index.isValid())
        {
            return Qt::NoItemFlags;
        }

        const int column = index.column();
        switch (column)
        {
        case 0:
        {
            return Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractTableModel::flags(index);
        }

        case 2:
        {
            return Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractTableModel::flags(index);
        }

        default:
        {
            return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractTableModel::flags(index);
        }
        }
    }

    template <typename T>
    QStringList ControlModel<T>::mimeTypes() const
    {
        return {mimeType};
    }

    template <typename T>
    QMimeData *ControlModel<T>::mimeData(const QModelIndexList &indexes) const
    {
        if (indexes.size() < 1)
        {
            return nullptr;
        }

        QMimeData *mimeData = new QMimeData();
        const QModelIndex index = indexes.at(0);
        mimeData->setData(mimeType, QByteArray::number(index.row()));
        return mimeData;
    }

    template <typename T>
    Qt::DropActions ControlModel<T>::supportedDragActions() const
    {
        return Qt::MoveAction;
    }

    template <typename T>
    Qt::DropActions ControlModel<T>::supportedDropActions() const
    {
        return Qt::MoveAction;
    }

    template <typename T>
    bool ControlModel<T>::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
    {
        return data && action == Qt::MoveAction && data->hasFormat(mimeType);
    }

    template <typename T>
    bool ControlModel<T>::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
    {
        if (action == Qt::IgnoreAction)
        {
            return true;
        }

        if (!data || action != Qt::MoveAction || !data->hasFormat(mimeType))
        {
            return false;
        }

        const int fromRow = data->data(mimeType).toInt();
        const int toRow = parent.row();

        beginMoveRows(QModelIndex(), fromRow, fromRow, QModelIndex(), fromRow < toRow ? toRow + 1 : toRow);
        auto itFrom = files.begin();
        std::advance(itFrom, fromRow);
        auto itTo = files.begin();
        std::advance(itTo, toRow);
        std::swap(*itFrom, *itTo);
        endMoveRows();

        return true;
    }
} // namespace QModel
