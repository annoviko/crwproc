*** Settings ***

Library    libs/proc_interact.py
Library    libs/crwproc_interact.py


*** Test Cases ***

Start and Exit
    ${crwproc}=        Crwproc Run     ./../../x64/Release/    ${TEST NAME}
    ${exit code}=      Crwproc Exit    ${crwproc}
    Should Be Equal    ${exit code}    ${0}


Start and Wrong Input and Exit
    ${crwproc}=        Crwproc Run     ./../../x64/Release/    ${TEST NAME}
    Send Command       ${crwproc}      \\WrongCommand

    Crwproc Press Any Key   ${crwproc}

    ${exit code}=      Crwproc Exit    ${crwproc}
    Should Be Equal    ${exit code}    ${0}
