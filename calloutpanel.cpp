#include "calloutpanel.h"

#include <QFocusEvent>

CalloutPanel::CalloutPanel(QWidget* parent)
    : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
}

void CalloutPanel::focusOutEvent(QFocusEvent* e) {
    if (auto w = focusWidget(); w->parentWidget() != this) {
        hideOnFocusOut();
        w->setFocusProxy(nullptr);
    } else {
        w->setFocusProxy(this);
    }
}

void CalloutPanel::callout() {
    show();
    setFocus();
    activateWindow();
}

void CalloutPanel::hideOnFocusOut() {
    if (isVisible() && parent() == nullptr) {
        hide();
    }
}
