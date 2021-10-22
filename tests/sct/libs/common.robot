*** Settings ***

Library    proc_interact.py


*** Variables ***
${BINARY FOLDER}    ./../../x64/Release/
${SUBJECT PORT}     51425

${MEM TYPE STACK}       stack
${MEM TYPE HEAP}        heap
${MEM TYPE GLOBAL}      global

${TYPE EXACT FILTER}    ${0}

${TYPE INT TYPE}        ${0}
${TYPE FLT TYPE}        ${1}
${TYPE DBL TYPE}        ${2}

${SIZE 1}    ${0}
${SIZE 2}    ${1}
${SIZE 4}    ${2}
${SIZE 8}    ${3}

${TYPE INT UNSIGNED}    ${0}
${TYPE INT SIGNED}      ${1}

${CRWPROC}          ${None}
${SUBJECT}          ${None}


*** Keywords ***

Create Exact Filter
    [Arguments]   ${var type}   ${value}
    IF    "${var type}" == "i8"
        Create Exact Integral Filter   ${SIZE 1}   ${TYPE INT SIGNED}   ${value}
    ELSE IF    "${var type}" == "i16"
        Create Exact Integral Filter   ${SIZE 2}   ${TYPE INT SIGNED}   ${value}
    ELSE IF    "${var type}" == "i32"
        Create Exact Integral Filter   ${SIZE 4}   ${TYPE INT SIGNED}   ${value}
    ELSE IF    "${var type}" == "i64"
        Create Exact Integral Filter   ${SIZE 8}   ${TYPE INT SIGNED}   ${value}
    ELSE IF    "${var type}" == "u8"
        Create Exact Integral Filter   ${SIZE 1}   ${TYPE INT UNSIGNED}   ${value}
    ELSE IF    "${var type}" == "u16"
        Create Exact Integral Filter   ${SIZE 2}   ${TYPE INT UNSIGNED}   ${value}
    ELSE IF    "${var type}" == "u32"
        Create Exact Integral Filter   ${SIZE 4}   ${TYPE INT UNSIGNED}   ${value}
    ELSE IF    "${var type}" == "u64"
        Create Exact Integral Filter   ${SIZE 8}   ${TYPE INT UNSIGNED}   ${value}
    END


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


Set Value via CRWPROC
    [Arguments]   ${value}
    Send Command   ${CRWPROC}   \\show
    Send Command   ${CRWPROC}   \\add 0
    Send Command   ${CRWPROC}   \\edit
    Send Command   ${CRWPROC}   \\set 0 ${value}


Set Subject Variable
    [Arguments]   ${mem type}   ${var type}    ${value}
    Send HTTP Request   POST           /operation/set/memory/${mem type}/variable/${var type}/value/${value}
    ${status}=          Get Response Status
    Should Be Equal     ${status}    ${202}

    Check Subject Variable   ${mem type}   ${var type}   ${value}


Check Subject Variable
    [Arguments]   ${mem type}   ${var type}   ${expected value}
    Send HTTP Request   GET           /memory/${mem type}/variable/${var type}
    
    ${status}=          Get Response Status
    Should Be Equal     ${status}    ${200}

    ${body}=            Get Response Body
    Should Be Equal     ${body}    ${expected value}


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