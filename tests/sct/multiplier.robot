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

Set Multiplier 1000
    Connect to Subject Process

    Add Variable by Address   ${MEM TYPE STACK}   i32    10000
    Navigate to Edit Table
    Send Command   ${CRWPROC}   \\set_multiplier 0 1000

    Wait For Output Stream Should Contain   .* 0 .* 1000 .* 10 .*

    Send Command   ${CRWPROC}   \\set 0 64

    Wait For Output Stream Should Contain   .* 0 .* 1000 .* 64 .*


Set Multiplier 1000 For One Variable
    Connect to Subject Process

    Add Variable by Address   ${MEM TYPE STACK}   i32    100000
    Add Variable by Address   ${MEM TYPE STACK}   u32    64

    Navigate to Edit Table
    Send Command   ${CRWPROC}   \\set_multiplier 0 1000

    Wait For Output Stream Should Contain   .* 0 .* 1000 .* 100 .*
    Wait For Output Stream Should Contain   .* 1 .* 64 .*


*** Keywords ***

Add Variable by Address
    [Arguments]   ${mem type}   ${var type}   ${inital value}
    ${address}=    Get Subject Variable Address       ${mem type}   ${var type}
    ${address}=    Convert To Upper Case              ${address}

    Find Value by Address and Check Output   ${inital value}   ${address}   ${mem type}   ${var type}
    
    Send Command   ${CRWPROC}   \\add
