# il_parser
IEC61131-3 IL parser

### EXAMPLE
```
<START>
labels:
[0009] while:
[0019] reverse:
[0023] endwhile:
[0025] end:

program:
  [0001] LD %i0.0
  [0002] ST %m0.0
  [0003] EQ %m3.0
  [0004] JMPC end
  [0005] LDN bool#true
  [0006] ST %m1.0
  [0007] eq %m3.0
  [0008] JMPC end
  [0009] LD 450
  [0010] EQ %m1.0
  [0011] JMPNC endwhile
  [0012] LD UINT#16#a5
  [0013] LT %m1.0
  [0014] jmpc reverse
  [0015] LD 23.6
  [0016] SUB %m1.0
  [0017] ST %m0.0
  [0018] JMP while
  [0019] LD -12e-5
  [0020] SUB %m0.0
  [0021] ST %m1.0
  [0022] JMP while
  [0023] LD a_Variable_23
  [0024] ST %qw5
  [0025] ST %q0.0
  [0026] LD TIME_OF_DAY#11:36:15.20
  [0027] LD TIME#1h_15m_30s_60ms
  [0028] LD TIME#18ms
  [0029] LD DATE#2001-04-09
  [0030] LD DATE_AND_TIME#2001-04-09-11:36:15.20
  [0031] LD "this is a string"
  [0032] LD 'a'
  [0033] LD 'AaBb'
  [0034] CAL CTU_1 (RESET:=%IX3.6, PV:=Limit, CU:=_1S2)

> identify literals
> parse values

----------
- RESULT -
----------
  [0001] LD IX0.0 (type: NULL#, format: LIT_PHY, value: I, X, 0, 0)
  [0002] ST MX0.0 (type: NULL#, format: LIT_PHY, value: M, X, 0, 0)
  [0003] EQ MX3.0 (type: NULL#, format: LIT_PHY, value: M, X, 3, 0)
  [0004] JMPC 25 (type: NULL#, format: LIT_NONE, value: [null])
  [0005] LDN 1 (type: BOOL#, format: LIT_BOOLEAN, value: 1)
  [0006] ST MX1.0 (type: NULL#, format: LIT_PHY, value: M, X, 1, 0)
  [0007] EQ MX3.0 (type: NULL#, format: LIT_PHY, value: M, X, 3, 0)
  [0008] JMPC 25 (type: NULL#, format: LIT_NONE, value: [null])
  [0009] LD 450 (type: NULL#, format: LIT_INTEGER, value: 450)
  [0010] EQ MX1.0 (type: NULL#, format: LIT_PHY, value: M, X, 1, 0)
  [0011] JMPCN 23 (type: NULL#, format: LIT_NONE, value: [null])
  [0012] LD A5 (type: UINT#, format: LIT_INTEGER, value: 165)
  [0013] LT MX1.0 (type: NULL#, format: LIT_PHY, value: M, X, 1, 0)
  [0014] JMPC 19 (type: NULL#, format: LIT_NONE, value: [null])
  [0015] LD 23.6 (type: NULL#, format: LIT_REAL, value: 23.600000)
  [0016] SUB MX1.0 (type: NULL#, format: LIT_PHY, value: M, X, 1, 0)
  [0017] ST MX0.0 (type: NULL#, format: LIT_PHY, value: M, X, 0, 0)
  [0018] JMP 9 (type: NULL#, format: LIT_NONE, value: [null])
  [0019] LD -12E-5 (type: NULL#, format: LIT_REAL, value: -0.000120)
  [0020] SUB MX0.0 (type: NULL#, format: LIT_PHY, value: M, X, 0, 0)
  [0021] ST MX1.0 (type: NULL#, format: LIT_PHY, value: M, X, 1, 0)
  [0022] JMP 9 (type: NULL#, format: LIT_NONE, value: [null])
  [0023] LD a_Variable_23 (type: NULL#, format: LIT_VAR, value: a_Variable_23)
  [0024] ST QX5.0 (type: SINT#, format: LIT_PHY, value: Q, X, 5, 0)
  [0025] ST QX0.0 (type: NULL#, format: LIT_PHY, value: Q, X, 0, 0)
  [0026] LD 15 (type: TOD#, format: LIT_TIME_OF_DAY, value: 11, 36, 15, 20)
  [0027] LD 1 (type: NULL#, format: LIT_DURATION, value: 1, 15, 30, 60)
  [0028] LD 18 (type: NULL#, format: LIT_DURATION, value: 0, 0, 0, 18)
  [0029] LD 04 (type: NULL#, format: LIT_DATE, value: 2001, 4, 9)
  [0030] LD 15 (type: DT#, format: LIT_DATE_AND_TIME, value: 2001, 4, 9, 11, 36, 15, 20)
  [0031] LD "this is a string" (type: NULL#, format: LIT_STRING, value: "this is a string")
  [0032] LD 'a' (type: NULL#, format: LIT_STRING, value: 'a')
  [0033] LD 'AaBb' (type: NULL#, format: LIT_STRING, value: 'AaBb')
  [0034] CAL _1S2 (type: NULL#, format: LIT_CAL, value: func: CTU_1)
            [var: RESET type: NULL#, format: LIT_PHY value: I, X, 3, 6]    
            [var: PV type: NULL#, format: LIT_VAR value: CU]    
            [var: CU type: NULL#, format: LIT_VAR value: _1S2]    
  [0035] END  (type: NULL#, format: LIT_NONE, value: [null])

//////////////////////////////////////

<START>
labels:
[none]

program:
  [0001] LD %i0.4
  [0002] OR( %i1.2
  [0003] AND( %i0.3
  [0004] OR( %i0.2
  [0005] OR( %i0.1
  [0006] )
  [0007] )
  [0008] )
  [0009] OR( %i0.6
  [0010] AND( %i1.0
  [0011] OR( %i0.7
  [0012] ORN( %i0.5
  [0013] AND( %i0.3
  [0014] OR( %i0.2
  [0015] OR( %i0.1
  [0016] )
  [0017] )
  [0018] )
  [0019] )
  [0020] )
  [0021] )
  [0022] )
  [0023] )
  [0024] S
  [0025] ST  %Q0.0

> identify literals
> parse values

----------
- RESULT -
----------
  [0001] LD IX0.4 (type: NULL#, format: LIT_PHY, value: I, X, 0, 4)
  [0002] OR( IX1.2 (type: NULL#, format: LIT_PHY, value: I, X, 1, 2)
  [0003] AND( IX0.3 (type: NULL#, format: LIT_PHY, value: I, X, 0, 3)
  [0004] OR( IX0.2 (type: NULL#, format: LIT_PHY, value: I, X, 0, 2)
  [0005] OR( IX0.1 (type: NULL#, format: LIT_PHY, value: I, X, 0, 1)
  [0006] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0007] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0008] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0009] OR( IX0.6 (type: NULL#, format: LIT_PHY, value: I, X, 0, 6)
  [0010] AND( IX1.0 (type: NULL#, format: LIT_PHY, value: I, X, 1, 0)
  [0011] OR( IX0.7 (type: NULL#, format: LIT_PHY, value: I, X, 0, 7)
  [0012] ORN( IX0.5 (type: NULL#, format: LIT_PHY, value: I, X, 0, 5)
  [0013] AND( IX0.3 (type: NULL#, format: LIT_PHY, value: I, X, 0, 3)
  [0014] OR( IX0.2 (type: NULL#, format: LIT_PHY, value: I, X, 0, 2)
  [0015] OR( IX0.1 (type: NULL#, format: LIT_PHY, value: I, X, 0, 1)
  [0016] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0017] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0018] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0019] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0020] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0021] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0022] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0023] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0024] S  (type: NULL#, format: LIT_NONE, value: [null])
  [0025] ST QX0.0 (type: NULL#, format: LIT_PHY, value: Q, X, 0, 0)
  [0026] END  (type: NULL#, format: LIT_NONE, value: [null])
```
