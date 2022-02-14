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

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLineEdit;
QT_END_NAMESPACE

namespace QModel
{
    class Split;
} // namespace QModel

namespace Tool
{
    class Pdf;
} // namespace Tool

namespace Ui
{
    class Loading;

    class Split : public QWidget
    {
        Q_OBJECT

    public:
        explicit Split(QWidget *parent = nullptr);
        ~Split() = default;

        void addFiles();
        void addFiles(const QList<QUrl> &files) const;
        bool isWorking() const;

    protected:
        void resizeEvent(QResizeEvent *event) override;

    private slots:
        void handleOutputDirButtonClicked();
        void start();
        void handleSplitSuccess();

    private:
        void handleSplitFail(const QString &message);

    private:
        QLineEdit *outputDirInput;
        QPushButton *startButton;
        Loading *loading;
        QModel::Split *controlModel;
        Tool::Pdf *pdf;
        bool working;
    };
} // namespace Ui
