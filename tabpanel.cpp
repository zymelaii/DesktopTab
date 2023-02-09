#include "tabpanel.h"

#include <QScreen>
#include <QFocusEvent>
#include <QSizePolicy>
#include <QLayout>
#include <QListView>
#include <QStringListModel>
#include <QDir>

TabPanel::TabPanel(QWidget* parent)
	: CalloutPanel(parent)
	, model(nullptr)
	, records(nullptr) {
	setFocusPolicy(Qt::ClickFocus);

	setupUi();

	setFixedWidth(160);
	setMaximumHeight(256);

	setStyleSheet(R"(
QScrollBar {
    background: transparent;
    width: 6px;
    margin: 1px 1px 1px 1px;
}
QScrollBar::handle {
    border-radius: 2px;
}
QScrollBar::handle:normal {
    background: #d0d0d0;
}
QScrollBar::handle:hover {
    background: #a0a0a0;
}
QScrollBar::handle:pressed {
    background: #808080;
}
QScrollBar::add-line
, QScrollBar::sub-line
, QScrollBar::add-page
, QScrollBar::sub-page {
    background: transparent;
    border: none;
})");
}

void TabPanel::setupUi() {
	model	  = new QStringListModel(this);
	lvHistory = new QListView(this);
	lvHistory->setModel(model);
	lvHistory->setFont(QFont("Source Code Pro for Powerline"));

	auto layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(lvHistory);

	//! FIXME: 赶着出效果，TabPanel 这边数据模型的部分和交互都没好好做，待修
	connect(lvHistory, &QListView::clicked, this, [this](const QModelIndex& index) {
		auto history = records->keys();
		std::sort(history.begin(),
				  history.end(),
				  [&weight = *records](const QString& lhs, const QString& rhs) {
					  return weight[lhs] > weight[rhs];
				  });
		emit select(history[index.row()]);
		hide();
	});
}

void TabPanel::focusInEvent(QFocusEvent* e) {
	lvHistory->setFocus();
}

void TabPanel::callout() {
	CalloutPanel::callout();

	auto history = records->keys();
	std::sort(history.begin(),
			  history.end(),
			  [&weight = *records](const QString& lhs, const QString& rhs) {
				  return weight[lhs] > weight[rhs];
			  });
	std::transform(history.begin(), history.end(), history.begin(), [](const QString& path) {
		QDir dir(path);
		return dir.isRoot() ? dir.path() : dir.dirName();
	});
	model->setStringList(history);

	auto rect = screen()->geometry();
	move(rect.width() - width() - 16, (rect.height() - height()) / 2);
}

void TabPanel::bind(QMap<QString, size_t>* target) {
	if (target != nullptr) {
		records = target;
	}
}
