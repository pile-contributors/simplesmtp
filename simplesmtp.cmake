
# enable/disable cmake debug messages related to this pile
set (SIMPLESMTP_DEBUG_MSG ON)

# make sure support code is present; no harm
# in including it twice; the user, however, should have used
# pileInclude() from pile_support.cmake module.
include(pile_support)

# initialize this module
macro    (simplesmtpInit
          ref_cnt_use_mode)

    # default name
    if (NOT SIMPLESMTP_INIT_NAME)
        set(SIMPLESMTP_INIT_NAME "SimpleSmtp")
    endif ()

    # compose the list of headers and sources
    set(SIMPLESMTP_HEADERS
        "simplesmtp.h")
    set(SIMPLESMTP_SOURCES
        "simplesmtp.cc")

    pileSetSources(
        "${SIMPLESMTP_INIT_NAME}"
        "${SIMPLESMTP_HEADERS}"
        "${SIMPLESMTP_SOURCES}")

    pileSetCommon(
        "${SIMPLESMTP_INIT_NAME}"
        "0;0;1;d"
        "ON"
        "${ref_cnt_use_mode}"
        ""
        "category1"
        "tag1;tag2")

endmacro ()
