/*
    SPDX-FileCopyrightText: 2018 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
    Work sponsored by the LiMux project of the city of Munich

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SSHTUNNELTHREAD_H
#define SSHTUNNELTHREAD_H

#include <QThread>

#include <QByteArray>
#include <QString>

#include <atomic>

#include <libssh/libssh.h>

#include "krdccore_export.h"

class KRDCCORE_EXPORT SshTunnelThread : public QThread
{
    Q_OBJECT
public:
    SshTunnelThread(const QByteArray &host, int port, int tunnelPort, int sshPort, const QByteArray &sshUserName, bool loopback);
    ~SshTunnelThread() override;

    enum PasswordOrigin {
        PasswordFromWallet,
        PasswordFromDialog,
    };

    enum PasswordRequestFlags {
        NoFlags,
        IgnoreWallet,
    };

    int tunnelPort() const;

    QString password() const;
    void setPassword(const QString &password, PasswordOrigin origin);
    void userCanceledPasswordRequest();

    void run() override;

Q_SIGNALS:
    void passwordRequest(PasswordRequestFlags flags);
    void listenReady();
    void errorMessage(const QString &message);

private:
    QByteArray m_host;
    int m_port;
    int m_tunnelPort;
    int m_sshPort;
    QByteArray m_sshUserName;
    bool m_loopback;
    QString m_password;
    PasswordOrigin m_passwordOrigin;
    bool m_passwordRequestCanceledByUser;

    std::atomic_bool m_stop_thread;
};

#endif
