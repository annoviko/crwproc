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
    Connect to Subject Process
    Add Variable by Address   ${MEM TYPE STACK}   i8    64
    Navigate to Edit Table

    Send Command   ${CRWPROC}   \\rename 0 cash
    
    Wait For Output Stream Should Contain   .* 0 .* cash .* 64 .*


Rename Two Added Variables
    Connect to Subject Process
    Add Variable by Address   ${MEM TYPE STACK}   i8    111
    Add Variable by Address   ${MEM TYPE STACK}   i16   22222

    Navigate to Edit Table

    Send Command   ${CRWPROC}   \\rename 0 wood
    Send Command   ${CRWPROC}   \\rename 1 gold

    Wait For Output Stream Should Contain   .* 0 .* wood .* 111 .*
    Wait For Output Stream Should Contain   .* 1 .* gold .* 22222 .*


Rename And Set By Name
    Connect to Subject Process
    Add Variable by Address and Rename   ${MEM TYPE STACK}   i8    64   0   adm
    
    Send Command   ${CRWPROC}   \\set adm 101

    Check Subject Variable   ${MEM TYPE STACK}   i8   101
    Wait For Output Stream Should Contain   .* 0 .* adm .* 101 .*


Rename And Delete By Name
    Connect to Subject Process
    Add Variable by Address and Rename   ${MEM TYPE STACK}   i8    64   0   dip

    Send Command   ${CRWPROC}   \\remove dip

    Wait For Output Stream Should Not Contain   .* 0 .* 101 .*


Rename And Rename By Name
    Connect to Subject Process
    Add Variable by Address and Rename   ${MEM TYPE STACK}   i8    64   0   mil

    Send Command   ${CRWPROC}   \\rename mil cash

    Wait For Output Stream Should Contain   .* 0 .* cash .* 64 .*


Rename Unique Variable
    Connect to Subject Process
    Add Variable by Address and Rename   ${MEM TYPE STACK}   i8     16   0   coal
    Add Variable by Address and Rename   ${MEM TYPE STACK}   i16    32   1   iron

    Add Variable by Address   ${MEM TYPE STACK}   i32    64
    Navigate to Edit Table
    Send Command   ${CRWPROC}   \\rename 2 coal

    ${result}=    Output Stream Contains    ${CRWPROC}    .*Error: variable name should be unique.*
    Should Be True    ${result}

    ${result}=   Output Stream Contains    ${CRWPROC}    .*Press any key to continue.*
    Should Be True    ${result}

    Crwproc Press Any Key   ${CRWPROC}


Rename To Old Name
    Connect to Subject Process
	Add Variable by Address and Rename   ${MEM TYPE STACK}   i8     16   0   wood
	
	Send Command   ${CRWPROC}   \\rename 0 food
	Wait For Output Stream Should Contain   .* 0 .* food .* 16 .*

	Send Command   ${CRWPROC}   \\rename 0 wood
	Wait For Output Stream Should Contain   .* 0 .* wood .* 16 .*
	
	Send Command   ${CRWPROC}   \\rename wood food
	Wait For Output Stream Should Contain   .* 0 .* food .* 16 .*


*** Keywords ***

Add Variable by Address
    [Arguments]   ${mem type}   ${var type}   ${inital value}
    ${address}=    Get Subject Variable Address       ${mem type}   ${var type}
    ${address}=    Convert To Upper Case              ${address}

    Find Value by Address and Check Output   ${inital value}   ${address}   ${mem type}   ${var type}
    
    Send Command   ${CRWPROC}   \\add


Add Variable by Address and Rename
    [Arguments]   ${mem type}   ${var type}   ${inital value}   ${var index}   ${var name}
    Add Variable by Address   ${mem type}   ${var type}    ${inital value}
    Navigate to Edit Table

    Send Command   ${CRWPROC}   \\rename ${var index} ${var name}
    
    Wait For Output Stream Should Contain   .* ${var index} .* ${var name} .* ${inital value} .*
