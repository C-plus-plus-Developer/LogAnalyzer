#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSettings>
#include <QTextCursor>
#include <QTextCharFormat>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings;

    QString lastPath = settings.value("lastOpenedFile").toString();
    if(!lastPath.isEmpty()){
        ui->enterPathFile->setText(lastPath);
    }

    QString lastWord = settings.value("lastSearchWord").toString();
    if(!lastWord.isEmpty()){
        ui->enterFindWord->setText(lastWord);
    }

    // Блокируем кнопки навигации при старте, так как файл не открыт
    ui->buttonNext->setEnabled(false);
    ui->buttonPrev->setEnabled(false);

    //поле вывода только для чтения
    ui->outputFile->setReadOnly(true);

    //шрифт как в консоли, чтобы логи выглядели ровно
    QFont font("Consolas");
    font.setStyleHint(QFont::Monospace);
    ui->outputFile->setFont(font);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Подвердить путь
void MainWindow::on_pathConfirm_clicked()
{
    //Настройка цвета кнопки если путь верный(зеленый)
    QString clickedStyleTrue =
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: black;"
        "}";
    //Если не верный то красный
    QString clickedStyleFalse =
        "QPushButton {"
        "   background-color: #FF0000;"
        "   color: black;"
        "}";

    // Берем путь из поля enterPathFile
    QString path = ui->enterPathFile->text();

    if (m_file.is_open()) m_file.close();

    try {
        m_file.open(path.toStdString());

        if (m_file.is_open()) {

            ui->pathConfirm->setStyleSheet(clickedStyleTrue);

            // Создаем view на весь файл
            m_fileView = std::string_view(m_file.data(), m_file.size());
            m_currentPos = 0;

            QSettings settings;
            settings.setValue("lastOpenedFile", path);

            ui->outputFile->setPlainText("Файл успешно открыт! Введите слово для поиска (например, ERROR).");

            // Разблокируем кнопки
            ui->buttonNext->setEnabled(true);
            ui->buttonPrev->setEnabled(true);
        }
    } catch (const std::exception& e) {
        ui->pathConfirm->setStyleSheet(clickedStyleFalse);
        QMessageBox::critical(this, "Ошибка", QString("Не удалось открыть файл:\n%1").arg(e.what()));
    }
}

//Подвердить слово
void MainWindow::on_findWordConfirm_clicked()
{
    if (!m_file.is_open()) {
        QMessageBox::warning(this, "Внимание", "Сначала откройте файл!");
        return;
    }

    QString word = ui->enterFindWord->text();
    if (word.isEmpty()) return;

    QSettings settings;
    settings.setValue("lastSearchWord", word);

    // Ищем с самого начала файла (с позиции 0)
    size_t found = m_fileView.find(word.toStdString(), 0);

    if (found != std::string_view::npos) {
        m_currentPos = found;
        displayContext(m_currentPos);
    } else {
        ui->outputFile->setPlainText("Слово не найдено во всем файле.");
    }
}

//Кнопка далее
void MainWindow::on_buttonNext_clicked()
{
    if (!m_file.is_open()) return;

    std::string word = ui->enterFindWord->text().toStdString();
    if (word.empty()) return;

    // Начинаем поиск СРАЗУ ПОСЛЕ текущего найденного слова
    size_t startSearch = m_currentPos + word.length();

    if (startSearch >= m_fileView.size()) {
        QMessageBox::information(this, "Инфо", "Конец файла достигнут.");
        return;
    }

    size_t found = m_fileView.find(word, startSearch);

    if (found != std::string_view::npos) {
        m_currentPos = found;
        displayContext(m_currentPos);
    } else {
        QMessageBox::information(this, "Инфо", "Больше совпадений нет (Конец файла).");
    }
}
//Кнопка назад
void MainWindow::on_buttonPrev_clicked()
{
    if (!m_file.is_open()) return;

    std::string word = ui->enterFindWord->text().toStdString();
    if (word.empty()) return;

    if (m_currentPos == 0) {
        QMessageBox::information(this, "Инфо", "Мы в самом начале файла.");
        return;
    }

    // Ищем в обратную сторону от позиции ПЕРЕД текущим словом
    size_t found = m_fileView.rfind(word, m_currentPos - 1);

    if (found != std::string_view::npos) {
        m_currentPos = found;
        displayContext(m_currentPos);
    } else {
        QMessageBox::information(this, "Инфо", "Больше совпадений нет (Начало файла).");
    }
}

//Вывод текста
void MainWindow::displayContext(size_t pos)
{
    // Берем 3 строки до и 5 после (через функцию в ErrorBlock.h)
    std::string_view ctx = getContext(m_fileView, pos, 3, 5);
    QString text = QString::fromUtf8(ctx.data(), ctx.size());
    // Выводим
    ui->outputFile->setPlainText(QString::fromUtf8(ctx.data(), ctx.size()));

    QString target = ui->enterFindWord->text();
    if(target.isEmpty()) return;

    QTextDocument *doc = ui->outputFile->document();
    QTextCursor cursor(doc);

    //Подсветка искомого слова
    QTextCharFormat highlightFormat;
    highlightFormat.setForeground(Qt::red);
    highlightFormat.setFontWeight(QFont::Bold);

    // Бегаем по тексту и ищем слово
    while (!cursor.isNull() && !cursor.atEnd()) {
        // Ищем слово, начиная с текущей позиции курсора
        cursor = doc->find(target, cursor);

        if (!cursor.isNull()) {
            // Если нашли - применяем формат к выделению
            cursor.mergeCharFormat(highlightFormat);
        }
    }
}

void MainWindow::on_buttonClose_clicked()
{
    close();
}

