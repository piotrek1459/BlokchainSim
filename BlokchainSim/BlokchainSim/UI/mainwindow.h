#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QCheckBox>
#include "../Blockchain/Blockchain.h"
#include "../Node/FullNode.h"
#include "../Node/MinerNode.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onAddBlockClicked();
    void onValidateChainClicked();
    void onSaveChainClicked();

private:
    Blockchain m_blockchain;
    Blockchain m_chain;                      // single ledger
    FullNode  m_fullNode{ m_chain };
    MinerNode m_minerNode{ m_chain, 2 };   
    Node * m_activeNode{ &m_minerNode };    // pointer to whichever is selected

    // Basic UI
    QListWidget* m_listWidget;
    QPushButton* m_addBlockButton;
    QPushButton* m_validateButton;
    QPushButton* m_saveButton;
    QCheckBox* m_powCheck;

    void updateStatusBar();
    void refreshChainDisplay();
};

#endif // MAINWINDOW_H
