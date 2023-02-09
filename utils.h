#ifndef UTILS_H
#define UTILS_H

#include <shlobj.h>
#include <QString>
#include <QDebug>

inline bool setKnownFolderPath(const KNOWNFOLDERID& rfid, const QString& path) {
    wchar_t pathBuf[MAX_PATH]{};
    path.toWCharArray(pathBuf);
    auto resp = SHSetKnownFolderPath(rfid, 0, nullptr, pathBuf);
    if (!SUCCEEDED(resp)) return false;
    LPITEMIDLIST list = nullptr;
    SHGetKnownFolderIDList(FOLDERID_Desktop, 0, nullptr, &list);
    SHChangeNotify(SHCNE_UPDATEDIR, SHCNF_FLUSH | SHCNF_IDLIST, list, nullptr);
    return true;
}

inline QString getKnownFolderPath(const KNOWNFOLDERID& rfid) {
    wchar_t* path = nullptr;
    SHGetKnownFolderPath(rfid, 0, nullptr, &path);
    QString resp = QString::fromWCharArray(path);
    CoTaskMemFree(path);
    path = nullptr;
    return std::move(resp);
}

inline bool revertDesktopPath() {
    wchar_t nameBuf[MAX_PATH];
    DWORD	bufLen = MAX_PATH;
    GetUserName(nameBuf, &bufLen);
    auto resp = QStringLiteral(R"(C:\Users\%1\Desktop)").arg(QString::fromWCharArray(nameBuf));
    return setKnownFolderPath(FOLDERID_Desktop, resp);
}

#endif	 // UTILS_H
