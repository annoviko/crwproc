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

Change Stack Int8 By Address
    Test Template Address   ${MEM TYPE STACK}   i8   100   -5


Change Stack Int16 By Address
    Test Template Address   ${MEM TYPE STACK}   i16   1600   -3200


Change Stack Int32 By Address
    Test Template Address   ${MEM TYPE STACK}   i32   128000   256000


Change Stack Int64 By Address
    Test Template Address   ${MEM TYPE STACK}   i64   512000   -16


Change Heap Uint8 By Address
    Test Template Address   ${MEM TYPE HEAP}   u8   64   128


Change Global Uint8 By Address
    Test Template Address   ${MEM TYPE GLOBAL}   u8   16   32


Change Stack Float By Address
    Test Template Address   ${MEM TYPE STACK}   flt   3.14   1.2345


Change Heap Float By Address
    Test Template Address   ${MEM TYPE HEAP}   flt   1.23   10.567


Change Global Float By Address
    Test Template Address   ${MEM TYPE GLOBAL}   flt   7.98   -4.56


Change Stack Double By Address
    Test Template Address   ${MEM TYPE STACK}   dbl   23.144   12.23456


Change Heap Double By Address
    Test Template Address   ${MEM TYPE HEAP}   dbl   14.73   -10.567


Change Global Double By Address
    Test Template Address   ${MEM TYPE GLOBAL}   dbl   76.938   -14.526


*** Keywords ***

Test Template Address
    [Arguments]   ${mem type}   ${var type}   ${inital value}    ${final value}
    ${val size}=   Get Option Variable Size By Type          ${var type}
    ${val sign}=   Get Option Variable Signed Type by Type   ${var type}

    ${address}=    Get Subject Variable Address       ${mem type}   ${var type}
    ${address}=    Convert To Upper Case              ${address}

    Connect to Subject Process
    Find Value by Address and Check Output   ${inital value}   ${address}   ${mem type}   ${var type}
    
    Send Command   ${CRWPROC}   \\add

    Edit Value via CRWPROC   0   ${final value}
    Check Subject Variable   ${mem type}   ${var type}   ${final value}
