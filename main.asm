; port definitions
LEDState            EQU         P1.0
buttonS1            EQU         P1.1

; constant definitions
maxTime             EQU         100
zero                EQU         0
twentyFive          EQU         25
fifty               EQU         50
seventyFive         EQU         75
oneHundred          EQU         100 

; first instruction, after resetting the microcontroller
CSEG AT 0000H
    JMP main

CSEG AT 0003H
    JMP externalInterruption0

CSEG AT 000BH
    JMP externalInterruption1

CSEG AT 0014H
    JMP timerInterruption0

CSEG AT 001CH
    JMP timerInterruption1

main:
    LCALL init

; REGISTERS
; R0 - changeIntensityState
; R1 - clappingDetected
; R2 - timerCounter
; R3 - timerClap
; R4 - LEDEnabled
; R5 - LEDBrightness

externalInterruption0:
    SETB changeIntensityState
    RETI

externalInterruption1:
    SETB clappingDetected
    RETI

timerInterruption0:
    INC R2
    RETI

timerInterruption1:
    INC R3
    RETI

init:
    MOV IE, #10001111B
    MOV TMOD, #0000010B
    MOV TH0, #037H
    MOV TL0, #037H
    MOV TH1, #037H
    MOV TL1, #037H
    SETB TR0
    SETB IT0
    SETB TR1
    SETB IT1
    CLR LEDState

    MOV R0, #0
    MOV R1, #0
    MOV R2, #0
    MOV R3, #0
    MOV R4, #0
    MOV R5, #oneHundred
    RET

    END

turnOnOffLED:
    MOV A, R4
    SUBB A, 1
    JNC turnOnOffLEDDisable
    MOV B, 1
    MOV LEDState, B
    MOV LEDEnabled, B
    MOV LEDBrightness, #oneHundred
    RET
    END
turnOnOffLEDDisable:
    MOV B, 0
    MOV LEDState, B
    MOV LEDEnabled, B
    MOV LEDBrightness, #zero
    RET
    END

checkClap:
    CALL turnOnOffLED
    MOV R1, 0
    RET
    END

enabledLEDTask1:
    MOV A, R2
    MOV B, R5
    CJNE A, B, enabledLEDTask2
    CALL disableLEDStateTimer
enabledLEDTask2:
    MOV A, R2
    MOV B, #maxTime
    CJNE A, B, enabledLEDTask3
    CALL enableLEDStateTimer
enabledLEDTask3:
    MOV A, R0
    CJNE A, 1, enabledLEDTaskEnd
    MOV A, R5
    CJNE A, #zero, enabledLEDTask3_twentyFive
    MOV R5, #twentyFive
    JMP enabledLEDTask3End
enabledLEDTask3_twentyFive:
    CJNE A, #twentyFive, enabledLEDTask3_fifty
    MOV R5, #fifty
    JMP enabledLEDTask3End
enabledLEDTask3_fifty:
    CJNE A, #fifty, enabledLEDTask3_seventyFive
    MOV R5, #seventyFive
    JMP enabledLEDTask3End
enabledLEDTask3_seventyFive:
    CJNE A, #seventyFive, enabledLEDTask3_oneHundred
    MOV R5, #oneHundred
    JMP enabledLEDTask3End
enabledLEDTask3_oneHundred:
    CJNE A, #oneHundred, enabledLEDTask3End
    MOV R5, #twentyFive
enabledLEDTask3End:
    MOV R0, 0
enabledLEDTaskEnd:
    RET
    END

disableLEDStateTimer:
    CLR LEDState

enableLEDStateTimer:
    MOV R2, 0
    SETB LEDState

checkbuttonS1:
    CALL turnOnOffLED
checkbuttonS1Loop:
    MOV A, buttonS1
    SUBB A, 1
    JNC checkbuttonS1LeaveRoutine
    JMP checkbuttonS1Loop
checkbuttonS1LeaveRoutine:
    RET
    END

startProgram:
    MOV R6, buttonS1
    MOV A, R6
    SUBB A, 1
    JNC checkbuttonS1
    MOV A, R1
    SUBB A, 1
    JNC checkClap
    MOV A, R5
    SUBB A, 1
    JNC enabledLEDTasks
    JMP startProgram