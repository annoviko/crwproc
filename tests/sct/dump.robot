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

Read Byte Sequence By Address
    Connect to Subject Process
    ${address}=   Send HTTP Request with Check and Get Body   GET   /address/sequence   ${200}

    Clean Output Stream      ${CRWPROC}

    Send Command    ${CRWPROC}   \\dump
    Send Command    ${CRWPROC}   ${address}
    Send Command    ${CRWPROC}   32

    Output Stream Should Contain   .*10 21 12 23 14 25 16 27 18 29 1A 2B 1C 2D 1E 2F.*
    Output Stream Should Contain   .*A0 B1 A2 B3 A4 B5 A6 B7 A8 B9 AA BB AC BD AE BF.*
