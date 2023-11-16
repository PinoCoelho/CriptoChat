#ifndef MAINWINDOW_SERVER_H
#define MAINWINDOW_SERVER_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <string>

void handleErrors();
void encrypt(const std::string& plaintext, const unsigned char* key, const unsigned char* iv, std::string& ciphertext);
void decrypt(const std::string& ciphertext, const unsigned char* key, const unsigned char* iv, std::string& decryptedText);
void generateHMAC(const std::string& data, const unsigned char* key, std::string& hmac);


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
    void onEncryptButtonClicked();
    void onDecryptButtonClicked();

private:
    Ui::MainWindow *ui;

    QVBoxLayout *layout;
    QPushButton *encryptButton;
    QPushButton *decryptButton;
    QPushButton *showOriginalButton;
    QLabel *resultLabel;

    std::string originalText;
    std::string ciphertext;
    std::string decryptedText;
    unsigned char key[16];
    unsigned char iv[AES_BLOCK_SIZE];

    void generateKeyAndIV();
    void encryptText();
    void decryptText();
};

#endif // MAINWINDOW_SERVER_H