if (NOT SIMPLESMTP_BUILD_MODE)
    set (SIMPLESMTP_BUILD_MODE STATIC)
endif ()

include(pile_support)
pileInclude (SimpleSmtp)
simplesmtpInit(${SIMPLESMTP_BUILD_MODE})
