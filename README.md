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
  [0001] LD IX0.0 (type: ?#, format: LIT_PHY) [I, X, 0, 0]    
  [0002] ST MX0.0 (type: ?#, format: LIT_PHY) [M, X, 0, 0]    
  [0003] EQ MX3.0 (type: ?#, format: LIT_PHY) [M, X, 3, 0]    
  [0004] JMPC 25 (type: ?#, format: LIT_NONE)     
  [0005] LDN 1 (type: BOOL#, format: LIT_BOOLEAN) [1]    
  [0006] ST MX1.0 (type: ?#, format: LIT_PHY) [M, X, 1, 0]    
  [0007] EQ MX3.0 (type: ?#, format: LIT_PHY) [M, X, 3, 0]    
  [0008] JMPC 25 (type: ?#, format: LIT_NONE)     
  [0009] LD 450 (type: ?#, format: LIT_INTEGER) [450]    
  [0010] EQ MX1.0 (type: ?#, format: LIT_PHY) [M, X, 1, 0]    
  [0011] JMPCN 23 (type: ?#, format: LIT_NONE)     
  [0012] LD A5 (type: UINT#, format: LIT_INTEGER) [165]    
  [0013] LT MX1.0 (type: ?#, format: LIT_PHY) [M, X, 1, 0]    
  [0014] JMPC 19 (type: ?#, format: LIT_NONE)     
  [0015] LD 23.6 (type: ?#, format: LIT_REAL) [23.600000]    
  [0016] SUB MX1.0 (type: ?#, format: LIT_PHY) [M, X, 1, 0]    
  [0017] ST MX0.0 (type: ?#, format: LIT_PHY) [M, X, 0, 0]    
  [0018] JMP 9 (type: ?#, format: LIT_NONE)     
  [0019] LD -12E-5 (type: ?#, format: LIT_REAL) [-0.000120]    
  [0020] SUB MX0.0 (type: ?#, format: LIT_PHY) [M, X, 0, 0]    
  [0021] ST MX1.0 (type: ?#, format: LIT_PHY) [M, X, 1, 0]    
  [0022] JMP 9 (type: ?#, format: LIT_NONE)     
  [0023] LD a_Variable_23 (type: ?#, format: LIT_VAR) [a_Variable_23]    
  [0024] ST QX5.0 (type: SINT#, format: LIT_PHY) [Q, X, 5, 0]    
  [0025] ST QX0.0 (type: ?#, format: LIT_PHY) [Q, X, 0, 0]    
  [0026] LD 15 (type: TOD#, format: LIT_TIME_OF_DAY) [11, 36, 15, 20]    
  [0027] LD 1 (type: ?#, format: LIT_DURATION) [1, 15, 30, 60]    
  [0028] LD 18 (type: ?#, format: LIT_DURATION) [0, 0, 0, 18]    
  [0029] LD 04 (type: ?#, format: LIT_DATE) [2001, 4, 9]    
  [0030] LD 15 (type: DT#, format: LIT_DATE_AND_TIME) [2001, 4, 9, 11, 36, 15, 20]    
  [0031] LD "this is a string" (type: ?#, format: LIT_STRING) ["this is a string"]    
  [0032] LD 'a' (type: ?#, format: LIT_STRING) ['a']    
  [0033] LD 'AaBb' (type: ?#, format: LIT_STRING) ['AaBb']    
  [0034] CAL _1S2 (type: ?#, format: LIT_CAL) [func: CTU_1]    
            [var: RESET type: ?#, format: LIT_PHY value: I, X, 3, 6]    
            [var: PV type: ?#, format: LIT_VAR value: CU]    
            [var: CU type: ?#, format: LIT_VAR value: _1S2]    
  [0035] END  (type: ?#, format: LIT_NONE)     

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
  [0001] LD IX0.4 (type: ?#, format: LIT_PHY) [I, X, 0, 4]    
  [0002] OR( IX1.2 (type: ?#, format: LIT_PHY) [I, X, 1, 2]    
  [0003] AND( IX0.3 (type: ?#, format: LIT_PHY) [I, X, 0, 3]    
  [0004] OR( IX0.2 (type: ?#, format: LIT_PHY) [I, X, 0, 2]    
  [0005] OR( IX0.1 (type: ?#, format: LIT_PHY) [I, X, 0, 1]    
  [0006] POP  (type: ?#, format: LIT_NONE)     
  [0007] POP  (type: ?#, format: LIT_NONE)     
  [0008] POP  (type: ?#, format: LIT_NONE)     
  [0009] OR( IX0.6 (type: ?#, format: LIT_PHY) [I, X, 0, 6]    
  [0010] AND( IX1.0 (type: ?#, format: LIT_PHY) [I, X, 1, 0]    
  [0011] OR( IX0.7 (type: ?#, format: LIT_PHY) [I, X, 0, 7]    
  [0012] ORN( IX0.5 (type: ?#, format: LIT_PHY) [I, X, 0, 5]    
  [0013] AND( IX0.3 (type: ?#, format: LIT_PHY) [I, X, 0, 3]    
  [0014] OR( IX0.2 (type: ?#, format: LIT_PHY) [I, X, 0, 2]    
  [0015] OR( IX0.1 (type: ?#, format: LIT_PHY) [I, X, 0, 1]    
  [0016] POP  (type: ?#, format: LIT_NONE)     
  [0017] POP  (type: ?#, format: LIT_NONE)     
  [0018] POP  (type: ?#, format: LIT_NONE)     
  [0019] POP  (type: ?#, format: LIT_NONE)     
  [0020] POP  (type: ?#, format: LIT_NONE)     
  [0021] POP  (type: ?#, format: LIT_NONE)     
  [0022] POP  (type: ?#, format: LIT_NONE)     
  [0023] POP  (type: ?#, format: LIT_NONE)     
  [0024] S  (type: ?#, format: LIT_NONE)     
  [0025] ST QX0.0 (type: ?#, format: LIT_PHY) [Q, X, 0, 0]    
  [0026] END  (type: ?#, format: LIT_NONE) 
```
