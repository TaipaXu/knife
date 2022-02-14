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

#include "pdf.hpp"
#include <QDir>
extern "C"
{
#include <mupdf/fitz.h>
#include <mupdf/pdf.h>
}
#include "errors/pdfError.hpp"

namespace Tool
{
    Pdf::Pdf(QObject *parent)
        : QObject(parent)
    {
    }

    int Pdf::getPageCount(const QString &file)
    {
        fz_context *ctx = fz_new_context(nullptr, nullptr, FZ_STORE_UNLIMITED);
        if (!ctx)
        {
            throw PdfError(tr("PDF error"));
        }

        pdf_document *doc = nullptr;
        fz_try(ctx)
        {
            doc = pdf_open_document(ctx, file.toStdString().c_str());
        }
        fz_catch(ctx)
        {
            fz_flush_warnings(ctx);
            pdf_drop_document(ctx, doc);
            fz_drop_context(ctx);
            throw PdfError(tr("PDF context error"));
        }
        const int count = pdf_count_pages(ctx, doc);
        pdf_drop_document(ctx, doc);
        fz_drop_context(ctx);
        return count;
    }

    void Pdf::combine(const std::list<Model::CombinePdf> &files, const QString &outputFile) const
    {
        fz_context *ctx = fz_new_context(nullptr, nullptr, FZ_STORE_UNLIMITED);
        if (!ctx)
        {
            throw PdfError(tr("PDF error"));
        }

        pdf_document *docDes = pdf_create_document(ctx);
        pdf_write_options opts = pdf_default_write_options;
        const char *const flags = "";
        pdf_parse_write_options(ctx, &opts, flags);

        for (auto &&file : files)
        {
            pdf_graft_map *graft_map = pdf_new_graft_map(ctx, docDes);
            Model::CombinePdf::Range range = file.getRange();
            pdf_document *docSrc = nullptr;
            fz_try(ctx)
            {
                docSrc = pdf_open_document(ctx, file.getLocation().toStdString().c_str());
            }
            fz_catch(ctx)
            {
                fz_flush_warnings(ctx);
                pdf_drop_document(ctx, docSrc);
                pdf_drop_document(ctx, docDes);
                pdf_drop_graft_map(ctx, graft_map);
                fz_drop_context(ctx);
                throw PdfError(tr("Can not open the file"));
            }
            for (size_t i = range.startPage; i <= range.endPage; i++)
            {
                pdf_graft_mapped_page(ctx, graft_map, -1, docSrc, i);
            }
            pdf_drop_document(ctx, docSrc);
            pdf_drop_graft_map(ctx, graft_map);
        }
        fz_try(ctx)
        {
            pdf_save_document(ctx, docDes, outputFile.toStdString().c_str(), &opts);
            emit combineSuccess();
        }
        fz_catch(ctx)
        {
            fz_flush_warnings(ctx);
            pdf_drop_document(ctx, docDes);
            fz_drop_context(ctx);
            throw PdfError(tr("Can not save the combined file"));
        }

        pdf_drop_document(ctx, docDes);
        fz_drop_context(ctx);
    }

    void Pdf::split(const std::list<Model::SplitPdf> &files, const QString &outputDir) const
    {
        fz_context *ctx = fz_new_context(nullptr, nullptr, FZ_STORE_UNLIMITED);
        if (!ctx)
        {
            throw PdfError(tr("PDF error"));
        }

        pdf_write_options opts = pdf_default_write_options;
        const char *const flags = "";
        pdf_parse_write_options(ctx, &opts, flags);

        unsigned int index = 0;
        for (auto &&file : files)
        {
            const QString dirName = QStringLiteral("%1#%2").arg(++index).arg(file.getName().split(".")[0]);
            const QString dirPath = QStringLiteral("%1/%2").arg(outputDir).arg(dirName);
            if (!QDir().exists(dirPath))
            {
                QDir().mkdir(dirPath);
            }

            const Model::SplitPdf::Mode mode = file.getMode();
            int start = mode.startPage;
            while (start <= mode.endPage)
            {
                const int end = (start + mode.length - 1) < mode.endPage ? start + mode.length - 1 : mode.endPage;

                pdf_document *docDes = pdf_create_document(ctx);
                pdf_graft_map *graft_map = pdf_new_graft_map(ctx, docDes);
                pdf_document *docSrc = nullptr;
                fz_try(ctx)
                {
                    docSrc = pdf_open_document(ctx, file.getLocation().toStdString().c_str());
                }
                fz_catch(ctx)
                {
                    fz_flush_warnings(ctx);
                    pdf_drop_document(ctx, docSrc);
                    pdf_drop_document(ctx, docDes);
                    pdf_drop_graft_map(ctx, graft_map);
                    fz_drop_context(ctx);
                    throw PdfError(tr("Can not open the file"));
                }
                for (size_t i = start; i <= end; i += 1)
                {
                    pdf_graft_mapped_page(ctx, graft_map, -1, docSrc, i);
                }
                fz_try(ctx)
                {
                    pdf_save_document(ctx, docDes, QStringLiteral("%1/%2/%3-%4.pdf").arg(outputDir).arg(dirName).arg(start + 1).arg(end + 1).toStdString().c_str(), &opts);
                    start = end + mode.spacing + 1;
                }
                fz_catch(ctx)
                {
                    pdf_drop_document(ctx, docSrc);
                    pdf_drop_document(ctx, docDes);
                    pdf_drop_graft_map(ctx, graft_map);
                    fz_drop_context(ctx);
                    throw PdfError(tr("Can not save the splitted file"));
                }

                pdf_drop_document(ctx, docSrc);
                pdf_drop_document(ctx, docDes);
                pdf_drop_graft_map(ctx, graft_map);
            }
        }
        emit splitSuccess();

        fz_drop_context(ctx);
    }
} // namespace Tool
