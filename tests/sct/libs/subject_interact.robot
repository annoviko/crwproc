*** Settings ***

Library    proc_interact.py


*** Keywords ***

Run Subject
    [Arguments]     ${path}
    ${app path}=    Set Variable       ${path}/basic_subject.exe
    ${subject}=     Run Application    ${app path}
    [Return]        ${subject}


Stop Subject
    [Arguments]     ${subject}
    IF    ${subject} == ${None}
    Kill Application    ${subject}
    END