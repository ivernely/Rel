#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QSystemSemaphore semaphore("Reliability_test_AT", 1);
    semaphore.acquire();

#ifndef Q_OS_WIN32
    // linux/unix
    QSharedMemory nix_fix_shared_memory("Reliability_test_AT_2");
    if(nix_fix_shared_memory.attach()){
        nix_fix_shared_memory.detach();
    }
#endif

    QSharedMemory sharedMemory("Reliability_test_AT_2");
    bool is_running;

    if (sharedMemory.attach())
        is_running = true;
    else{
        sharedMemory.create(1);
        is_running = false;
    }

    semaphore.release();

    if(is_running){
            QMessageBox msg;
            msg.addButton(QObject::trUtf8("OK"), QMessageBox::AcceptRole);
            msg.setText(QObject::trUtf8("Application is already running.\n"
                                        "You can start only one example of application."));
            msg.setWindowTitle(QObject::trUtf8("Error"));
            msg.setIcon(QMessageBox::Critical);
            msg.exec();
            return 1;
            }

    w.setWindowTitle(QObject::tr("Reliability testing"));

    w.show();

    return a.exec();
}
