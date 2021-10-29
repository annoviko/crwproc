*** Settings ***

Library         libs/proc_interact.py
Library         libs/crwproc_interact.py

Resource        libs/common.robot


*** Test Cases ***

Start and Exit
    Initialize Crwproc
    ${exit code}=      Crwproc Exit    ${CRWPROC}
    Should Be Equal    ${exit code}    ${0}


Start and Wrong Input and Exit
    Initialize Crwproc
    Send Command       ${CRWPROC}      \\WrongCommand

    Crwproc Press Any Key   ${CRWPROC}

    ${exit code}=      Crwproc Exit    ${CRWPROC}
    Should Be Equal    ${exit code}    ${0}
