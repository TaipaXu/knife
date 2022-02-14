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

#include "combine.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <thread>
#include "widgets/pdfTable.hpp"
#include "widgets/loading.hpp"
#include "qmodels/combine.hpp"
#include "qmodels/combineDelegate.hpp"
#include "utils/util.hpp"
#include "../pdf.hpp"
#include <QtDebug>

namespace Ui
{
    Combine::Combine(QWidget *parent)
        : QWidget(parent), working{false}
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();

        PdfTable<QModel::Combine> *table = new PdfTable<QModel::Combine>(this);
        controlModel = new QModel::Combine(table);
        table->setModel(controlModel);
        QModel::CombineDelegate *delegate = new QModel::CombineDelegate(this);
        table->setItemDelegate(delegate);
        mainLayout->addWidget(table);

        QHBoxLayout *controlALayout = new QHBoxLayout();

        QLabel *outputNameLabel = new QLabel(tr("File name"), this);
        controlALayout->addWidget(outputNameLabel);

        outputNameInput = new QLineEdit(tr("Combine"), this);
        connect(outputNameInput, &QLineEdit::textChanged, this, &Combine::handleOutputNameChanged);
        controlALayout->addWidget(outputNameInput);

        QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
        controlALayout->addItem(spacer);

        const QString path = Util::getDocumentPath();
        outputDirInput = new QLineEdit(path, this);
        outputDirInput->setReadOnly(true);
        controlALayout->addWidget(outputDirInput);

        QPushButton *outputDirButton = new QPushButton(tr("Output"), this);
        connect(outputDirButton, &QPushButton::clicked, this, &Combine::handleOutputDirButtonClicked);
        controlALayout->addWidget(outputDirButton);

        mainLayout->addLayout(controlALayout);

        QHBoxLayout *controlBLayout = new QHBoxLayout();
        QPushButton *addFilesButton = new QPushButton(tr("Add"), this);
        connect(addFilesButton, &QPushButton::clicked, this, static_cast<void (Combine::*)()>(&Combine::addFiles));
        controlBLayout->addWidget(addFilesButton);
        spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Fixed);
        controlBLayout->addItem(spacer);
        startButton = new QPushButton(tr("Start"), this);
        connect(startButton, &QPushButton::clicked, this, &Combine::start);
        controlBLayout->addWidget(startButton);

        mainLayout->addLayout(controlBLayout);

        setLayout(mainLayout);

        loading = new Loading(this);
        loading->hide();

        pdf = new Tool::Pdf(this);
        connect(pdf, &Tool::Pdf::combineSuccess, this, &Combine::handleCombineSuccess);
    }

    void Combine::addFiles()
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

    void Combine::addFiles(const QList<QUrl> &files) const
    {
        for (auto &&file : files)
        {
            const QString path = file.toLocalFile();
            try
            {
                const int pageCount = Tool::Pdf::getPageCount(path);
                controlModel->addFile(Model::CombinePdf(file.fileName(), path, pageCount));
            }
            catch (const std::exception &e)
            {
            }
        }
    }

    bool Combine::isWorking() const
    {
        return working;
    }

    void Combine::resizeEvent(QResizeEvent *event)
    {
        if (working)
        {
            loading->resize(size());
        }
    }

    void Combine::handleOutputNameChanged(const QString &text) const
    {
        startButton->setEnabled(!text.isEmpty());
    }

    void Combine::handleOutputDirButtonClicked()
    {
        const QString directory = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Output dir"), Util::getDocumentPath()));
        if (!directory.isEmpty())
        {
            outputDirInput->setText(directory);
        }
    }

    void Combine::start()
    {
        working = true;
        loading->resize(size());
        loading->start();

        std::thread t([this]() {
            std::list<Model::CombinePdf> files = controlModel->getFiles();
            files.remove_if([](auto file) {
                return !file.getEnabled();
            });
            const QString safeName = Util::getSafeName(outputNameInput->text());
            try
            {
                pdf->combine(files, QStringLiteral("%1/%2.pdf").arg(outputDirInput->text()).arg(safeName));
            }
            catch (const std::exception &e)
            {
                handleCombineFail(e.what());
            }
        });
        t.detach();
    }

    void Combine::handleCombineSuccess()
    {
        working = false;
        loading->stop();
    }

    void Combine::handleCombineFail(const QString &message)
    {
        working = false;
        loading->stop();
        QMessageBox::critical(this, tr("Criticial"), message, QMessageBox::Yes);
    }
} // namespace Ui
