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
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <thread>
#include "widgets/pdfTable.hpp"
#include "widgets/loading.hpp"
#include "qmodels/split.hpp"
#include "qmodels/splitDelegate.hpp"
#include "utils/util.hpp"
#include "../pdf.hpp"

namespace Ui
{
    Split::Split(QWidget *parent)
        : QWidget(parent), working{false}
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();

        PdfTable<QModel::Split> *table = new PdfTable<QModel::Split>(this);
        controlModel = new QModel::Split(table);
        table->setModel(controlModel);
        QModel::SplitDelegate *delegate = new QModel::SplitDelegate(this);
        table->setItemDelegate(delegate);
        mainLayout->addWidget(table);

        QHBoxLayout *controlALayout = new QHBoxLayout();

        const QString path = Util::getDocumentPath();
        outputDirInput = new QLineEdit(path, this);
        outputDirInput->setReadOnly(true);
        controlALayout->addWidget(outputDirInput);

        QPushButton *outputDirButton = new QPushButton(tr("Output"), this);
        connect(outputDirButton, &QPushButton::clicked, this, &Split::handleOutputDirButtonClicked);
        controlALayout->addWidget(outputDirButton);

        mainLayout->addLayout(controlALayout);

        QHBoxLayout *controlBLayout = new QHBoxLayout();
        QPushButton *addFilesButton = new QPushButton(tr("Add"), this);
        connect(addFilesButton, &QPushButton::clicked, this, static_cast<void (Split::*)()>(&Split::addFiles));
        controlBLayout->addWidget(addFilesButton);
        QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Fixed);
        controlBLayout->addItem(spacer);
        startButton = new QPushButton(tr("Start"), this);
        connect(startButton, &QPushButton::clicked, this, &Split::start);
        controlBLayout->addWidget(startButton);

        mainLayout->addLayout(controlBLayout);

        setLayout(mainLayout);

        loading = new Loading(this);
        loading->hide();

        pdf = new Tool::Pdf(this);
        connect(pdf, &Tool::Pdf::splitSuccess, this, &Split::handleSplitSuccess);
    }

    void Split::addFiles()
    {
        QList<QUrl> files = QFileDialog::getOpenFileUrls(this, tr("Files"), QUrl::fromLocalFile(Util::getDocumentPath()), QStringLiteral("*.pdf"));
        files.erase(std::remove_if(files.begin(), files.end(), [](auto file) {
                        return !Util::isPdf(file);
                    }),
                    files.end());
        if (!files.isEmpty())
        {
            addFiles(files);
        }
    }

    void Split::addFiles(const QList<QUrl> &files) const
    {
        for (auto &&file : files)
        {
            const QString path = file.toLocalFile();
            try
            {
                const int pageCount = Tool::Pdf::getPageCount(path);
                controlModel->addFile(Model::SplitPdf(file.fileName(), path, pageCount));
            }
            catch (const std::exception &e)
            {
            }
        }
    }

    bool Split::isWorking() const
    {
        return working;
    }

    void Split::resizeEvent(QResizeEvent *event)
    {
        if (working)
        {
            loading->resize(size());
        }
    }

    void Split::handleOutputDirButtonClicked()
    {
        const QString directory = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Output dir"), Util::getDocumentPath()));
        if (!directory.isEmpty())
        {
            outputDirInput->setText(directory);
        }
    }

    void Split::start()
    {
        working = true;
        loading->resize(size());
        loading->start();

        std::thread t([this]() {
            std::list<Model::SplitPdf> files = controlModel->getFiles();
            files.remove_if([](auto file) {
                return !file.getEnabled();
            });
            try
            {
                pdf->split(files, outputDirInput->text());
            }
            catch (const std::exception &e)
            {
                handleSplitFail(e.what());
            }
        });
        t.detach();
    }

    void Split::handleSplitSuccess()
    {
        working = false;
        loading->stop();
    }

    void Split::handleSplitFail(const QString &message)
    {
        working = false;
        loading->stop();
        QMessageBox::critical(this, tr("Criticial"), message, QMessageBox::Yes);
    }
} // namespace Ui
