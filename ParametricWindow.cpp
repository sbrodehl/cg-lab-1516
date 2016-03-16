#include "ParametricWindow.h"

ParametricWindow::ParametricWindow(QWidget *parent)
        : QMainWindow(parent) {
}

ParametricWindow::ParametricWindow(Part &part) {
    resize(512, 256);

}
