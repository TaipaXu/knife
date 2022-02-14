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

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QTabWidget;
QT_END_NAMESPACE

namespace Persistence
{
    class Settings;
} // namespace Persistence

namespace Ui
{
    class Combine;
    class Split;
    class SettingsDialog;
    class Drop;

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() = default;

        void showInCenter();

    protected:
        void dragEnterEvent(QDragEnterEvent *event) override;
        void dragLeaveEvent(QDragLeaveEvent *event) override;
        void dropEvent(QDropEvent *event) override;
        void closeEvent(QCloseEvent *event) override;

    private slots:
        void handleAddActionTriggered() const;
        void handleSettingsActionTriggered() const;
        void handleAboutActionTriggered() const;
        void handleReportBugActionTriggered() const;

    private:
        void createActions();
        void showCombine() const;
        void showSplit() const;

    private:
        QTabWidget *tabWidget;
        Combine *combine;
        Split *split;
        SettingsDialog *settingsDialog;
        Drop *drop;
        Persistence::Settings *settings;
    };
} // namespace Ui
