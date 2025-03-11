#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include "../Blockchain/Blockchain.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onAddBlockClicked();
    void onValidateChainClicked();

private:
    Blockchain m_blockchain;

    // Basic UI
    QListWidget* m_listWidget;
    QPushButton* m_addBlockButton;
    QPushButton* m_validateButton;

    void refreshChainDisplay();
};

#endif // MAINWINDOW_H
