#ifndef FILTERWINDOW_H
#define FILTERWINDOW_H

#include <QMainWindow>
#include "Filter.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class FilterWindow;
}
QT_END_NAMESPACE

using AppLogic::Filter;

class FilterWindow : public QMainWindow
{
    Q_OBJECT

public:
    FilterWindow(QWidget *parent = nullptr);
    ~FilterWindow();

private:
    Ui::FilterWindow *ui;
    Filter filter_calculator;
};
#endif // FILTERWINDOW_H
