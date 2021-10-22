*** Settings ***

Library     HttpCtrl.Client
Library     libs/crwproc_interact.py
Library     libs/proc_interact.py

Resource    libs/subject_interact.robot

Test Setup       Initialize Object and Subject
Test Teardown    Terminate Object and Subject


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


*** Test Cases ***

Connect to Subject Process
    ${subject pid}=     Get Pid        ${subject}
    Send Command        ${CRWPROC}     ${subject pid}


Find and Change Stack Int8
    ${value evolution}=    Create List   0   -16   32
    Test Template Find and Change   ${MEM TYPE STACK}   i8   ${value evolution}   -8


Find and Change Stack Int16
    ${value evolution}=    Create List   0   -2048   4096
    Test Template Find and Change   ${MEM TYPE STACK}   i16   ${value evolution}   1024


Find and Change Stack Int32
    ${value evolution}=    Create List   0   -4194304   8388608
    Test Template Find and Change   ${MEM TYPE STACK}   i32   ${value evolution}   -16777216


Find and Change Stack Int64
    ${value evolution}=    Create List   0   -8589934592   8589934592
    Test Template Find and Change   ${MEM TYPE STACK}   i64   ${value evolution}   16


Find and Change Stack Uint8
    ${value evolution}=    Create List   0   16   128
    Test Template Find and Change   ${MEM TYPE STACK}   u8   ${value evolution}   8


Find and Change Stack Uin16
    ${value evolution}=    Create List   0   2048   32769
    Test Template Find and Change   ${MEM TYPE STACK}   u16   ${value evolution}   1024


Find and Change Stack Uint32
    ${value evolution}=    Create List   0   4194304   2147483649
    Test Template Find and Change   ${MEM TYPE STACK}   u32   ${value evolution}   16777216


Find and Change Stack Uint64
    ${value evolution}=    Create List   0   8589934592   9223372036854775808
    Test Template Find and Change   ${MEM TYPE STACK}   u64   ${value evolution}   128


Find and Change Heap Int8
    ${value evolution}=    Create List   0   -16   32
    Test Template Find and Change   ${MEM TYPE HEAP}   i8   ${value evolution}   -8


Find and Change Heap Int16
    ${value evolution}=    Create List   0   -2048   4096
    Test Template Find and Change   ${MEM TYPE HEAP}   i16   ${value evolution}   1024


Find and Change Heap Int32
    ${value evolution}=    Create List   0   -4194304   8388608
    Test Template Find and Change   ${MEM TYPE HEAP}   i32   ${value evolution}   -16777216


Find and Change Heap Int64
    ${value evolution}=    Create List   0   -8589934592   8589934592
    Test Template Find and Change   ${MEM TYPE HEAP}   i64   ${value evolution}   16


Find and Change Global Int8
    ${value evolution}=    Create List   0   -16   32
    Test Template Find and Change   ${MEM TYPE GLOBAL}   i8   ${value evolution}   -8


Find and Change Global Int16
    ${value evolution}=    Create List   0   -2048   4096
    Test Template Find and Change   ${MEM TYPE GLOBAL}   i16   ${value evolution}   1024


Find and Change Global Int32
    ${value evolution}=    Create List   0   -4194304   8388608
    Test Template Find and Change   ${MEM TYPE GLOBAL}   i32   ${value evolution}   -16777216


Find and Change Global Int64
    ${value evolution}=    Create List   0   -8589934592   8589934592
    Test Template Find and Change   ${MEM TYPE GLOBAL}   i64   ${value evolution}   16


Filter Value Int8 is Out of Range
    Test Template Filter Value is Out of Range   i8   -129


Filter Value Uint8 is Out of Range
    Test Template Filter Value is Out of Range   u8   256


Filter Value Int8 is Lower Border
    Test Template Filter Value on a Range Border   i8   -128


Filter Value Uint8 is Upper Border
    Test Template Filter Value on a Range Border   u8   255



*** Keywords ***

Test Template Filter Value is Out of Range
    [Arguments]   ${var type}   ${value}
    ${subject pid}=     Get Pid        ${SUBJECT}
    Send Command        ${CRWPROC}     ${subject pid}

    Clean Output Stream    ${CRWPROC}
    Create Exact Filter    ${var type}   ${value}
    ${result}=   Output Stream Contains    ${CRWPROC}    .*Error: specified value '.*' is out of range.*
    Should Be True   ${result}


Test Template Filter Value on a Range Border
    [Arguments]   ${var type}   ${value}
    ${subject pid}=     Get Pid        ${SUBJECT}
    Send Command        ${CRWPROC}     ${subject pid}

    Clean Output Stream    ${CRWPROC}
    Create Exact Filter    u8   255
    ${result}=   Output Stream Contains    ${CRWPROC}    .*Error: specified value '.*' is out of range.*
    Should Be Equal    ${FALSE}    ${result}


Test Template Find and Change
    [Arguments]   ${mem type}   ${var type}   ${value evolution}   ${value to set}
    ${subject pid}=     Get Pid        ${SUBJECT}
    Send Command        ${CRWPROC}     ${subject pid}

    ${value}=   Set Variable   ${value evolution[0]}
    Set Subject Variable    ${mem type}    ${var type}    ${value}
    Create Exact Filter     ${var type}    ${value}

    ${length}=    Get length    ${value evolution}
    FOR    ${index}    IN RANGE    1    ${length}
        ${value}=   Set Variable   ${value evolution[${index}]}
        Set Subject Variable          ${mem type}    ${var type}    ${value}
        Update Filter and Continue    ${value}
    END

    Set Value via CRWPROC    ${value to set}
    Check Subject Variable   ${mem type}    ${var type}   ${value to set}


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