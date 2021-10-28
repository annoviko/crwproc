*** Settings ***

Library          HttpCtrl.Client
Library          libs/crwproc_interact.py
Library          libs/proc_interact.py

Resource         libs/common.robot
Resource         libs/subject_interact.robot

Test Setup       Initialize Object and Subject
Test Teardown    Terminate Object and Subject



*** Test Cases ***

Save Edit Table With One Entry
    Should Be True    ${True}