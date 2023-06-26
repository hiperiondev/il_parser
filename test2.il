          LD %i0.0
          ST %mb0
          EQ %m3.0
          JMPC end
          LDN bool#true
          ST %m1.0
          eq %m3.0
          JMPC end
while:    LD 450           ; A
          EQ %m1.0         ; A == B
          JMPNC endwhile   ; while(A != B)
          LD UINT#16#a5
          LT %m1.0         ; A < B
          jmpc reverse
          LD 23.6
          SUB %m1.0        ; A - B
          ST %m0.0         ; A = A - B
          JMP while
reverse:  LD -12e-5        ; B
          SUB %m0.0        ; B - A
          ST %md4.23       ; B = B - A
          JMP while
endwhile: LD a_Variable_23 
          ST %qw5          (* output gcd *) 
end:      ST %q0.0
          LD TIME_OF_DAY#11:36:15.20
          LD TIME#1h_15m_30s_60ms
          LD TIME#15m_30s
          CAL FUN_3 (var1:=TIME#1h_15m_30s_60ms, PV:=DATE_AND_TIME#2001-04-09-11:36:15.20, CU:=-12e6, DT:=DATE#2001-04-09)
          LD TIME#18ms
          LD DATE#2001-04-09
          LD DATE_AND_TIME#2001-04-09-11:36:15.20
          LD "this is a string"
          LD 'a'
          LD 'AaBb'
          
          CAL CTU_1 (RESET:=%IX3.6, PVv_5:=Limit, _aCU:=145, _sTR_:="str_test")
          CAL FUNC_NF (%IX3.6, Limit, 145, "string")   ; not formal
          CAL FUN_IN_OUT (RESET:=%IX3.6, PVv_5:=Limit, _aCU:=145, _sTR_:="str_test", OUT1=>FO1, OUT2=>FO2)
          ST  FUNC.IV
          otherFUNC %IX3.6, Limit, 145, "string"
          CAL FUN_EXP1 (
              RESET:=%IX3.6,
              PVv_5:=Limit,
              _aCU:=145,
              _sTR_:="str_test",
              OUT1=>FO1,
              OUT2=>FO2
          )
 lbl_1:  GEN_FUN_EXP ( ;comment
              RESET:=%IX3.6,
              PVv_5:=Limit,
              _aCU:=145,
              _sTR_:="str_test",
              OUT1=>FO1,
              OUT2=>FO2)
              
         VAR C10: CTU; END_VAR
         VAR
             C10: CTU;
             CMD_TMR: TON;
             A, B: INT;
             ELAPSED: TIME;
             OUT, ERR, TEMPL, COND: BOOL;
         END_VAR 
         
         
         
        