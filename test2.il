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
          ST %m1.0         ; B = B - A
          JMP while
endwhile: LD a_Variable_23 
          ST %qw5          (* output gcd *) 
end:      ST %q0.0
          LD TIME_OF_DAY#11:36:15.20
          LD TIME#1h_15m_30s_60ms
          CAL FUN_3 (var1:=TIME#1h_15m_30s_60ms, PV:=DATE_AND_TIME#2001-04-09-11:36:15.20, CU:=-12e6, DT:=DATE#2001-04-09)
          LD TIME#18ms
          LD DATE#2001-04-09
          LD DATE_AND_TIME#2001-04-09-11:36:15.20
          LD "this is a string"
          LD 'a'
          LD 'AaBb'
          CAL CTU_1 (RESET:=%IX3.6, PV:=Limit, CU:=145, STR:="string")
