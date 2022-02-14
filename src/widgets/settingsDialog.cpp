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

#include "settingsDialog.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QComboBox>
#include <QDialogButtonBox>
#include "models/language.hpp"
#include "persistence/settings.hpp"

namespace Ui
{
    SettingsDialog::SettingsDialog(Persistence::Settings *settings, QWidget *parent)
        : QDialog(parent),
          settings(settings)
    {
        setWindowTitle(tr("Settings"));

        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setAlignment(Qt::AlignTop);

        QHBoxLayout *languageLayout = new QHBoxLayout();

        QLabel *languageLabel = new QLabel(tr("Language"), this);
        languageSelect = new QComboBox(this);
        languageSelect->addItems(Model::getAllLanguageNames());
        languageLayout->addWidget(languageLabel);
        languageLayout->addWidget(languageSelect);

        mainLayout->addLayout(languageLayout);

        QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
        mainLayout->addItem(spacer);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
        buttonBox->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
        buttonBox->addButton(tr("OK"), QDialogButtonBox::AcceptRole);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::handleAcceptButtonClicked);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
        buttonBox->setOrientation(Qt::Horizontal);
        mainLayout->addWidget(buttonBox);

        setLayout(mainLayout);

        resize(400, 300);

        readSettings();
    }

    void SettingsDialog::handleAcceptButtonClicked()
    {
        writeSettings();
        accept();
    }

    void SettingsDialog::readSettings() const
    {
        const Model::Language language = settings->getLanguage();
        const QString languageStr = Model::getLanguageName(language);
        languageSelect->setCurrentText(languageStr);
    }

    void SettingsDialog::writeSettings() const
    {
        const QString str = languageSelect->currentText();
        const Model::Language language = Model::getLanguage(str);
        settings->setLanguage(language);
    }
} // namespace Ui
