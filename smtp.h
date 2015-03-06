/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			smtp.h
  \date			November 2014
  \author		Raivis Strogonovs

  \brief		Contains

  Retreived from https://github.com/xcoder123/SimpleSmtp_SSL_QT5 on November 1st, 2014

*/

/*
Copyright (c) 2013 Raivis Strogonovs

http://morf.lv

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef __CUTTILES_SUPPORT_SMTP_INC__
#define __CUTTILES_SUPPORT_SMTP_INC__
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <simplesmtp/simplesmtp-config.h>
#include <usermsg/usermsg.h>

#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QSslSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QtWidgets/QMessageBox>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

//! send an email using an smtp server
class SIMPLESMTP_EXPORT Smtp : public QObject
{
    Q_OBJECT

    //
    //
    //
    //
    /*  DEFINITIONS    ----------------------------------------------------- */

public:

    //! the states that we can have
    enum states{
        Tls,
        HandShake,
        Auth,
        User,
        Pass,
        Rcpt,
        Mail,
        Data,
        Init,
        Body,
        Quit,
        Close
    };

    /*  DEFINITIONS    ===================================================== */
    //
    //
    //
    //
    /*  DATA    ------------------------------------------------------------ */

private:

    int timeout;
    QString message;
    QSslSocket *socket;
    QTextStream *t;
    QString rcpt;
    QString response;
    QString user;
    QString pass;
    QString host;
    int port;
    int state;

    /*  DATA    ============================================================ */
    //
    //
    //
    //
    /*  FUNCTIONS    ------------------------------------------------------- */


public:

    //! constructor
    Smtp(
            const QString &user,
            const QString &pass,
            const QString &host,
            int port = 465,
            int timeout = 30000);

    //! constructor
    Smtp (
            const Smtp & other);

    //! destructor
    virtual ~Smtp();

    //! send the mail
    bool
    sendMail (
            UserMsg & um,
            const QString &to,
            const QString &subject,
            const QString &body,
            bool body_is_html = false,
            QStringList files = QStringList());

signals:

    //! the user can get notifications of the progress
    void
    status(
            const QString &);

private slots:

    void
    stateChanged(
            QAbstractSocket::SocketState socketState);

    void
    errorReceived(
            QAbstractSocket::SocketError socketError);

    void
    disconnected();

    void
    connected();

    void
    readyRead();

    void
    sslErrors(
            QList<QSslError>);

private:

    static bool
    attachFiles (
            QString & message,
            QStringList files,
            UserMsg & um);

    void
    doConnections();

    /*  FUNCTIONS    ======================================================= */
    //
    //
    //
    //

};	/*	class Smtp	*/

/*  CLASS    =============================================================== */
//
//
//
//

#endif // __CUTTILES_SUPPORT_SMTP_INC__
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
