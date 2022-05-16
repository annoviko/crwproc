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

Find Byte Sequence 11 Bytes
    Connect to Subject Process
    ${address}=   Send HTTP Request with Check and Get Body   GET   /address/sequence   ${200}
    ${address}=   Convert To Upper Case   ${address}

    Clean Output Stream      ${CRWPROC}

    Send Command    ${CRWPROC}   \\find_sequence
    Send Command    ${CRWPROC}   10 21 12 23 14 25 16 27 18 29 1A

    Wait For Output Stream Should Contain   .*${address}.*


Find Byte Sequence 16 Bytes
    Connect to Subject Process
    ${address}=   Send HTTP Request with Check and Get Body   GET   /address/sequence   ${200}
    ${address}=   Convert To Upper Case   ${address}

    Clean Output Stream      ${CRWPROC}

    Send Command    ${CRWPROC}   \\find_sequence
    Send Command    ${CRWPROC}   10 21 12 23 14 25 16 27 18 29 1A 2B 1C 2D 1E 2F A0 B1 A2 B3 A4 B5 A6 B7 A8 B9 AA BB AC BD AE BF

    Wait For Output Stream Should Contain   .*${address}.*


Find Byte Sequence Failure
    Connect to Subject Process
    ${address}=   Send HTTP Request with Check and Get Body   GET   /address/sequence   ${200}
    ${address}=   Convert To Upper Case   ${address}

    Clean Output Stream      ${CRWPROC}

    Send Command    ${CRWPROC}   \\find_sequence
    Send Command    ${CRWPROC}   00 01 02 03 04 05 06 07 08 09 0A FF FF FF

    Wait For Output Stream Should Not Contain   .*${address}.*
    Output Stream Should Contain       .*Byte sequence has not been found.*
