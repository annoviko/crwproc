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

${INT TYPE NAME}        integral
${FLT TYPE NAME}        floating
${DBL TYPE NAME}        doubling

${SIZE 1}    ${0}
${SIZE 2}    ${1}
${SIZE 4}    ${2}
${SIZE 8}    ${3}

${TYPE INT UNSIGNED}    ${0}
${TYPE INT SIGNED}      ${1}

${CRWPROC}          ${None}
${SUBJECT}          ${None}


*** Keywords ***

Is Variable Signed By Type
    [Arguments]   ${var type}
    ${result}=    Set Variable   ${None}
    IF    "${var type}" == "i8"
        ${result}=   Set Variable   ${True}
    ELSE IF    "${var type}" == "i16"
        ${result}=   Set Variable   ${True}
    ELSE IF    "${var type}" == "i32"
        ${result}=   Set Variable   ${True}
    ELSE IF    "${var type}" == "i64"
        ${result}=   Set Variable   ${True}
    ELSE IF    "${var type}" == "u8"
        ${result}=   Set Variable   ${False}
    ELSE IF    "${var type}" == "u16"
        ${result}=   Set Variable   ${False}
    ELSE IF    "${var type}" == "u32"
        ${result}=   Set Variable   ${False}
    ELSE IF    "${var type}" == "u64"
        ${result}=   Set Variable   ${False}
    ELSE IF    "${var type}" == "flt"
        ${result}=   Set Variable   ${True}
    ELSE IF    "${var type}" == "dbl"
        ${result}=   Set Variable   ${True}
    END
    [Return]   ${result}


Get Variable Size By Type
    [Arguments]   ${var type}
    ${result}=    Set Variable   ${None}
    IF    "${var type}" == "i8"
        ${result}=   Set Variable   ${1}
    ELSE IF    "${var type}" == "i16"
        ${result}=   Set Variable   ${2}
    ELSE IF    "${var type}" == "i32"
        ${result}=   Set Variable   ${4}
    ELSE IF    "${var type}" == "i64"
        ${result}=   Set Variable   ${8}
    ELSE IF    "${var type}" == "u8"
        ${result}=   Set Variable   ${1}
    ELSE IF    "${var type}" == "u16"
        ${result}=   Set Variable   ${2}
    ELSE IF    "${var type}" == "u32"
        ${result}=   Set Variable   ${4}
    ELSE IF    "${var type}" == "u64"
        ${result}=   Set Variable   ${8}
    ELSE IF    "${var type}" == "flt"
        ${result}=   Set Variable   ${4}
    ELSE IF    "${var type}" == "dbl"
        ${result}=   Set Variable   ${8}
    END
    [Return]   ${result}


Get Typename By Type
    [Arguments]   ${var type}
    ${result}=    Set Variable   ${None}

    ${count}=     Count Values In List   ${INT TYPES}   ${var type}
    IF   "${count}" > "${0}"
        ${result}=    Set Variable   integral
    ELSE
        IF    "${var type}" == "flt"
            ${result}=   Set Variable   floating
        ELSE IF    "${var type}" == "dbl"
            ${result}=   Set Variable   doubling
        END
    END
    [Return]   ${result}


Get Option Variable Type By Type
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


Get Option Variable Signed Type By Type
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


Get Option Variable Size By Type
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
    ${val type}=   Get Option Variable Type By Type          ${var type}
    ${val size}=   Get Option Variable Size By Type          ${var type}
    ${val sign}=   Get Option Variable Signed Type By Type   ${var type}

    ${count}=     Count Values In List   ${INT TYPES}   ${var type}
    IF   "${count}" > "${0}"
        Create Exact Integral Filter   ${val size}   ${val sign}   ${value}
    ELSE
        Create Exact Float or Double Filter   ${val type}   ${value}
    END

    Wait For Output Stream Content   ${CRWPROC}   .*Processing time:.*


Create Exact Float or Double Filter
    [Arguments]   ${val type}   ${value}
    Send Command   ${CRWPROC}   ${TYPE EXACT FILTER}
    Send Command   ${CRWPROC}   ${val type}
    Send Command   ${CRWPROC}   ${value}


Create Exact Integral Filter
    [Arguments]   ${value size}   ${type int}   ${value}
    Send Command   ${CRWPROC}   ${TYPE EXACT FILTER}
    Send Command   ${CRWPROC}   ${TYPE INT TYPE}
    Send Command   ${CRWPROC}   ${value size}
    Send Command   ${CRWPROC}   ${type int}
    Send Command   ${CRWPROC}   ${value}


Connect to Subject Process
    ${subject pid}=     Get Pid        ${SUBJECT}
    Wait For Output Stream Content   ${CRWPROC}   .*Please, enter PID or crwproc command.*
    Send Command        ${CRWPROC}     ${subject pid}
    Wait For Output Stream Content   ${CRWPROC}   .*Create filter for the process.*


Update Filter and Continue
    [Arguments]   ${value}
    Send Command   ${CRWPROC}   \\update_filter
    Send Command   ${CRWPROC}   ${value}

    Wait For Output Stream Content   ${CRWPROC}   .*Processing time:.*


Edit Value via CRWPROC
    [Arguments]   ${index}   ${value}
    Send Command   ${CRWPROC}   \\edit
    Send Command   ${CRWPROC}   \\set ${index} ${value}


Set Value via CRWPROC
    [Arguments]   ${index}   ${value}
    Send Command   ${CRWPROC}   \\show
    Send Command   ${CRWPROC}   \\add ${index}
    Edit Value via CRWPROC      ${index}   ${value}


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


Find Value by Address
    [Arguments]   ${var inital value}   ${address}   ${mem type}   ${var type}
    Set Subject Variable   ${mem type}   ${var type}   ${var inital value}

    ${val size}=   Get Option Variable Size By Type          ${var type}
    ${val sign}=   Get Option Variable Signed Type By Type   ${var type}
    ${val type}=   Get Option Variable Type By Type          ${var type}

    Send Command    ${CRWPROC}   \\address
    Send Command    ${CRWPROC}   ${val type}

    IF    "${val type}" == "${TYPE INT TYPE}"
        Send Command    ${CRWPROC}   ${val size}
        Send Command    ${CRWPROC}   ${val sign}
    END

    Send Command    ${CRWPROC}   ${address}


Find Value by Address and Check Output
    [Arguments]   ${var inital value}   ${address}   ${mem type}   ${var type}

    Clean Output Stream      ${CRWPROC}
    Find Value by Address    ${var inital value}   ${address}   ${mem type}   ${var type}
    
    ${pattern}=   Set Variable     .*${address}.*${var inital value}.*
    ${result}=    Output Stream Contains    ${CRWPROC}    ${pattern}
    Should Be True    ${result}


Output Stream Should Contain
    [Arguments]   ${pattern}
    ${result}=    Output Stream Contains    ${CRWPROC}    ${pattern}
    Should Be True    ${result}


Wait For Output Stream Should Contain
    [Arguments]   ${pattern}   ${timeout}=${1}
    ${result}=    Wait For Output Stream Content    ${CRWPROC}   ${pattern}   ${timeout}
    Should Be True    ${result}


Output Stream Should Not Contain
    [Arguments]   ${pattern}
    ${result}=    Output Stream Contains    ${CRWPROC}    ${pattern}
    Should Not Be True    ${result}


Wait For Output Stream Should Not Contain
    [Arguments]   ${pattern}   ${timeout}=${1}
    ${result}=    Wait For Output Stream Content    ${CRWPROC}   ${pattern}   ${timeout}
    Should Not Be True    ${result}


Find Value by Address and Add It
    [Arguments]   ${var inital value}   ${address}   ${mem type}   ${var type}
    Find Value by Address and Check Output   ${var inital value}   ${address}   ${mem type}   ${var type}
    Send Command   ${CRWPROC}   \\add


Get Index From View Table By Address
    [Arguments]   ${address}
    Navigate to Show View

    ${address uppercase}=    Convert To Upper Case    ${address}
    ${pattern}=    Set Variable           .* (\\d+) .*${address uppercase}.*

    ${groups}=     Output Stream Match    ${CRWPROC}    ${pattern}

    Should Not Be Empty    ${groups}
    ${index}=              Set Variable       ${groups}[0]
    [Return]   ${index}


Send HTTP Request with Check and Get Body
    [Arguments]   ${method}   ${uri}   ${expected code}   
    Send HTTP Request   ${method}   ${uri}

    ${status}=          Get Response Status
    Should Be Equal     ${status}   ${expected code}

    ${body}=            Get Response Body
    [Return]   ${body}


Navigate to Show View
    Clean Output Stream    ${CRWPROC}
    Send Command           ${CRWPROC}    \\show
    Wait For Output Stream Content   ${CRWPROC}   .*Found values with the filter.*


Navigate to Edit Table
    Clean Output Stream    ${CRWPROC}
    Send Command           ${CRWPROC}    \\show
    Wait For Output Stream Content   ${CRWPROC}   .*User table to edit values.*


Initialize Crwproc
    ${crwproc instance}=    Crwproc Run     ${BINARY FOLDER}    ${TEST NAME}
    Set Suite Variable      ${CRWPROC}      ${crwproc instance}


Initialize Subject
    ${subject instance}=    Run Subject     ${BINARY FOLDER}
    Set Suite Variable      ${SUBJECT}      ${subject instance}


Initialize Object and Subject
    Initialize Crwproc
    Initialize Subject
    Initialize Client   127.0.0.1      ${SUBJECT PORT}
    Sleep   10ms


Terminate Crwproc
    Crwproc Press Enter Key     ${CRWPROC}
    Crwproc Exit                ${CRWPROC}
    Kill Application            ${CRWPROC}


Terminate Object and Subject
    Terminate Crwproc
    Kill Application    ${SUBJECT}
