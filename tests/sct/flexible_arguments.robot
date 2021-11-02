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

Set All Variables and Revert
    ${var types}   Create List   i8   i16   i32   i64
    Test Template Set All and Revert   ${MEM TYPE STACK}   ${var types}   8   16


Set First Half Variables Size 3 and Revert
    ${var types}   Create List   i8   i16   i32
    Test Template Set First Half and Revert   ${MEM TYPE STACK}   ${var types}   8   16   0   2


Set First Half Variables Size 4 and Revert
    ${var types}   Create List   i8   i16   i32   i64
    Test Template Set First Half and Revert   ${MEM TYPE STACK}   ${var types}   8   16   0   2


Set Second Half Variables Size 3 and Revert
    ${var types}   Create List   i8   i16   i32
    Test Template Set Second Half and Revert   ${MEM TYPE STACK}   ${var types}   8   16   2   3


Set Second Half Variables Size 4 and Revert
    ${var types}   Create List   i8   i16   i32   i64
    Test Template Set Second Half and Revert   ${MEM TYPE STACK}   ${var types}   8   16   2   4


*** Keywords ***

Test Template Set All and Revert
    [Arguments]   ${mem type}   ${var types}   ${inital value}   ${final value}
    Connect to Subject Process

    ${length}=  Get Length   ${var types}

    FOR   ${index}   IN RANGE   0   ${length}
        ${type}=      Get From List   ${var types}   ${index}

        Set Subject Variable   ${mem type}   ${type}   ${inital value}

        ${address}=   Get Subject Variable Address       ${mem type}   ${type}
        ${address}=   Convert To Upper Case              ${address}
    
        Find Value by Address and Add It   ${inital value}   ${address}   ${mem type}   ${type}
    END

    Send Command    ${CRWPROC}   \\edit
    Send Command    ${CRWPROC}   \\set all ${final value}

    FOR   ${index}   IN RANGE   0   ${length}
        ${type}=      Get From List   ${var types}   ${index}
        Check Subject Variable   ${mem type}   ${type}   ${final value}
    END

    Send Command    ${CRWPROC}   \\revert

    FOR   ${index}   IN RANGE   0   ${length}
        ${type}=      Get From List   ${var types}   ${index}
        Check Subject Variable   ${mem type}   ${type}   ${inital value}
    END


Test Template Set First Half and Revert
    [Arguments]   ${mem type}   ${var types}   ${inital value}   ${final value}  ${from}   ${to}
    Connect to Subject Process

    ${length}=  Get Length   ${var types}

    FOR   ${index}   IN RANGE   0   ${length}
        ${type}=      Get From List   ${var types}   ${index}

        Set Subject Variable   ${mem type}   ${type}   ${inital value}

        ${address}=   Get Subject Variable Address       ${mem type}   ${type}
        ${address}=   Convert To Upper Case              ${address}
    
        Find Value by Address and Add It   ${inital value}   ${address}   ${mem type}   ${type}
    END

    Send Command    ${CRWPROC}   \\edit
    Send Command    ${CRWPROC}   \\set 1half ${final value}

    FOR   ${index}   IN RANGE   ${from}   ${to}
        ${type}=      Get From List   ${var types}   ${index}
        Check Subject Variable   ${mem type}   ${type}   ${final value}
    END
    
    FOR   ${index}   IN RANGE   ${to}   ${length}
        ${type}=      Get From List   ${var types}   ${index}
        Check Subject Variable   ${mem type}   ${type}   ${inital value}
    END

    Send Command    ${CRWPROC}   \\revert

    FOR   ${index}   IN RANGE   0   ${length}
        ${type}=      Get From List   ${var types}   ${index}
        Check Subject Variable   ${mem type}   ${type}   ${inital value}
    END


Test Template Set Second Half and Revert
    [Arguments]   ${mem type}   ${var types}   ${inital value}   ${final value}  ${from}   ${to}
    Connect to Subject Process

    ${length}=  Get Length   ${var types}

    FOR   ${index}   IN RANGE   0   ${length}
        ${type}=      Get From List   ${var types}   ${index}

        Set Subject Variable   ${mem type}   ${type}   ${inital value}

        ${address}=   Get Subject Variable Address       ${mem type}   ${type}
        ${address}=   Convert To Upper Case              ${address}
    
        Find Value by Address and Add It   ${inital value}   ${address}   ${mem type}   ${type}
    END

    Send Command    ${CRWPROC}   \\edit
    Send Command    ${CRWPROC}   \\set 2half ${final value}

    FOR   ${index}   IN RANGE   0   ${from}
        ${type}=      Get From List   ${var types}   ${index}
        Check Subject Variable   ${mem type}   ${type}   ${inital value}
    END

    FOR   ${index}   IN RANGE   ${from}   ${to}
        ${type}=      Get From List   ${var types}   ${index}
        Check Subject Variable   ${mem type}   ${type}   ${final value}
    END

    Send Command    ${CRWPROC}   \\revert

    FOR   ${index}   IN RANGE   0   ${length}
        ${type}=      Get From List   ${var types}   ${index}
        Check Subject Variable   ${mem type}   ${type}   ${inital value}
    END