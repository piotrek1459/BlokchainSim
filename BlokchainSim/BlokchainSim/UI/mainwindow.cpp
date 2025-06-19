#include "mainwindow.h"
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QStatusBar>
#include "../Blockchain/FileManager.h"
#include "../Blockchain/ChainStats.h"
#include "../Blockchain/Transaction.h"      

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{

    //Load persistent chain BEFORE constructing nodes
    m_chain = FileManager::load();
    qDebug() << "Loaded ledger from" << QString::fromStdString(FileManager::defaultPath().string());


    // Create central widget + layout
    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);

    // Create UI elements
    m_listWidget = new QListWidget(this);
    m_addBlockButton = new QPushButton("Add Block", this);
    m_validateButton = new QPushButton("Validate Chain", this);
    m_saveButton = new QPushButton("Save Chain", this);
    m_powCheck = new QCheckBox("Proof‑of‑Work (mining)", this);
    m_powCheck->setChecked(true);          // default ON

    // Add to layout
    layout->addWidget(m_listWidget);
    layout->addWidget(m_addBlockButton);
    layout->addWidget(m_validateButton);
    layout->addWidget(m_saveButton);
    layout->addWidget(m_powCheck);

    setCentralWidget(central);

    // Connect signals
    connect(m_addBlockButton, &QPushButton::clicked,
        this, &MainWindow::onAddBlockClicked);

    connect(m_validateButton, &QPushButton::clicked,
        this, &MainWindow::onValidateChainClicked);

    connect(m_saveButton, &QPushButton::clicked,
        this, &MainWindow::onSaveChainClicked);

    connect(m_powCheck, &QCheckBox::toggled, [this](bool on) {
        m_activeNode = on ? static_cast<Node*>(&m_minerNode) : static_cast<Node*>(&m_fullNode);
        refreshChainDisplay();             // show that node’s view
     });

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

    if (!ok || data.isEmpty())
        return;                                   

    try {
        // 1. validate pattern  → throws if not OK
        Transaction::parse(data.toStdString());

        // 2. add via polymorphic Node
        m_activeNode->createBlock(data.toStdString());
        refreshChainDisplay();
    }
    catch (const std::exception&) {
        QMessageBox::warning(this,
            "Invalid transaction",
            "Input must follow the pattern:\n"
            "<sender>-><receiver>:<amount>\n\n"
            "Example:  Alice->Bob:50");
    }
}


void MainWindow::onValidateChainClicked()
{
    bool valid = m_activeNode->getBlockchain().isChainValid();
    QMessageBox::information(this, "Chain Validation",
        valid ? "Blockchain is VALID" : "Blockchain is INVALID");
}

void MainWindow::updateStatusBar()      // NEW
{
    using namespace std::string_literals;
    auto stats = ChainStats::compute(m_activeNode->getBlockchain());
    statusBar()->showMessage(
        QString("Blocks: %1 | Avg nonce: %2")
        .arg(m_activeNode->getBlockchain().getChain().size())
        .arg(stats.averageNonce, 0, 'f', 1));
}

void MainWindow::onSaveChainClicked()
{
    try {
        FileManager::save(m_activeNode->getBlockchain());
        QMessageBox::information(this, "Save Chain", "Blockchain saved to disk.");
    }
    catch (const std::exception& ex) {
        QMessageBox::critical(this, "Save Chain", ex.what());
    }
}

void MainWindow::refreshChainDisplay()
{
    m_listWidget->clear();
    const auto& chain = m_activeNode->getBlockchain().getChain();
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

