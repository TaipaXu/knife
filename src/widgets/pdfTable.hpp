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

#include <QVBoxLayout>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QMenu>
#include <QAction>

namespace Ui
{
    template <typename T>
    class PdfTable : public QWidget
    {
    public:
        explicit PdfTable(QWidget *parent = nullptr);
        ~PdfTable() = default;

        void setModel(T *model);
        void setItemDelegate(QAbstractItemDelegate *delegate) const;

    private slots:
        void handleTableViewMenuRequested(const QPoint &pos) const;
        void handleClearFilesActionTriggered() const;
        void handleDeleteFileActionTriggered() const;

    private:
        QTableView *tableView;
        QMenu *tableMenu;
        QAction *clearFilesAction;
        QMenu *fileMenu;
        QAction *deleteFileAction;
        T *model;
    };

    template <typename T>
    PdfTable<T>::PdfTable(QWidget *parent)
        : QWidget(parent), tableView{nullptr}
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setContentsMargins(0, 0, 0, 0);

        tableView = new QTableView(this);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setDragDropMode(QAbstractItemView::InternalMove);
        tableView->setDragEnabled(true);
        tableView->setAcceptDrops(true);
        tableView->setDropIndicatorShown(true);
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        tableView->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(tableView, &QTableView::customContextMenuRequested, this, &PdfTable::handleTableViewMenuRequested);

        mainLayout->addWidget(tableView);

        tableMenu = new QMenu(this);
        clearFilesAction = new QAction(tr("Clear"), this);
        connect(clearFilesAction, &QAction::triggered, this, &PdfTable::handleClearFilesActionTriggered);
        tableMenu->addAction(clearFilesAction);

        fileMenu = new QMenu(this);
        deleteFileAction = new QAction(tr("Delete"), this);
        connect(deleteFileAction, &QAction::triggered, this, &PdfTable::handleDeleteFileActionTriggered);
        fileMenu->addAction(deleteFileAction);

        setLayout(mainLayout);
    }

    template <typename T>
    void PdfTable<T>::setModel(T *model)
    {
        this->model = model;
        tableView->setModel(model);
    }

    template <typename T>
    void PdfTable<T>::setItemDelegate(QAbstractItemDelegate *delegate) const
    {
        tableView->setItemDelegate(delegate);
    }

    template <typename T>
    void PdfTable<T>::handleTableViewMenuRequested(const QPoint &pos) const
    {
        const QModelIndex index = tableView->indexAt(pos);
        if (index.isValid())
        {
            deleteFileAction->setData(pos);
            fileMenu->popup(tableView->viewport()->mapToGlobal(pos));
        }
        else
        {
            tableMenu->popup(tableView->viewport()->mapToGlobal(pos));
        }
    }

    template <typename T>
    void PdfTable<T>::handleClearFilesActionTriggered() const
    {
        model->clearFiles();
    }

    template <typename T>
    void PdfTable<T>::handleDeleteFileActionTriggered() const
    {
        const QAction *action = qobject_cast<QAction *>(sender());
        const QPoint point = action->data().toPoint();
        QModelIndex index = tableView->indexAt(point);
        model->removeFile(index);
    }
} // namespace Ui
