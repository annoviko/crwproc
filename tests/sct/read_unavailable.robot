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

Read Unavailable Value
    Connect to Subject Process
    Create Exact Filter     u8    0
    Navigate to Show View
    Send Command           ${CRWPROC}    \\add 0-10

    Terminate Subject
    Sleep   1000ms

    Navigate to Edit Table
    Wait For Output Stream Should Contain    .*[TERMINATED].*
    FOR    ${index}    IN RANGE    0    11
        Wait For Output Stream Should Contain    .* ${index} .* ######## .*
    END
