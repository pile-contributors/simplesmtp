/**
 * @file simplesmtp.cc
 * @brief Definitions for SimpleSmtp class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "smtp.h"
#include "simplesmtp.h"
#include "simplesmtp-private.h"

/**
 * @class SimpleSmtp
 *
 *
 */

static Smtp * uniq_ = NULL;


bool SimpleSmtp::init (
        const QString &user, const QString &pass,
        const QString &host, int port, int timeout)
{
    SIMPLESMTP_TRACE_ENTRY;
    if (uniq_ != NULL) end ();
    uniq_ = new Smtp (user, pass, host, port, timeout);
    SIMPLESMTP_TRACE_EXIT;
    return true;
}

void SimpleSmtp::end ()
{
    SIMPLESMTP_TRACE_ENTRY;
    if (uniq_ != NULL) {
        //TODO problem for some reason on delete
        //uniq_->deleteLater ();
        uniq_ = NULL;
    }
    SIMPLESMTP_TRACE_EXIT;
}

bool SimpleSmtp::send_mail (
        UserMsg & um, const QString &to,
        const QString &subject, const QString &body,
        bool body_is_html, QStringList files)
{
    SIMPLESMTP_TRACE_ENTRY;
    if (uniq_ == NULL) {
        um.addErr(
                QObject::tr ("System not properly initialized for sending mails."));
        return false;
    }

    Smtp * sender = new Smtp (*uniq_);
    bool b_ret = sender->sendMail (
                um, to, subject, body, body_is_html, files);
    SIMPLESMTP_TRACE_EXIT;
    return b_ret;
}
