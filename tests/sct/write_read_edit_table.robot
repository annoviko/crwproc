*** Settings ***

Library          OperatingSystem
Library          String

Library          HttpCtrl.Client
Library          HttpCtrl.Json

Library          libs/crwproc_interact.py
Library          libs/proc_interact.py

Resource         libs/common.robot
Resource         libs/subject_interact.robot

Test Setup       Initialize Object and Subject
Test Teardown    Terminate Object and Subject



*** Test Cases ***

Save Edit Table With One Entry Uint8
    Test Template Save Edit Table With One Entry   u8   128


Save Edit Table With One Entry Float
    Test Template Save Edit Table With One Entry   flt   12.8


Save Edit Table With One Entry Double
    Test Template Save Edit Table With One Entry   dbl   153.7809


Save Edit Table With Two Entries
    Connect to Subject Process
    ${address 0}=   Find Value By Address And Add It   ${MEM TYPE STACK}   u32   131072
    ${address 1}=   Find Value By Address And Add It   ${MEM TYPE STACK}   dbl   3.14
    Navigate To Edit View and Check Variable   0   ${address 0}   131072
    Navigate To Edit View and Check Variable   1   ${address 1}   3.14
    
    Save Edit Table   edit_table.json

    ${json content}=   Get File   edit_table.json
    Check Variable In Edit Table Json Content   ${json content}   0   ${address 0}   u32
    Check Variable In Edit Table Json Content   ${json content}   1   ${address 1}   dbl


Save Edit Table With Three Entries
    Connect to Subject Process
    ${address 0}=   Find Value By Address And Add It   ${MEM TYPE STACK}   i32   -452389
    ${address 1}=   Find Value By Address And Add It   ${MEM TYPE STACK}   flt   1.4
    ${address 2}=   Find Value By Address And Add It   ${MEM TYPE STACK}   dbl   -1.345
    Navigate To Edit View and Check Variable   0   ${address 0}   -452389
    Navigate To Edit View and Check Variable   1   ${address 1}   1.4
    Navigate To Edit View and Check Variable   2   ${address 2}   -1.345
    
    Save Edit Table   edit_table.json

    ${json content}=   Get File   edit_table.json
    Check Variable In Edit Table Json Content   ${json content}   0   ${address 0}   i32
    Check Variable In Edit Table Json Content   ${json content}   1   ${address 1}   flt
    Check Variable In Edit Table Json Content   ${json content}   2   ${address 2}   dbl


Load Edit Table With One Entry
    Test Template Save Edit Table With One Entry   u8   128
    Terminate Crwproc

    Initialize Crwproc
    Connect to Subject Process
    Load Edit Table   edit_table.json

    Navigate To Edit View and Check Variable   0   ${None}   128


Load Edit Table With Two Entries
    Connect to Subject Process
    ${address 0}=   Find Value By Address And Add It   ${MEM TYPE STACK}   u32   6782034
    ${address 1}=   Find Value By Address And Add It   ${MEM TYPE STACK}   dbl   3.14
    Navigate To Edit View and Check Variable   0   ${address 0}   6782034
    Navigate To Edit View and Check Variable   1   ${address 1}   3.14
    
    Save Edit Table   edit_table.json

    ${json content}=   Get File   edit_table.json
    Check Variable In Edit Table Json Content   ${json content}   0   ${address 0}   u32
    Check Variable In Edit Table Json Content   ${json content}   1   ${address 1}   dbl

    Terminate Crwproc
    Initialize Crwproc
    Connect to Subject Process
    Load Edit Table   edit_table.json

    Navigate To Edit View and Check Variable   0   ${None}   6782034
    Navigate To Edit View and Check Variable   1   ${None}   3.14


*** Keywords ***

Test Template Save Edit Table With One Entry
    [Arguments]   ${var type}   ${value}
    Connect to Subject Process
    ${address}=   Find Value By Address And Add It   ${MEM TYPE STACK}   ${var type}   ${value}
    Navigate To Edit View and Check Variable   0   ${address}   ${value}

    Save Edit Table   edit_table.json

    ${json content}=   Get File   edit_table.json
    Check Variable In Edit Table Json Content   ${json content}   0   ${address}   ${var type}


Find Value By Address And Add It
    [Arguments]   ${mem type}   ${var type}   ${initial value}
    ${address}=    Get Subject Variable Address       ${mem type}   ${var type}
    ${address}=    Convert To Upper Case              ${address}

    Find Value by Address and Check Output   ${initial value}   ${address}   ${mem type}   ${var type}
    Send Command   ${CRWPROC}   \\add
    [Return]   ${address}


Navigate To Edit View and Check Variable
    [Arguments]   ${index}   ${address}   ${value}
    Clean Output Stream      ${CRWPROC}
    Send Command   ${CRWPROC}   \\edit

    ${pattern} =    Set Variable If    "${address}" == "${None}"    .*${index}.*${value}.*    .*${index}.*${address}.*${value}.*

    ${result}=    Output Stream Contains    ${CRWPROC}    ${pattern}
    Should Be True    ${result}    ${pattern}


Save Edit Table
    [Arguments]   ${filename}
    Send Command   ${CRWPROC}   \\edit
    Send Command   ${CRWPROC}   \\save ${filename}


Load Edit Table
    [Arguments]   ${filename}
    Send Command   ${CRWPROC}   \\edit
    Send Command   ${CRWPROC}   \\load ${filename}


Check Variable In Edit Table Json Content
    [Arguments]   ${json content}   ${index}   ${address}   ${var type}
    ${typename}=            Get Typename By Type         ${var type}
    ${size}=                Get Variable Size By Type    ${var type}
    ${signed}=              Is Variable Signed By Type   ${var type}
    
    ${actual address}=      Get Json Value From String   ${json content}   ${index}/address
    ${actual typename}=     Get Json Value From String   ${json content}   ${index}/type/type
    ${actual size}=         Get Json Value From String   ${json content}   ${index}/type/size
    ${actual signed}=       Get Json Value From String   ${json content}   ${index}/type/signed

    ${address}=             Convert To Integer   ${address}   ${16}
    ${actual address}=      Convert To Integer   ${actual address}
    ${actual size}=         Convert To Integer   ${actual size}
    ${actual signed}=       Convert To Boolean   ${actual signed}

    Should Be Equal   ${actual typename}   ${typename}
    Should Be Equal   ${actual address}    ${address}
    Should Be Equal   ${actual size}       ${size}
    Should Be Equal   ${actual signed}     ${signed}
