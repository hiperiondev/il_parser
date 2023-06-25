# il_parser
IEC61131-3 IL parser

### EXAMPLE
```
------------------ test 1 ------------------
[FILE: test1.il]

[LABELS]
    [NONE]

[0000] LD PHY#i0.4
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 0[I], datatype: 0[X] phy_a: 0, phy_b: 4]

[0001] OR( PHY#i1.2
    [code: 6(0x06)[OR], conditional: 0, negate: 0, push: 1, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 0[I], datatype: 0[X] phy_a: 1, phy_b: 2]

[0002] AND( PHY#i0.3
    [code: 5(0x05)[AND], conditional: 0, negate: 0, push: 1, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 0[I], datatype: 0[X] phy_a: 0, phy_b: 3]

[0003] OR( PHY#i0.2
    [code: 6(0x06)[OR], conditional: 0, negate: 0, push: 1, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 0[I], datatype: 0[X] phy_a: 0, phy_b: 2]

[0004] OR( PHY#i0.1
    [code: 6(0x06)[OR], conditional: 0, negate: 0, push: 1, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 0[I], datatype: 0[X] phy_a: 0, phy_b: 1]

[0005]  )
    [code: 22(0x16)[POP], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0006]  )
    [code: 22(0x16)[POP], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0007]  )
    [code: 22(0x16)[POP], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0008] OR( PHY#i0.6
    [code: 6(0x06)[OR], conditional: 0, negate: 0, push: 1, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 0[I], datatype: 0[X] phy_a: 0, phy_b: 6]

[0009] AND( PHY#i1.0
    [code: 5(0x05)[AND], conditional: 0, negate: 0, push: 1, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 0[I], datatype: 0[X] phy_a: 1, phy_b: 0]

[0010] OR( PHY#i0.7
    [code: 6(0x06)[OR], conditional: 0, negate: 0, push: 1, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 0[I], datatype: 0[X] phy_a: 0, phy_b: 7]

[0011] ORN( PHY#i0.5
    [code: 6(0x06)[OR], conditional: 0, negate: 1, push: 1, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 0[I], datatype: 0[X] phy_a: 0, phy_b: 5]

[0012] AND( PHY#i0.3
    [code: 5(0x05)[AND], conditional: 0, negate: 0, push: 1, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 0[I], datatype: 0[X] phy_a: 0, phy_b: 3]

[0013] OR( PHY#i0.2
    [code: 6(0x06)[OR], conditional: 0, negate: 0, push: 1, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 0[I], datatype: 0[X] phy_a: 0, phy_b: 2]

[0014] OR( PHY#i0.1
    [code: 6(0x06)[OR], conditional: 0, negate: 0, push: 1, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 0[I], datatype: 0[X] phy_a: 0, phy_b: 1]

[0015]  )
    [code: 22(0x16)[POP], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0016]  )
    [code: 22(0x16)[POP], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0017]  )
    [code: 22(0x16)[POP], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0018]  )
    [code: 22(0x16)[POP], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0019]  )
    [code: 22(0x16)[POP], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0020]  )
    [code: 22(0x16)[POP], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0021]  )
    [code: 22(0x16)[POP], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0022]  )
    [code: 22(0x16)[POP], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0023]  S
    [code: 3(0x03)[S], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0024] ST PHY#Q0.0
    [code: 2(0x02)[ST], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 1[Q], datatype: 0[X] phy_a: 0, phy_b: 0]

[0025] END
    [code: 31(0x1f)[END], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[lines = 26]
--------------------------------------------

------------------ test 2 ------------------
[FILE: test2.il]

[LABELS]
    [while line: 8]
    [reverse line: 18]
    [endwhile line: 22]
    [end line: 24]

[0000] LD PHY#i0.0
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 0[I], datatype: 0[X] phy_a: 0, phy_b: 0]

[0001] ST PHY#mb0
    [code: 2(0x02)[ST], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 2[M], datatype: 1[B] phy_byte: 0]

[0002] EQ PHY#m3.0
    [code: 15(0x0f)[EQ], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 2[M], datatype: 0[X] phy_a: 3, phy_b: 0]

[0003] JMPC 24
    [code: 19(0x13)[JMP], conditional: 1, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0004] LDN bool#true
    [code: 1(0x01)[LD], conditional: 0, negate: 1, push: 0, lit_dataformat: LIT_BOOLEAN, iec_datatype: BOOL#]
        [boolean: 1]

[0005] ST PHY#m1.0
    [code: 2(0x02)[ST], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 2[M], datatype: 0[X] phy_a: 1, phy_b: 0]

[0006] EQ PHY#m3.0
    [code: 15(0x0f)[EQ], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 2[M], datatype: 0[X] phy_a: 3, phy_b: 0]

[0007] JMPC 24
    [code: 19(0x13)[JMP], conditional: 1, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0008] LD 450
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_INTEGER, iec_datatype: NULL#]
        [integer: 450]

[0009] EQ PHY#m1.0
    [code: 15(0x0f)[EQ], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 2[M], datatype: 0[X] phy_a: 1, phy_b: 0]

[0010] JMPNC 22
    [code: 19(0x13)[JMP], conditional: 1, negate: 1, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0011] LD UINT#16#a5
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_BASE16, iec_datatype: UINT#]
value: [A5](165)
        [integer: 165]

[0012] LT PHY#m1.0
    [code: 18(0x12)[LT], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 2[M], datatype: 0[X] phy_a: 1, phy_b: 0]

[0013] JMPC 18
    [code: 19(0x13)[JMP], conditional: 1, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0014] LD 23.6
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_REAL, iec_datatype: NULL#]
        [real: 23.600000]

[0015] SUB PHY#m1.0
    [code: 10(0x0a)[SUB], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 2[M], datatype: 0[X] phy_a: 1, phy_b: 0]

[0016] ST PHY#m0.0
    [code: 2(0x02)[ST], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 2[M], datatype: 0[X] phy_a: 0, phy_b: 0]

[0017] JMP 8
    [code: 19(0x13)[JMP], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0018] LD -12e-5
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_REAL_EXP, iec_datatype: NULL#]
        [real: -0.000120]

[0019] SUB PHY#m0.0
    [code: 10(0x0a)[SUB], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 2[M], datatype: 0[X] phy_a: 0, phy_b: 0]

[0020] ST PHY#md4.23
    [code: 2(0x02)[ST], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 2[M], datatype: 3[D] phy_double: 4.230000]

[0021] JMP 8
    [code: 19(0x13)[JMP], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[0022] LD a_Variable_23
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_VAR, iec_datatype: NULL#]
        [variable: a_Variable_23]

[0023] ST PHY#qw5
    [code: 2(0x02)[ST], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 1[Q], datatype: 2[W] phy_word: 5]

[0024] ST PHY#q0.0
    [code: 2(0x02)[ST], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_PHY, iec_datatype: PHY#]
        [prefix: 1[Q], datatype: 0[X] phy_a: 0, phy_b: 0]

[0025] LD TIME_OF_DAY#11:36:15.20
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_TIME_OF_DAY, iec_datatype: NULL#]
        [H: 11, M: 36, S: 15, MS: 20]

[0026] LD TIME#1h_15m_30s_60ms
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_DURATION, iec_datatype: TIME#]
        [H: 1, M: 15, S: 30, MS: 60]

[0027] LD TIME#15m_30s
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_DURATION, iec_datatype: TIME#]
        [H: 0, M: 15, S: 30, MS: 0]

[0028] CAL FUN_3 (var1:=TIME#1h_15m_30s_60ms, PV:=DATE_AND_TIME#2001-04-09-11:36:15.20, CU:=-12e6, DT:=DATE#2001-04-09)
    [code: 20(0x14)[CAL], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_CAL, iec_datatype: NULL#]
    [ var1 (lit_dataformat: LIT_DURATION, iec_datatype: TIME#) ]
        [H: 1, M: 15, S: 30, MS: 60]
    [ PV (lit_dataformat: LIT_DATE_AND_TIME, iec_datatype: TIME#) ]
        [year: 2001, month: 4, day: 9, H: 11, M: 36, S: 15, MS: 20]
    [ CU (lit_dataformat: LIT_REAL_EXP, iec_datatype: NULL#) ]
        [real: -12000000.000000]
    [ DT (lit_dataformat: LIT_DATE, iec_datatype: DATE#) ]
        [year: 2001, month: 4, day: 9]

[0029] LD TIME#18ms
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_DURATION, iec_datatype: TIME#]
        [H: 0, M: 0, S: 0, MS: 18]

[0030] LD DATE#2001-04-09
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_DATE, iec_datatype: DATE#]
        [year: 2001, month: 4, day: 9]

[0031] LD DATE_AND_TIME#2001-04-09-11:36:15.20
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_DATE_AND_TIME, iec_datatype: TIME#]
        [year: 2001, month: 4, day: 9, H: 11, M: 36, S: 15, MS: 20]

[0032] LD "this is a string"
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_STRING, iec_datatype: NULL#]
        [string: this is a string]

[0033] LD 'a'
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_STRING, iec_datatype: NULL#]
        [string: a]

[0034] LD 'AaBb'
    [code: 1(0x01)[LD], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_STRING, iec_datatype: NULL#]
        [string: AaBb]

[0035] CAL CTU_1 (RESET:=PHY#IX3.6, PVv_5:=Limit, _aCU:=145, _sTR_:="string")
    [code: 20(0x14)[CAL], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_CAL, iec_datatype: NULL#]
    [ RESET (lit_dataformat: LIT_PHY, iec_datatype: PHY#) ]
        [prefix: 0[I], datatype: 0[X] phy_a: 3, phy_b: 6]
    [ PVv_5 (lit_dataformat: LIT_VAR, iec_datatype: NULL#) ]
        [variable: Limit]
    [ _aCU (lit_dataformat: LIT_INTEGER, iec_datatype: NULL#) ]
        [integer: 145]
    [ _sTR_ (lit_dataformat: LIT_STRING, iec_datatype: NULL#) ]
        [string: string]

[0036] END
    [code: 31(0x1f)[END], conditional: 0, negate: 0, push: 0, lit_dataformat: LIT_NONE, iec_datatype: NULL#]

[lines = 37]
--------------------------------------------
```
