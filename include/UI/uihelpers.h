/**
 * @file uihelpers.h
 * @author Jorge Agüero Zamora
 * @brief Contains helper functions for the Window classes
 * @version 0.1
 * @date 2021-06-14
 * 
 * 
 */
#ifndef UIHELPERS_H
#define UIHELPERS_H

#define ELEMENT_NUMBER_MILLION_CUTOFF 1000000 //!< one million constant
#define ELEMENT_NUMBER_THOUSAND_CUTOFF 1000   //!< on thousand constant

#define LONGER_MESSAGE_TIME 3000   //!< time for status messages to appear in window
#define STANDARD_MESSAGE_TIME 2000 //!< time for status messages to appear in window

#include <QtWidgets>
#include <QString>

namespace uihelp
{
    /**
     * @brief enum with file dialog type
     * 
     */
    typedef enum
    {
        FD_IMAGE, //!<  an image file dialog
        FD_ZIP    //!< file dialog for zip files

    } file_dialog_type_t;

    /**
     * @brief constructs string with all supported MIME types
     * 
     * @param mimeTypeFilters a list of strings of the MIME types
     * @return QString a string with all valid extensions of the MIME type
     */
    inline static QString allSupportedFormatsString(QStringList mimeTypeFilters)
    {
        QMimeDatabase mimeDB;
        QStringList allSupportedFormats;
        for (const QString &mimeTypeFilter : mimeTypeFilters)
        {
            QMimeType mimeType = mimeDB.mimeTypeForName(mimeTypeFilter);
            if (mimeType.isValid())
            {
                allSupportedFormats.append(mimeType.globPatterns());
            }
        }
        return QString("All supported formats (%1)").arg(allSupportedFormats.join(' '));
    }

    /**
     * @brief creates a file dialog window
     * 
     * @param dialog pointer to the file dialog window
     * @param acceptMode whether it is to save or load file
     * @param file_dialog_type type of dialog window, either FD_ZIP or FD_IMAGE
     * @param filemode whether it is looking for a file or a directory
     */
    inline static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode, file_dialog_type_t file_dialog_type = file_dialog_type_t::FD_IMAGE, QFileDialog::FileMode filemode = QFileDialog::FileMode::DirectoryOnly)
    {
        static bool firstDialog = true;
        dialog.setFileMode(filemode);
        if (firstDialog)
        {
            firstDialog = false;
            const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
            dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
        }

        if (filemode == QFileDialog::FileMode::DirectoryOnly)
            return;

        QStringList mimeTypeFilters;

        if (file_dialog_type == file_dialog_type_t::FD_IMAGE)
        {
            if (acceptMode == QFileDialog::AcceptOpen)
                mimeTypeFilters.append("image/jpeg");
            mimeTypeFilters.append("image/png");
            // mimeTypeFilters.append("image/png | image/jpeg");
            if (acceptMode == QFileDialog::AcceptSave)
                dialog.setDefaultSuffix("png");
            QString allSupportedFormats = allSupportedFormatsString(mimeTypeFilters);
            dialog.setMimeTypeFilters(mimeTypeFilters);
            QStringList nameFilters = dialog.nameFilters();
            nameFilters.append(allSupportedFormats);
            dialog.setNameFilters(nameFilters);
            dialog.selectNameFilter(allSupportedFormats);
        }

        if (file_dialog_type == file_dialog_type_t::FD_ZIP)
        {
            mimeTypeFilters.append("application/zip");
            if (acceptMode == QFileDialog::AcceptSave)
                dialog.setDefaultSuffix("zip");

            dialog.setMimeTypeFilters(mimeTypeFilters);
        }
    }

    /**
     * @brief hides all items in a QListWidget
     * 
     * @param list lis widget to hide items for
     */
    inline static void hideListItems(QListWidget *list)
    {
        for (int i = 0; i < list->count(); ++i)
        {
            list->item(i)->setHidden(true);
        }
    }

    /**
     * @brief filters the list contents that contain the filter string
     * 
     * @param list list of items to filter
     * @param filter_string contains the string to filter for
     */
    inline static void filterListItems(QListWidget *list, QString filter_string)
    {
        hideListItems(list);
        QList<QListWidgetItem *> matches(list->findItems(filter_string, Qt::MatchFlag::MatchContains));
        for (QListWidgetItem *item : matches)
            item->setHidden(false);
    }

    /**
     * @brief generates string with size information
     * 
     * @param rows amount of rows
     * @param cols amount of columns
     * @return QString formatted size message string
     */
    inline static QString generateSizeMessage(int rows, int cols)
    {
        long int num_elements = rows * cols;
        QString elem_string;
        if (num_elements > ELEMENT_NUMBER_MILLION_CUTOFF)
        {
            elem_string = QString("~%1 M").arg(QString::number(num_elements / ELEMENT_NUMBER_MILLION_CUTOFF));
        }
        else if (ELEMENT_NUMBER_THOUSAND_CUTOFF)
        {
            elem_string = QString("~%1 K").arg(QString::number(num_elements / ELEMENT_NUMBER_THOUSAND_CUTOFF));
        }

        else
        {
            elem_string = QString::number(num_elements);
        }

        return QString("rows: %1\n cols: %2\n total: %3")
            .arg(QString::number(rows))
            .arg(QString::number(cols))
            .arg(elem_string);
    }

}

#endif // UIHELPERS_H
