*** Settings ***

Library          HttpCtrl.Client
Library          libs/crwproc_interact.py
Library          libs/proc_interact.py

Resource         libs/common.robot
Resource         libs/subject_interact.robot

Test Setup       Initialize Object and Subject
Test Teardown    Terminate Object and Subject



*** Test Cases ***

Wrong Command
    Test Template Set View Wrong Command    wrong_command qwe    .*Error: unknown command is specified 'wrong_command'.*


Wrong Index
    Test Template Set View Wrong Command    \\set qwe 16    .*Error: impossible to parse input argument 'qwe' as an index or range.*


Wrong Value
    Test Template Set View Wrong Command    \\set 0 zxcv    .*Error: impossible to set value 'zxcv' for.*


Out of Range Index
    Test Template Set View Wrong Command    \\set 128 64    .*Error: input value '128' is out of range \\(0-1\\).*



*** Keywords ***

Test Template Set View Wrong Command
    [Arguments]   ${command line}   ${error message}
    Connect to Subject Process
    Create Exact Filter    u8   64

    Navigate to Show View
    Send Command   ${CRWPROC}   \\add 0

    Clean Output Stream    ${CRWPROC}
    Send Command   ${CRWPROC}   \\edit
    Send Command    ${CRWPROC}    ${command line}
    Crwproc Press Any Key    ${CRWPROC}

    ${result}=   Output Stream Contains    ${CRWPROC}    ${error message}
    Should Be True    ${result}

    ${result}=   Output Stream Contains    ${CRWPROC}    .*Press any key to continue.*
    Should Be True    ${result}
