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

    Output Stream Should Contains   .*00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F.*
    Output Stream Should Contains   .*10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F.*
