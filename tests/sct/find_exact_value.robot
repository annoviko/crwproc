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


Find and Change Int32
    ${subject pid}=     Get Pid        ${SUBJECT}
    Send Command        ${CRWPROC}     ${subject pid}

    ${value}=   Set Variable   0
    Set Subject Variable    i32    ${value}
    Create Exact Integral Filter   ${SIZE 4}   ${TYPE INT SIGNED}   ${value}

    ${value}=   Set Variable   1024
    Set Subject Variable    i32    ${value}
    Update Filter and Continue    ${value}

    ${value}=   Set Variable   32512
    Set Subject Variable    i32    ${value}
    Update Filter and Continue    ${value}

    ${value}=   Set Variable   128
    Set Value via CRWPROC   ${value}
    Check Subject Variable   i32   ${value}


Find and Change Int64
    ${subject pid}=     Get Pid        ${SUBJECT}
    Send Command        ${CRWPROC}     ${subject pid}

    ${value}=   Set Variable   0
    Set Subject Variable    i64    ${value}
    Create Exact Integral Filter   ${SIZE 8}   ${TYPE INT SIGNED}   ${value}

    ${value}=   Set Variable   128821
    Set Subject Variable    i64    ${value}
    Update Filter and Continue    ${value}

    ${value}=   Set Variable   325120945
    Set Subject Variable    i64    ${value}
    Update Filter and Continue    ${value}

    ${value}=   Set Variable   16
    Set Value via CRWPROC   ${value}
    Check Subject Variable   i64   ${value}


*** Keywords ***

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
    [Arguments]   ${type}    ${value}
    Send HTTP Request   POST           /operation/set/memory/stack/variable/${type}/value/${value}
    ${status}=          Get Response Status
    Should Be Equal     ${status}    ${202}

    Check Subject Variable   ${type}   ${value}
    Sleep   500ms


Check Subject Variable
    [Arguments]   ${type}   ${expected value}
    Send HTTP Request   GET           /memory/stack/variable/${type}
    
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