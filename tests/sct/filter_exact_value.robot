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


Find and Change Stack Float
    ${value evolution}=    Create List   0   1.5   -3.5
    Test Template Find and Change   ${MEM TYPE STACK}   flt   ${value evolution}   4.5


Find and Change Stack Double
    ${value evolution}=    Create List   0   4.95   -0.55
    Test Template Find and Change   ${MEM TYPE STACK}   dbl   ${value evolution}   -1.5


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


Find and Change Heap Float
    ${value evolution}=    Create List   0   1.5   -3.5
    Test Template Find and Change   ${MEM TYPE HEAP}   flt   ${value evolution}   4.5


Find and Change Heap Double
    ${value evolution}=    Create List   0   4.95   -0.55
    Test Template Find and Change   ${MEM TYPE HEAP}   dbl   ${value evolution}   -1.5


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


Find and Change Global Float
    ${value evolution}=    Create List   0   1.5   -3.5
    Test Template Find and Change   ${MEM TYPE GLOBAL}   flt   ${value evolution}   4.5


Find and Change Global Double
    ${value evolution}=    Create List   0   4.95   -0.55
    Test Template Find and Change   ${MEM TYPE GLOBAL}   dbl   ${value evolution}   -1.5


Find and Change Heap Int8 and Int16
    ${value evolution}=    Create List   0   -4   8   -16
    Test Template Find and Change among Two   ${MEM TYPE HEAP}   i8   ${MEM TYPE HEAP}   i16   ${value evolution}   32


Find and Change Global Uint8 and Uint16
    ${value evolution}=    Create List   0   8   16   32
    Test Template Find and Change among Two   ${MEM TYPE GLOBAL}   u8   ${MEM TYPE GLOBAL}   u16   ${value evolution}   64


Find and Change Heap Uint8 and Uint16
    ${value evolution}=    Create List   0   4   8   16
    Test Template Find and Change among Two   ${MEM TYPE HEAP}   u8   ${MEM TYPE HEAP}   u16   ${value evolution}   32


Filter Value Int8 is Out of Range
    Test Template Filter Value is Out of Range   i8   -129


Filter Value Uint8 is Out of Range
    Test Template Filter Value is Out of Range   u8   256


Filter Value Int8 is Lower Border
    Test Template Filter Value on a Range Border   i8   -128


Filter Value Uint8 is Upper Border
    Test Template Filter Value on a Range Border   u8   255


No Duplication in Show View When Wrong Command
    ${value evolution}=    Create List   0   8   16   32
    Test Template Find and Change   ${MEM TYPE STACK}   u8   ${value evolution}   64

    ${address}=   Get Subject Variable Address   ${MEM TYPE STACK}   u8
    ${address}=   Convert To Upper Case          ${address}

    Clean Output Stream      ${CRWPROC}
    Send Command   ${CRWPROC}   \\show
    Send Command   ${CRWPROC}   wrong_command

    Wait For Output Stream Should Contain   .* 0 .*${address}.*
    Output Stream Should Not Contain        .* 1 .*${address}.*
    Output Stream Should Not Contain        .* 2 .*${address}.*


*** Keywords ***

Test Template Filter Value is Out of Range
    [Arguments]   ${var type}   ${value}
    Connect to Subject Process

    Clean Output Stream    ${CRWPROC}
    Create Exact Filter    ${var type}   ${value}
    ${result}=   Output Stream Contains    ${CRWPROC}    .*Error: specified value '.*' is out of range.*
    Should Be True   ${result}

    ${result}=   Output Stream Contains    ${CRWPROC}    .*Press any key to continue.*
    Should Be True   ${result}


Test Template Filter Value on a Range Border
    [Arguments]   ${var type}   ${value}
    Connect to Subject Process

    Clean Output Stream    ${CRWPROC}
    Create Exact Filter    u8   255
    ${result}=   Output Stream Contains    ${CRWPROC}    .*Error: specified value '.*' is out of range.*
    Should Be Equal    ${FALSE}    ${result}


Test Template Find and Change
    [Arguments]   ${mem type}   ${var type}   ${value evolution}   ${value to set}
    Connect to Subject Process

    ${value}=   Set Variable   ${value evolution[0]}
    Set Subject Variable    ${mem type}    ${var type}    ${value}
    Create Exact Filter     ${var type}    ${value}

    ${length}=    Get length    ${value evolution}
    FOR    ${index}    IN RANGE    1    ${length}
        ${value}=   Set Variable   ${value evolution[${index}]}
        Set Subject Variable          ${mem type}    ${var type}    ${value}
        Update Filter and Continue    ${value}
    END

    ${address}=    Get Subject Variable Address           ${mem type}    ${var type}
    ${index}=      Get Index From View Table By Address   ${address}

    Set Value via CRWPROC    ${index}       ${value to set}
    Check Subject Variable   ${mem type}    ${var type}   ${value to set}


Test Template Find and Change among Two
    [Arguments]   ${mem type 1}   ${var type 1}   ${mem type 2}   ${var type 2}   ${value evolution}   ${value to set}
    Connect to Subject Process

    ${value}=   Set Variable   ${value evolution[0]}
    Set Subject Variable    ${mem type 1}    ${var type 1}    ${value}
    Set Subject Variable    ${mem type 2}    ${var type 2}    ${value}

    Create Exact Filter     ${var type 1}    ${value}

    ${length}=    Get length    ${value evolution}
    FOR    ${index}    IN RANGE    1    ${length}
        ${value}=   Set Variable   ${value evolution[${index}]}
        Set Subject Variable          ${mem type 1}    ${var type 1}    ${value}
        Set Subject Variable          ${mem type 2}    ${var type 2}    ${value}

        Update Filter and Continue    ${value}
    END

    ${address}=    Get Subject Variable Address           ${mem type 1}    ${var type 1}
    ${index}=      Get Index From View Table By Address   ${address}

    Set Value via CRWPROC    ${index}       ${value to set}
    Check Subject Variable   ${mem type 1}    ${var type 1}   ${value to set}
