*** Settings ***

Library          String
Library          HttpCtrl.Client
Library          libs/crwproc_interact.py
Library          libs/proc_interact.py

Resource         libs/common.robot
Resource         libs/subject_interact.robot

Test Setup       Initialize Object and Subject
Test Teardown    Terminate Object and Subject



*** Test Cases ***

Rename Single Variable
    Add Variable by Address   ${MEM TYPE STACK}   i8    64
    Navigate to Edit Table

    Send Command   ${CRWPROC}   \\rename 0 cash
    
    Wait For Output Stream Should Contain   .* 0 .* cash .* 64 .*


Rename Two Added Variables
    Add Variable by Address   ${MEM TYPE STACK}   i8    111
    Add Variable by Address   ${MEM TYPE STACK}   i16   22222

    Navigate to Edit Table

    Send Command   ${CRWPROC}   \\rename 0 wood
    Send Command   ${CRWPROC}   \\rename 1 gold

    Wait For Output Stream Should Contain   .* 0 .* wood .* 111 .*
    Wait For Output Stream Should Contain   .* 1 .* gold .* 22222 .*


*** Keywords ***

Add Variable by Address
    [Arguments]   ${mem type}   ${var type}   ${inital value}
    ${address}=    Get Subject Variable Address       ${mem type}   ${var type}
    ${address}=    Convert To Upper Case              ${address}

    Connect to Subject Process
    Find Value by Address and Check Output   ${inital value}   ${address}   ${mem type}   ${var type}
    
    Send Command   ${CRWPROC}   \\add