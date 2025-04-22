#include "mainwindow.h"
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QStatusBar>
#include "../Blockchain/FileManager.h"
#include "../Blockchain/ChainStats.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Create central widget + layout
    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);

    // Create UI elements
    m_listWidget = new QListWidget(this);
    m_addBlockButton = new QPushButton("Add Block", this);
    m_validateButton = new QPushButton("Validate Chain", this);
    m_saveButton = new QPushButton("Save Chain", this);

    // Add to layout
    layout->addWidget(m_listWidget);
    layout->addWidget(m_addBlockButton);
    layout->addWidget(m_validateButton);
    layout->addWidget(m_saveButton);

    setCentralWidget(central);

    // Connect signals
    connect(m_addBlockButton, &QPushButton::clicked,
        this, &MainWindow::onAddBlockClicked);

    connect(m_validateButton, &QPushButton::clicked,
        this, &MainWindow::onValidateChainClicked);

    connect(m_saveButton, &QPushButton::clicked,
        this, &MainWindow::onSaveChainClicked);

    // Refresh display initially
    refreshChainDisplay();
    updateStatusBar();
}

MainWindow::~MainWindow()
{
}

void MainWindow::onAddBlockClicked()
{
    bool ok;
    QString data = QInputDialog::getText(
        this, tr("Add New Block"),
        tr("Block Data:"),
        QLineEdit::Normal,
        "", &ok);
    if (ok && !data.isEmpty()) {
        m_blockchain.addBlock(data.toStdString());
        refreshChainDisplay();
    }
}

void MainWindow::onValidateChainClicked()
{
    bool valid = m_blockchain.isChainValid();
    QMessageBox::information(this, "Chain Validation",
        valid ? "Blockchain is VALID" : "Blockchain is INVALID");
}

void MainWindow::updateStatusBar()      // NEW
{
    using namespace std::string_literals;
    auto stats = ChainStats::compute(m_blockchain);
        statusBar()->showMessage(
            QString("Blocks: %1 | Avg nonce: %2")
            .arg(m_blockchain.getChain().size())
            .arg(stats.averageNonce, 0, 'f', 1));
}

void MainWindow::onSaveChainClicked()
{
    try {
        FileManager::save(m_blockchain);
        QMessageBox::information(this, "Save Chain", "Blockchain saved to disk.");
    }
    catch (const std::exception& ex) {
        QMessageBox::critical(this, "Save Chain", ex.what());
    }
}

void MainWindow::refreshChainDisplay()
{
    m_listWidget->clear();
    const auto& chain = m_blockchain.getChain();
    for (const auto& block : chain) {
        // Build a more descriptive string
        QString itemStr = QString("Block %1: \n  Timestamp: %2 \n  Nonce: %3 \n  Data: %4 \n  PrevHash: %5 \n  Hash: %6")
            .arg(block.getIndex())
            .arg(block.getTimestamp())
            .arg(block.getNonce())
            .arg(QString::fromStdString(block.getData()))
            .arg(QString::fromStdString(block.getPrevHash()))
            .arg(QString::fromStdString(block.getHash()));
        m_listWidget->addItem(itemStr);
    }
	updateStatusBar();
}

