/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			smtp.cc
  \date			November 2014
  \author		Raivis Strogonovs

  \brief		Contains the definition for ...

  Retreived from http://qt-project.org/wiki/Simple_encryption on November 1st, 2014

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
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "smtp.h"
#include "simplesmtp-private.h"

#include <QAbstractSocket>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define SMTP_NEW_LINE "\r\n"

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
Smtp::Smtp(
        const QString &user, const QString &pass,
        const QString &host, int port, int timeout ) :
    QObject(),
    timeout(timeout),
    message(),
    socket(new QSslSocket(this)),
    t(new QTextStream (socket)),
    rcpt(),
    response(),
    user(user),
    pass(pass),
    host(host),
    port(port),
    state(Close),
    ignore_ssl_errors_(true)
{
    doConnections();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
Smtp::Smtp (const Smtp & other):
    QObject(),
    timeout(other.timeout),
    message(),
    socket(new QSslSocket(this)),
    t(new QTextStream (socket)),
    rcpt(),
    response(),
    user(other.user),
    pass(other.pass),
    host(other.host),
    port(other.port),
    state(Close),
    ignore_ssl_errors_(other.ignore_ssl_errors_)
{
    doConnections();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Smtp::doConnections ()
{
    connect(socket, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(sslErrors(QList<QSslError>)));
    connect(socket, SIGNAL(readyRead()),
            this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()),
            this, SLOT(connected() ) );
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(errorReceived(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(disconnected()),
            this, SLOT(disconnected()));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool Smtp::attachFiles (
        QString & message, QStringList files, UserMsg & um)
{
    bool b_file_failure = false;
    if (!files.isEmpty()) {
        SIMPLESMTP_DEBUGM ("Files to be sent: %d\n", files.size());

        foreach(QString filePath, files) {
            QFile file (filePath);

            if (!file.exists()) {
                um.addErr (tr("File %1 does not exists").arg(filePath));
                b_file_failure = true;
                continue;
            }

            if (!file.open(QIODevice::ReadOnly)) {
                um.addErr (tr("Unable to open file %1").arg(filePath));
                b_file_failure = true;
                continue;
            }

            if (b_file_failure) continue;

            QByteArray bytes = file.readAll();
            message.append( "--frontier\n" );
            message.append( "Content-Type: application/octet-stream\nContent-Disposition: attachment; filename="+ QFileInfo(file.fileName()).fileName() +";\nContent-Transfer-Encoding: base64\n\n" );
            message.append(bytes.toBase64());
            message.append("\n");
        }
    } else {
        SIMPLESMTP_DEBUGM ("No attachments found\n");
        b_file_failure = true;
    }

    return b_file_failure;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
Smtp::~Smtp()
{
    delete t;
    delete socket;

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool Smtp::sendMail (
        UserMsg & um, const QString &to,
        const QString &subject, const QString &body,
        bool body_is_html, QStringList files)
{
    bool b_ret = false;
    for (;;) {

        message = "To: " + to + "\n";
        message.append("From: " + user + "\n");
        message.append("Subject: " + subject + "\n");

        //Let's intitiate multipart MIME with cutting boundary "frontier"
        message.append("MIME-Version: 1.0\n");
        message.append("Content-Type: multipart/mixed; boundary=frontier\n\n");

        message.append( "--frontier\n" );
        if (body_is_html) {
            message.append ("Content-Type: text/html\n\n");
        } else {
            message.append ("Content-Type: text/plain\n\n");
        }

        message.append (body);
        message.append ("\n\n");

        if (!attachFiles(message, files, um)) break;

        message.append( "--frontier--\n" );

        message.replace(
                    QString::fromLatin1( "\n" ),
                    QString::fromLatin1( SMTP_NEW_LINE ) );
        message.replace(
                    QString::fromLatin1( SMTP_NEW_LINE "." SMTP_NEW_LINE ),
                    QString::fromLatin1( SMTP_NEW_LINE ".." SMTP_NEW_LINE ) );

        rcpt = to;
        state = Init;

        //socket->connectToHost (host, port);
        socket->connectToHostEncrypted (host, port);
        if (!socket->waitForConnected (timeout)) {
            um.addErr ( socket->errorString());
            break;
        }

        b_ret = true;
        break;
    }

    return b_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Smtp::sslErrors(QList<QSslError> err_lst)
{
    foreach(const QSslError & err, err_lst) {
        SIMPLESMTP_DEBUGM ("sslError: %d %s\n", (int)err.error (),
                 TMP_A(err.errorString ()));
    }
    if (ignore_ssl_errors_) {
        socket->ignoreSslErrors ();
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Smtp::stateChanged(QAbstractSocket::SocketState socketState)
{
    SIMPLESMTP_DEBUGM ("stateChanged: %d\n", (int)socketState);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Smtp::errorReceived (QAbstractSocket::SocketError socketError)
{
    UserMsg::err (
                tr("Sending mail failed due to following error (%1): %2")
                .arg (socketError)
                .arg (socket->errorString ()));
    SIMPLESMTP_DEBUGM ("smtp error: %d\n", (int)socketError);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Smtp::disconnected()
{
    qDebug() <<"disconneted";
    SIMPLESMTP_DEBUGM ("smtp disconnected; error: %s\n",
                       TMP_A(socket->errorString()));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Smtp::connected()
{
    SIMPLESMTP_DEBUGM ("Connected \n");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Smtp::readyRead()
{

    qDebug() <<"readyRead";
    // SMTP is line-oriented

    QString responseLine;
    do {
        responseLine = socket->readLine();
        response += responseLine;
    } while ( socket->canReadLine() && responseLine[3] != ' ' );

    responseLine.truncate( 3 );

    SIMPLESMTP_DEBUGM ("Server response code: %s\n", TMP_A(responseLine));
    SIMPLESMTP_DEBUGM ("Server response: %s\n", TMP_A(response));

    if ( state == Init && responseLine == "220" ) {
        // banner was okay, let's go on
        *t << "EHLO localhost" << SMTP_NEW_LINE;

        state = HandShake;

        // No need, because I'm using socket->startClienEncryption()
        // which makes the SSL handshake for you
        /*} else if (state == Tls && responseLine == "250") {
        // Trying AUTH
        SIMPLESMTP_DEBUGM ("STarting Tls";
        *t << "STARTTLS" << SMTP_NEW_LINE;
        state = HandShake;
    */
    } else if (state == HandShake && responseLine == "250") {
        socket->startClientEncryption();
        if(!socket->waitForEncrypted(timeout))
        {
            SIMPLESMTP_DEBUGM ("Wait for encrypted failed: %s\n",
                               TMP_A(socket->errorString()));
            state = Close;
        }

        //Send EHLO once again but now encrypted

        *t << "EHLO localhost" << SMTP_NEW_LINE;
        state = Auth;
    } else if (state == Auth && responseLine == "250") {
        // Trying AUTH
        SIMPLESMTP_DEBUGM ("SMTP Auth\n");
        *t << "AUTH LOGIN" << SMTP_NEW_LINE;
        state = User;
    } else if (state == User && responseLine == "334") {
        //Trying User
        SIMPLESMTP_DEBUGM ("SMTP Username\n");
        //GMAIL is using XOAUTH2 protocol, which basically means that password
        // and username has to be sent in base64 coding
        //https://developers.google.com/gmail/xoauth2_protocol
        *t << QByteArray().append(user).toBase64()  << SMTP_NEW_LINE;

        state = Pass;
    } else if (state == Pass && responseLine == "334") {
        //Trying pass
        SIMPLESMTP_DEBUGM ("SMTP Pass\n");
        *t << QByteArray().append(pass).toBase64() << SMTP_NEW_LINE;

        state = Mail;
    } else if ( state == Mail && responseLine == "235" ) {
        // HELO response was okay (well, it has to be)

        //Apperantly for Google it is mandatory to have MAIL FROM and RCPT
        //email formated the following way -> <email@gmail.com>
        SIMPLESMTP_DEBUGM ("SMTP MAIL FROM:<%s>\n", TMP_A(user));
        *t << "MAIL FROM:<" << user << ">" SMTP_NEW_LINE;
        state = Rcpt;
    } else if ( state == Rcpt && responseLine == "250" ) {
        //Apperantly for Google it is mandatory to have MAIL FROM and RCPT
        //email formated the following way -> <email@gmail.com>
        *t << "RCPT TO:<" << rcpt << ">" SMTP_NEW_LINE;
        state = Data;
    } else if ( state == Data && responseLine == "250" ) {

        *t << "DATA" SMTP_NEW_LINE;
        state = Body;
    } else if ( state == Body && responseLine == "354" ) {

        *t << message << SMTP_NEW_LINE "." SMTP_NEW_LINE;
        state = Quit;
    } else if ( state == Quit && responseLine == "250" ) {

        *t << "QUIT" SMTP_NEW_LINE;
        // here, we just close.
        state = Close;
        emit status( tr( "Message sent" ) );
    } else if ( state == Close ) {
        deleteLater();
        return;
    } else {
        // something broke.
        QMessageBox::warning( 0, tr( "Qt Simple SMTP client" ),
                              tr( "Unexpected reply from SMTP server:\n\n" ) + response );
        state = Close;
        emit status( tr( "Failed to send message" ) );
    }
    t->flush();
    response = "";
}
/* ========================================================================= */

/*  CLASS    =============================================================== */
//
//
//
//

/* ------------------------------------------------------------------------- */
/* ========================================================================= */
