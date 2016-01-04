/**
 * @file simplesmtp.h
 * @brief Declarations for SimpleSmtp class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_SIMPLESMTP_H_INCLUDE
#define GUARD_SIMPLESMTP_H_INCLUDE

#include <simplesmtp/simplesmtp-config.h>
#include <usermsg/usermsg.h>

#include <QStringList>

//! brief description
class SIMPLESMTP_EXPORT SimpleSmtp {

public:

    bool init (
            const QString &user,
            const QString &pass,
            const QString &host,
            int port,
            int timeout);

    void end ();

    bool send_mail (
            UserMsg & um,
            const QString &to,
            const QString &subject,
            const QString &body,
            bool body_is_html,
            const QStringList & files = QStringList());
};

#endif // GUARD_SIMPLESMTP_H_INCLUDE
