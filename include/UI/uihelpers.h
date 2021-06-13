#ifndef UIHELPERS_H
#define UIHELPERS_H

#define ELEMENT_NUMBER_MILLION_CUTOFF 1000000
#define ELEMENT_NUMBER_THOUSAND_CUTOFF 1000

#define LONGER_MESSAGE_TIME 3000
#define STANDARD_MESSAGE_TIME 2000

#include <QtWidgets>
#include <QString>


namespace uihelp
{

    typedef enum {
    FD_IMAGE,
    FD_ZIP

} file_dialog_type_t;

inline static QString allSupportedFormatsString(QStringList mimeTypeFilters){
    QMimeDatabase mimeDB;
    QStringList allSupportedFormats;
    for(const QString& mimeTypeFilter: mimeTypeFilters) {
        QMimeType mimeType = mimeDB.mimeTypeForName(mimeTypeFilter);
        if(mimeType.isValid()) {
            allSupportedFormats.append(mimeType.globPatterns());
        }
    }
    return QString("All supported formats (%1)").arg(allSupportedFormats.join(' '));

}
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

        if (filemode == QFileDialog::FileMode::DirectoryOnly) return;

        QStringList mimeTypeFilters;
        // const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        //                                               ? QImageReader::supportedMimeTypes()
        //                                               : QImageWriter::supportedMimeTypes();

        // mimeTypeFilters.append("application/octet-stream");


        if (file_dialog_type == file_dialog_type_t::FD_IMAGE){
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
        
        // mimeTypeFilters.sort();
        // QString allSupportedFormats = allSupportedFormatsString(mimeTypeFilters);
        // dialog.setMimeTypeFilters(mimeTypeFilters);
        // QStringList nameFilters = dialog.nameFilters();
        // nameFilters.append(allSupportedFormats);
        // dialog.setNameFilters(nameFilters);
        // dialog.selectNameFilter(allSupportedFormats);
        // dialog.selectMimeTypeFilter("image/png");

        // if (acceptMode == QFileDialog::AcceptSave)
        //     dialog.setDefaultSuffix("png");
    }

    inline static void hideListItems(QListWidget *list)
    {
        for (int i = 0; i < list->count(); ++i)
        {
            list->item(i)->setHidden(true);
        }
    }

    inline static void filterListItems(QListWidget *list, QString filter_string)
    {
        hideListItems(list);
        QList<QListWidgetItem *> matches(list->findItems(filter_string, Qt::MatchFlag::MatchContains));
        for (QListWidgetItem *item : matches)
            item->setHidden(false);
    }

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
