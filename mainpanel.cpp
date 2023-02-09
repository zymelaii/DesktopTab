#include "mainpanel.h"

#include "utils.h"

#include <QScreen>
#include <QFocusEvent>
#include <QFileDialog>
#include <QRadioButton>
#include <QLineEdit>
#include <QStandardPaths>
#include <QLayout>
#include <QSpacerItem>
#include <QDir>
#include <QFrame>
#include <QDebug>

MainPanel::MainPanel(QWidget* parent)
    : CalloutPanel(parent)
    , appHome(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)) {
    QDir appLocalDir(appHome);
    if (!appLocalDir.exists()) {
        appLocalDir.mkdir(appHome);
        appLocalDir.mkdir(".public");
    }

    setupUi();
}

void MainPanel::setupUi() {
    lbCurrentPath = new QLabel(getKnownFolderPath(FOLDERID_Desktop), this);
    auto btSelect = new QPushButton(tr("Select"), this);
    auto btRevert = new QPushButton(tr("Revert"), this);
    auto btPublic = new QRadioButton("Show public", this);
    btPublic->setChecked(QDir(getKnownFolderPath(FOLDERID_PublicDesktop)).absolutePath() !=
                         QDir(appHome + R"(\.public)").absolutePath());

    auto row1 = new QHBoxLayout;
    row1->addWidget(new QLabel(tr("Current Path:"), this));
    row1->addWidget(lbCurrentPath);

    auto row2 = new QHBoxLayout;
    row2->addWidget(btSelect);
    row2->addWidget(btRevert);
    row2->addWidget(btPublic);

    auto layout = new QVBoxLayout(this);
    layout->addLayout(row1);
    layout->addLayout(row2);

    connect(btSelect, &QPushButton::clicked, this, [this] {
        auto folder = QFileDialog::getExistingDirectory(this, tr("Select desktop folder"));
        if (switchDesktopTo(folder)) {
            lbCurrentPath->setText(folder);
        }
        callout();
    });

    connect(btRevert, &QPushButton::clicked, this, [this, button = btPublic] {
        if (revertDesktopPath()) {
            lbCurrentPath->setText(getKnownFolderPath(FOLDERID_Desktop));
        }
        emit button->clicked(true);
        callout();
    });

    connect(btPublic, &QRadioButton::clicked, this, [this, button = btPublic](bool checked) {
        button->setChecked(checked);
        if (checked) {
            setKnownFolderPath(FOLDERID_PublicDesktop, R"(C:\Users\Public\Desktop)");
        } else {
            setKnownFolderPath(FOLDERID_PublicDesktop, appHome + R"(\.public)");
        }
    });
}

void MainPanel::callout() {
    CalloutPanel::callout();
    auto rect = screen()->geometry();
    move((rect.width() - width()) / 2, (rect.height() - height()) / 2);
}

bool MainPanel::switchDesktopTo(const QString& path) {
    auto currentDesktopPath = QDir(getKnownFolderPath(FOLDERID_Desktop)).absolutePath();
    auto targetPath			= QDir(path).absolutePath();
    if (targetPath == currentDesktopPath) return true;
    auto resp = setKnownFolderPath(FOLDERID_Desktop, targetPath);
    if (!resp) return false;
    if (weightedHistory.contains(targetPath)) {
        ++weightedHistory[targetPath];
    } else {
        weightedHistory[targetPath] = 1;
    }
    return true;
}
