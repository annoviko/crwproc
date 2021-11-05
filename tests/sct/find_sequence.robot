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
    Send Command    ${CRWPROC}   00 01 02 03 04 05 06 07 08 09 0A

    Output Stream Should Contain   .*${address}.*


Find Byte Sequence 16 Bytes
    Connect to Subject Process
    ${address}=   Send HTTP Request with Check and Get Body   GET   /address/sequence   ${200}
    ${address}=   Convert To Upper Case   ${address}

    Clean Output Stream      ${CRWPROC}

    Send Command    ${CRWPROC}   \\find_sequence
    Send Command    ${CRWPROC}   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F

    Output Stream Should Contain   .*${address}.*


Find Byte Sequence Failure
    Connect to Subject Process
    ${address}=   Send HTTP Request with Check and Get Body   GET   /address/sequence   ${200}
    ${address}=   Convert To Upper Case   ${address}

    Clean Output Stream      ${CRWPROC}

    Send Command    ${CRWPROC}   \\find_sequence
    Send Command    ${CRWPROC}   00 01 02 03 04 05 06 07 08 09 0A FF FF FF

    Output Stream Should Not Contain   .*${address}.*
    Output Stream Should Contain       .*Byte sequence has not been found.*
