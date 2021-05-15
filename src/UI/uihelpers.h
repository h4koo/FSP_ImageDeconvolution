#ifndef UIHELPERS_H
#define UIHELPERS_H

#include <QtWidgets>

namespace uihelp {


inline static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;
    dialog.setFileMode(QFileDialog::FileMode::Directory);
    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
//    foreach (const QByteArray &mimeTypeName, supportedMimeTypes){
//        if (mimeTypeName == "image/jpeg" || mimeTypeName == "image/png")
        mimeTypeFilters.append("image/jpeg");
        mimeTypeFilters.append("image/png");

//    }
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

}


#endif // UIHELPERS_H
