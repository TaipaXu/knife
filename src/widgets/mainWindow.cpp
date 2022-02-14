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

#include "mainWindow.hpp"
#include <QApplication>
#include <QDesktopServices>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QTabWidget>
#include <QMessageBox>
#include <QDragEnterEvent>
#include <QShortcut>
#include <QMimeData>
#include "config.hpp"
#include "widgets/combine.hpp"
#include "widgets/split.hpp"
#include "widgets/settingsDialog.hpp"
#include "widgets/drop.hpp"
#include "persistence/settings.hpp"
#include "utils/util.hpp"

namespace Ui
{
    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
    {
        setWindowIcon(QIcon(":/images/knife"));

        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

        tabWidget = new QTabWidget(this);
        combine = new Combine(this);
        split = new Split(this);
        tabWidget->addTab(combine, tr("COMBINE"));
        tabWidget->addTab(split, tr("SPLIT"));
        mainLayout->addWidget(tabWidget);

        setCentralWidget(centralWidget);

        drop = new Drop(this);
        drop->hide();
        setAcceptDrops(true);

        createActions();

        settings = Persistence::Settings::getInstance();

        resize(800, 560);
    }

    void MainWindow::showInCenter()
    {
        QRect desktopRect = qApp->primaryScreen()->availableGeometry();
        QPoint center = desktopRect.center();
        move(center - frameGeometry().center());
        show();
    }

    void MainWindow::dragEnterEvent(QDragEnterEvent *event)
    {
        event->accept();
        drop->resize(size());

        if (event->mimeData()->hasUrls() && Util::allPdf(event->mimeData()->urls()))
        {
            drop->setAcceptable(true);
        }
        else
        {
            drop->setAcceptable(false);
        }
        drop->show();
    }

    void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
    {
        event->accept();
        drop->hide();
    }

    void MainWindow::dropEvent(QDropEvent *event)
    {
        event->accept();
        drop->hide();

        const QList<QUrl> urls = event->mimeData()->urls();
        if (!Util::allPdf(urls))
        {
            return;
        }

        const QWidget *widget = tabWidget->currentWidget();
        if (widget == combine)
        {
            combine->addFiles(urls);
        }
        else if (widget == split)
        {
            split->addFiles(urls);
        }
    }

    void MainWindow::closeEvent(QCloseEvent *event)
    {
        if (combine->isWorking() || split->isWorking())
        {
            QMessageBox::StandardButton result = QMessageBox::warning(this, tr("Warning"), tr("Do you want ot close this application?\nYour work will be terminated!"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (result != QMessageBox::Yes)
            {
                event->ignore();
                return;
            }
        }
        event->accept();
    }

    void MainWindow::handleAddActionTriggered() const
    {
        const QWidget *widget = tabWidget->currentWidget();
        if (widget == combine)
        {
            combine->addFiles();
        }
        else if (widget == split)
        {
            split->addFiles();
        }
    }

    void MainWindow::handleSettingsActionTriggered() const
    {
        SettingsDialog *settingsDialog = new SettingsDialog(settings);
        settingsDialog->exec();
        settingsDialog->deleteLater();
        settingsDialog = nullptr;
    }

    void MainWindow::handleAboutActionTriggered() const
    {
        QDesktopServices::openUrl(QUrl(PROJECT_DOMAIN));
    }

    void MainWindow::handleReportBugActionTriggered() const
    {
        QDesktopServices::openUrl(QUrl(PROJECT_ISSUES_DOMAIN));
    }

    void MainWindow::createActions()
    {
        QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

        QAction *addAction = new QAction(QIcon::fromTheme("list-add"), tr("&Add"), this);
        connect(addAction, &QAction::triggered, this, &MainWindow::handleAddActionTriggered);
        addAction->setStatusTip(tr("Add files"));
        fileMenu->addAction(addAction);

        QAction *settingsAction = new QAction(QIcon::fromTheme("emblem-system"), tr("&Settings"), fileMenu);
        connect(settingsAction, &QAction::triggered, this, &MainWindow::handleSettingsActionTriggered);
        settingsAction->setStatusTip(tr("Settings"));
        fileMenu->addAction(settingsAction);
        fileMenu->addSeparator();

        QAction *exitAct = fileMenu->addAction(QIcon::fromTheme("application-exit"), tr("E&xit"), this, &QCoreApplication::quit);
        exitAct->setStatusTip(tr("Exit the application"));
        exitAct->setShortcuts(QKeySequence::Quit);

        QMenu *aboutMenu = menuBar()->addMenu(tr("&About"));

        QAction *aboutAction = new QAction(tr("About"), aboutMenu);
        connect(aboutAction, &QAction::triggered, this, &MainWindow::handleAboutActionTriggered);
        aboutAction->setStatusTip(tr("Open the page of this project"));
        aboutMenu->addAction(aboutAction);

        QAction *aboutQtAction = new QAction(tr("About Qt"), aboutMenu);
        connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
        aboutMenu->addAction(aboutQtAction);
        aboutMenu->addSeparator();

        QAction *reportBugAction = new QAction(tr("Report Bug"), aboutMenu);
        connect(reportBugAction, &QAction::triggered, this, &MainWindow::handleReportBugActionTriggered);
        reportBugAction->setStatusTip(tr("Open the page to report bugs"));
        aboutMenu->addAction(reportBugAction);

        for (auto &&key : {Qt::CTRL, Qt::ALT})
        {
            QShortcut *showDictionaryShortcut = new QShortcut(this);
            connect(showDictionaryShortcut, &QShortcut::activated, this, &MainWindow::showCombine);
            showDictionaryShortcut->setKey(key + Qt::Key_1);
            showDictionaryShortcut->setAutoRepeat(false);
        }

        for (auto &&key : {Qt::CTRL, Qt::ALT})
        {
            QShortcut *showTranslateShortcutCtrl = new QShortcut(this);
            connect(showTranslateShortcutCtrl, &QShortcut::activated, this, &MainWindow::showSplit);
            showTranslateShortcutCtrl->setKey(key + Qt::Key_2);
            showTranslateShortcutCtrl->setAutoRepeat(false);
        }
    }

    void MainWindow::showCombine() const
    {
        tabWidget->setCurrentWidget(combine);
    }

    void MainWindow::showSplit() const
    {
        tabWidget->setCurrentWidget(split);
    }
} // namespace Ui
