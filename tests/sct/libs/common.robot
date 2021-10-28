*** Settings ***

Library    Collections

Library    proc_interact.py


*** Variables ***
${BINARY FOLDER}    ./../../x64/Release/
${SUBJECT PORT}     51425

${MEM TYPE STACK}       stack
${MEM TYPE HEAP}        heap
${MEM TYPE GLOBAL}      global

@{INT TYPES}            i8   i16   i32   i64   u8   u16   u32   u64

${TYPE EXACT FILTER}    ${0}

${TYPE INT TYPE}        ${0}
${TYPE FLT TYPE}        ${2}
${TYPE DBL TYPE}        ${1}

${SIZE 1}    ${0}
${SIZE 2}    ${1}
${SIZE 4}    ${2}
${SIZE 8}    ${3}

${TYPE INT UNSIGNED}    ${0}
${TYPE INT SIGNED}      ${1}

${CRWPROC}          ${None}
${SUBJECT}          ${None}


*** Keywords ***

Get Variable Type By Type
    [Arguments]   ${var type}
    ${result}=    Set Variable   ${None}
    IF    "${var type}" == "i8"
        ${result}=   Set Variable   ${TYPE INT TYPE}
    ELSE IF    "${var type}" == "i16"
        ${result}=   Set Variable   ${TYPE INT TYPE}
    ELSE IF    "${var type}" == "i32"
        ${result}=   Set Variable   ${TYPE INT TYPE}
    ELSE IF    "${var type}" == "i64"
        ${result}=   Set Variable   ${TYPE INT TYPE}
    ELSE IF    "${var type}" == "u8"
        ${result}=   Set Variable   ${TYPE INT TYPE}
    ELSE IF    "${var type}" == "u16"
        ${result}=   Set Variable   ${TYPE INT TYPE}
    ELSE IF    "${var type}" == "u32"
        ${result}=   Set Variable   ${TYPE INT TYPE}
    ELSE IF    "${var type}" == "u64"
        ${result}=   Set Variable   ${TYPE INT TYPE}
    ELSE IF    "${var type}" == "flt"
        ${result}=   Set Variable   ${TYPE FLT TYPE}
    ELSE IF    "${var type}" == "dbl"
        ${result}=   Set Variable   ${TYPE DBL TYPE}
    END
    [Return]   ${result}

Get Variable Signed Type By Type
    [Arguments]   ${var type}
    ${result}=    Set Variable   ${None}
    IF    "${var type}" == "i8"
        ${result}=   Set Variable   ${TYPE INT SIGNED}
    ELSE IF    "${var type}" == "i16"
        ${result}=   Set Variable   ${TYPE INT SIGNED}
    ELSE IF    "${var type}" == "i32"
        ${result}=   Set Variable   ${TYPE INT SIGNED}
    ELSE IF    "${var type}" == "i64"
        ${result}=   Set Variable   ${TYPE INT SIGNED}
    ELSE IF    "${var type}" == "u8"
        ${result}=   Set Variable   ${TYPE INT UNSIGNED}
    ELSE IF    "${var type}" == "u16"
        ${result}=   Set Variable   ${TYPE INT UNSIGNED}
    ELSE IF    "${var type}" == "u32"
        ${result}=   Set Variable   ${TYPE INT UNSIGNED}
    ELSE IF    "${var type}" == "u64"
        ${result}=   Set Variable   ${TYPE INT UNSIGNED}
    END
    [Return]   ${result}


Get Variable Size By Type
    [Arguments]   ${var type}
    ${result}=    Set Variable   ${None}
    IF    "${var type}" == "i8"
        ${result}=   Set Variable   ${SIZE 1}
    ELSE IF    "${var type}" == "i16"
        ${result}=   Set Variable   ${SIZE 2}
    ELSE IF    "${var type}" == "i32"
        ${result}=   Set Variable   ${SIZE 4}
    ELSE IF    "${var type}" == "i64"
        ${result}=   Set Variable   ${SIZE 8}
    ELSE IF    "${var type}" == "u8"
        ${result}=   Set Variable   ${SIZE 1}
    ELSE IF    "${var type}" == "u16"
        ${result}=   Set Variable   ${SIZE 2}
    ELSE IF    "${var type}" == "u32"
        ${result}=   Set Variable   ${SIZE 4}
    ELSE IF    "${var type}" == "u64"
        ${result}=   Set Variable   ${SIZE 8}
    ELSE IF    "${var type}" == "flt"
        ${result}=   Set Variable   ${SIZE 4}
    ELSE IF    "${var type}" == "dbl"
        ${result}=   Set Variable   ${SIZE 8}
    END
    [Return]   ${result}


Create Exact Filter
    [Arguments]   ${var type}   ${value}
    ${var size}=   Get Variable Size By Type          ${var type}
    ${var sign}=   Get Variable Signed Type by Type   ${var type}

    Create Exact Integral Filter   ${var size}   ${var sign}   ${value}


Create Exact Integral Filter
    [Arguments]   ${value size}   ${type int}   ${value}
    Send Command   ${CRWPROC}   ${TYPE EXACT FILTER}
    Send Command   ${CRWPROC}   ${TYPE INT TYPE}
    Send Command   ${CRWPROC}   ${value size}
    Send Command   ${CRWPROC}   ${type int}
    Send Command   ${CRWPROC}   ${value}


Connect to Subject Process
    ${subject pid}=     Get Pid        ${SUBJECT}
    Send Command        ${CRWPROC}     ${subject pid}


Update Filter and Continue
    [Arguments]   ${value}
    Send Command   ${CRWPROC}   \\update_filter
    Send Command   ${CRWPROC}   ${value}


Edit Value via CRWPROC
    [Arguments]   ${index}   ${value}
    Send Command   ${CRWPROC}   \\edit
    Send Command   ${CRWPROC}   \\set ${index} ${value}


Set Value via CRWPROC
    [Arguments]   ${value}
    Send Command   ${CRWPROC}   \\show
    Send Command   ${CRWPROC}   \\add 0
    Edit Value via CRWPROC      0   ${value}


Set Subject Variable
    [Arguments]   ${mem type}   ${var type}    ${value}
    Send HTTP Request   POST           /operation/set/memory/${mem type}/variable/${var type}/value/${value}
    ${status}=          Get Response Status
    Should Be Equal     ${status}    ${202}

    Check Subject Variable   ${mem type}   ${var type}   ${value}


Get Subject Variable Address
    [Arguments]   ${mem type}   ${var type}
    Send HTTP Request   GET           /address/memory/${mem type}/variable/${var type}

    ${status}=          Get Response Status
    Should Be Equal     ${status}    ${200}
    ${address}=         Get Response Body

    [Return]    ${address}


Check Subject Variable
    [Arguments]   ${mem type}   ${var type}   ${expected value}
    Send HTTP Request   GET           /memory/${mem type}/variable/${var type}
    
    ${status}=          Get Response Status
    Should Be Equal     ${status}    ${200}

    ${body}=            Get Response Body

    ${count}=   Count Values In List   ${INT TYPES}   ${var type}
    IF   "${count}" > "${0}"
        ${actual value}=     Convert To Integer   ${body}
        ${expected value}=   Convert To Integer   ${expected value}
    ELSE
        ${actual value}=     Convert To Number    ${body}
        ${expected value}=   Convert To Number    ${expected value}
    END
    Should Be Equal     ${actual value}    ${expected value}


Initialize Object and Subject
    ${subject instance}=    Run Subject     ${BINARY FOLDER}
    ${crwproc instance}=    Crwproc Run     ${BINARY FOLDER}

    Set Suite Variable    ${SUBJECT}    ${subject instance}
    Set Suite Variable    ${CRWPROC}    ${crwproc instance}

    Initialize Client   127.0.0.1      ${SUBJECT PORT}


Terminate Object and Subject
    Crwproc Press Enter Key    ${CRWPROC}
    Crwproc Exit        ${CRWPROC}

    Kill Application    ${SUBJECT}
    Kill Application    ${CRWPROC}