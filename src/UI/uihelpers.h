#ifndef UIHELPERS_H
#define UIHELPERS_H

#define ELEMENT_NUMBER_MILLION_CUTOFF 1000000
#define ELEMENT_NUMBER_THOUSAND_CUTOFF 1000

#define LONGER_MESSAGE_TIME 3000
#define STANDARD_MESSAGE_TIME 2000

#include <QtWidgets>
#include <QString>

namespace uihelp {


inline static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode, QFileDialog::FileMode filemode = QFileDialog::FileMode::DirectoryOnly)
{
    static bool firstDialog = true;
    dialog.setFileMode(filemode);
    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();

    
        
    if (acceptMode == QFileDialog::AcceptOpen) mimeTypeFilters.append("image/jpeg");
       
    mimeTypeFilters.append("image/png");
    mimeTypeFilters.append("application/octet-stream");
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    // dialog.selectMimeTypeFilter("image/png");

    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("png");
    
    
}

inline static void hideListItems(QListWidget * list){
    for (int i = 0; i < list->count(); ++i)
    {
        list->item(i)->setHidden(true);
    }
    
}

inline static void filterListItems(QListWidget * list, QString filter_string){
    hideListItems(list);
    QList<QListWidgetItem*> matches ( list->findItems(filter_string, Qt::MatchFlag::MatchContains) );
    for(QListWidgetItem* item : matches)
    item->setHidden(false);
}

inline static QString generateSizeMessage(int rows, int cols){
    long int num_elements = rows * cols;
    QString elem_string;
    if(num_elements > ELEMENT_NUMBER_MILLION_CUTOFF){
        elem_string = QString("~%1 K").arg(QString::number(num_elements/ELEMENT_NUMBER_MILLION_CUTOFF));
    }
    else if (ELEMENT_NUMBER_THOUSAND_CUTOFF)
    {
        elem_string = QString("~%1 M").arg(QString::number(num_elements/ELEMENT_NUMBER_THOUSAND_CUTOFF));
    }
    
    else{
        elem_string =QString::number(num_elements);
    }
    
    return QString("rows: %1\n cols: %2\n total: %3")
                                .arg(QString::number(rows))
                                .arg(QString::number(cols))
                                .arg(elem_string);
}

}


#endif // UIHELPERS_H
