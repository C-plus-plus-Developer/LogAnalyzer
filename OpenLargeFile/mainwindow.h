#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <boost/iostreams/device/mapped_file.hpp> // Для mmap
#include <string_view>
#include "ErrorBlock.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonClose_clicked();
    void on_buttonPrev_clicked();
    void on_buttonNext_clicked();
    void on_findWordConfirm_clicked();
    void on_pathConfirm_clicked();

private:
    Ui::MainWindow *ui;

    boost::iostreams::mapped_file_source m_file;
    std::string_view m_fileView;
    size_t m_currentPos = 0;
    void displayContext(size_t pos);
};

#endif // MAINWINDOW_H
