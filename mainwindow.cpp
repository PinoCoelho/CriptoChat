#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <openssl/rand.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    layout = new QVBoxLayout();

    encryptButton = new QPushButton("Encrypt");
    connect(encryptButton, SIGNAL(clicked()), this, SLOT(onEncryptButtonClicked()));
    layout->addWidget(encryptButton);

    decryptButton = new QPushButton("Decrypt");
    connect(decryptButton, SIGNAL(clicked()), this, SLOT(onDecryptButtonClicked()));
    layout->addWidget(decryptButton);

    showOriginalButton = new QPushButton("Show Original");
    connect(showOriginalButton, &QPushButton::clicked, [=]() {
        resultLabel->setText(QString::fromStdString("Texto original: " + originalText));
    });
    layout->addWidget(showOriginalButton);

    resultLabel = new QLabel();
    layout->addWidget(resultLabel);

    ui->centralwidget->setLayout(layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateKeyAndIV()
{
    if (RAND_bytes(key, sizeof(key)) != 1)
        handleErrors();

    if (RAND_bytes(iv, sizeof(iv)) != 1)
        handleErrors();
}

void MainWindow::encryptText()
{
    encrypt(originalText, key, iv, ciphertext);
}

void MainWindow::decryptText()
{
    decrypt(ciphertext, key, iv, decryptedText);
}

void MainWindow::onEncryptButtonClicked()
{
    originalText = "Pino trafica gomitas en el edificio F2 y esta armado";
    generateKeyAndIV();
    encryptText();

    resultLabel->setText(QString::fromStdString("Texto cifrado: " + ciphertext));
}

void MainWindow::onDecryptButtonClicked()
{
    decryptText();

    resultLabel->setText(QString::fromStdString("Texto descifrado: " + decryptedText));
}
