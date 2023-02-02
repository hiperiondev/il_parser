# il_parser
IEC61131-3 IL parser

### EXAMPLE
```
--------------------------------------------
< START test1 >

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
- RESULT (26)-
----------
  [0001] LD %IX0.4 (type: NULL#, format: LIT_PHY, value: I, X, 0, 4)
  [0002] OR( %IX1.2 (type: NULL#, format: LIT_PHY, value: I, X, 1, 2)
  [0003] AND( %IX0.3 (type: NULL#, format: LIT_PHY, value: I, X, 0, 3)
  [0004] OR( %IX0.2 (type: NULL#, format: LIT_PHY, value: I, X, 0, 2)
  [0005] OR( %IX0.1 (type: NULL#, format: LIT_PHY, value: I, X, 0, 1)
  [0006] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0007] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0008] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0009] OR( %IX0.6 (type: NULL#, format: LIT_PHY, value: I, X, 0, 6)
  [0010] AND( %IX1.0 (type: NULL#, format: LIT_PHY, value: I, X, 1, 0)
  [0011] OR( %IX0.7 (type: NULL#, format: LIT_PHY, value: I, X, 0, 7)
  [0012] ORN( %IX0.5 (type: NULL#, format: LIT_PHY, value: I, X, 0, 5)
  [0013] AND( %IX0.3 (type: NULL#, format: LIT_PHY, value: I, X, 0, 3)
  [0014] OR( %IX0.2 (type: NULL#, format: LIT_PHY, value: I, X, 0, 2)
  [0015] OR( %IX0.1 (type: NULL#, format: LIT_PHY, value: I, X, 0, 1)
  [0016] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0017] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0018] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0019] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0020] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0021] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0022] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0023] POP  (type: NULL#, format: LIT_NONE, value: [null])
  [0024] S  (type: NULL#, format: LIT_NONE, value: [null])
  [0025] ST %QX0.0 (type: NULL#, format: LIT_PHY, value: Q, X, 0, 0)
  [0026] END  (type: NULL#, format: LIT_NONE, value: [null])

< END test1 >
--------------------------------------------



--------------------------------------------
< START test2 >

<START>
labels:
[0009] while:
[0019] reverse:
[0023] endwhile:
[0025] end:

program:
  [0001] LD %i0.0
  [0002] ST %mb0
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
  [0028] CAL FUN_3 (var1:=TIME#1h_15m_30s_60ms, PV:=DATE_AND_TIME#2001-04-09-11:36:15.20, CU:=-12e6, DT:=DATE#2001-04-09)
  [0029] LD TIME#18ms
  [0030] LD DATE#2001-04-09
  [0031] LD DATE_AND_TIME#2001-04-09-11:36:15.20
  [0032] LD "this is a string"
  [0033] LD "a"
  [0034] LD "AaBb"
  [0035] CAL CTU_1 (RESET:=%IX3.6, PVv_5:=Limit, _aCU:=145, _sTR_:="string")

> identify literals
> parse values

----------
- RESULT (36)-
----------
  [0001] LD %IX0.0 (type: NULL#, format: LIT_PHY, value: I, X, 0, 0)
  [0002] ST %MB0 (type: NULL#, format: LIT_PHY, value: M, B, 0, 0)
  [0003] EQ %MX3.0 (type: NULL#, format: LIT_PHY, value: M, X, 3, 0)
  [0004] JMPC 25 (type: NULL#, format: LIT_NONE, value: [null])
  [0005] LDN 1 (type: BOOL#, format: LIT_BOOLEAN, value: 1)
  [0006] ST %MX1.0 (type: NULL#, format: LIT_PHY, value: M, X, 1, 0)
  [0007] EQ %MX3.0 (type: NULL#, format: LIT_PHY, value: M, X, 3, 0)
  [0008] JMPC 25 (type: NULL#, format: LIT_NONE, value: [null])
  [0009] LD 450 (type: NULL#, format: LIT_INTEGER, value: 450)
  [0010] EQ %MX1.0 (type: NULL#, format: LIT_PHY, value: M, X, 1, 0)
  [0011] JMPCN 23 (type: NULL#, format: LIT_NONE, value: [null])
  [0012] LD A5 (type: UINT#, format: LIT_INTEGER, value: 165)
  [0013] LT %MX1.0 (type: NULL#, format: LIT_PHY, value: M, X, 1, 0)
  [0014] JMPC 19 (type: NULL#, format: LIT_NONE, value: [null])
  [0015] LD 23.6 (type: NULL#, format: LIT_REAL, value: 23.600000)
  [0016] SUB %MX1.0 (type: NULL#, format: LIT_PHY, value: M, X, 1, 0)
  [0017] ST %MX0.0 (type: NULL#, format: LIT_PHY, value: M, X, 0, 0)
  [0018] JMP 9 (type: NULL#, format: LIT_NONE, value: [null])
  [0019] LD -12E-5 (type: NULL#, format: LIT_REAL, value: -0.000120)
  [0020] SUB %MX0.0 (type: NULL#, format: LIT_PHY, value: M, X, 0, 0)
  [0021] ST %MX1.0 (type: NULL#, format: LIT_PHY, value: M, X, 1, 0)
  [0022] JMP 9 (type: NULL#, format: LIT_NONE, value: [null])
  [0023] LD a_Variable_23 (type: NULL#, format: LIT_VAR, value: a_Variable_23)
  [0024] ST %QW5 (type: NULL#, format: LIT_PHY, value: Q, W, 5, 0)
  [0025] ST %QX0.0 (type: NULL#, format: LIT_PHY, value: Q, X, 0, 0)
  [0026] LD 11:36:15.20 (type: TOD#, format: LIT_TIME_OF_DAY, value: 11, 36, 15, 20)
  [0027] LD 1H 15M 30S 60MS (type: NULL#, format: LIT_DURATION, value: 1, 15, 30, 60)
  [0028] CAL FUN_3 (var1:=TIME#1h_15m_30s_60ms, PV:=DATE_AND_TIME#2001-04-09-11:36:15.20, CU:=-12e6, DT:=DATE#2001-04-09 (type: NULL#, format: LIT_CAL, value: [function])
         [fn: FUN_3]
             [var: var1 type: NULL#, format: LIT_DURATION, value: 1, 15, 30, 60]    
             [var: PV type: DT#, format: LIT_DATE_AND_TIME, value: 2001, 4, 9, 11, 36, 15, 20]    
             [var: CU type: NULL#, format: LIT_REAL, value: -12000000.000000]    
             [var: DT type: NULL#, format: LIT_DATE, value: 2001, 4, 9]    
  [0029] LD 18MS (type: NULL#, format: LIT_DURATION, value: 0, 0, 0, 18)
  [0030] LD 2001-04-09 (type: NULL#, format: LIT_DATE, value: 2001, 4, 9)
  [0031] LD 2001-04-09-11:36:15.20 (type: DT#, format: LIT_DATE_AND_TIME, value: 2001, 4, 9, 11, 36, 15, 20)
  [0032] LD this is a string (type: NULL#, format: LIT_STRING, value: this is a string)
  [0033] LD a (type: NULL#, format: LIT_STRING, value: a)
  [0034] LD AaBb (type: NULL#, format: LIT_STRING, value: AaBb)
  [0035] CAL CTU_1 (RESET:=%IX3.6, PVv_5:=Limit, _aCU:=145, _sTR_:="string" (type: NULL#, format: LIT_CAL, value: [function])
         [fn: CTU_1]
             [var: RESET type: NULL#, format: LIT_PHY, value: I, X, 3, 6]    
             [var: PVv_5 type: NULL#, format: LIT_VAR, value: Limit]    
             [var: _aCU type: NULL#, format: LIT_INTEGER, value: 145]    
             [var: _sTR_ type: NULL#, format: LIT_STRING, value: string]    
  [0036] END  (type: NULL#, format: LIT_NONE, value: [null])

----------
- JSON -
----------

{
    "program": {
        "1": {
            "instruction": "LD",
            "code": 1,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "PHY",
                "datatype": 0,
                "dataformat": 11,
                "str": "%I0.0",
                "value": {
                    "prefix": 0,
                    "datatype": 0,
                    "phy_a": 0,
                    "phy_b": 0
                }
            }
        },
        "2": {
            "instruction": "ST",
            "code": 2,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "PHY",
                "datatype": 0,
                "dataformat": 11,
                "str": "%MB0",
                "value": {
                    "prefix": 2,
                    "datatype": 1,
                    "phy_a": 0,
                    "phy_b": 0
                }
            }
        },
        "3": {
            "instruction": "EQ",
            "code": 15,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "PHY",
                "datatype": 0,
                "dataformat": 11,
                "str": "%M3.0",
                "value": {
                    "prefix": 2,
                    "datatype": 0,
                    "phy_a": 3,
                    "phy_b": 0
                }
            }
        },
        "4": {
            "instruction": "JMP",
            "code": 19,
            "conditional": 1,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "NONE",
                "datatype": 0,
                "dataformat": 15,
                "value": {
                    "none": 0
                }
            }
        },
        "5": {
            "instruction": "LD",
            "code": 1,
            "conditional": 0,
            "negate": 1,
            "push": 0,
            "argument": {
                "datatype_str": "BOOL",
                "dataformat_str": "BOOLEAN",
                "datatype": 1,
                "dataformat": 0,
                "str": "1",
                "value": {
                    "bool": 1
                }
            }
        },
        "6": {
            "instruction": "ST",
            "code": 2,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "PHY",
                "datatype": 0,
                "dataformat": 11,
                "str": "%M1.0",
                "value": {
                    "prefix": 2,
                    "datatype": 0,
                    "phy_a": 1,
                    "phy_b": 0
                }
            }
        },
        "7": {
            "instruction": "EQ",
            "code": 15,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "PHY",
                "datatype": 0,
                "dataformat": 11,
                "str": "%M3.0",
                "value": {
                    "prefix": 2,
                    "datatype": 0,
                    "phy_a": 3,
                    "phy_b": 0
                }
            }
        },
        "8": {
            "instruction": "JMP",
            "code": 19,
            "conditional": 1,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "NONE",
                "datatype": 0,
                "dataformat": 15,
                "value": {
                    "none": 0
                }
            }
        },
        "9": {
            "instruction": "LD",
            "code": 1,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "INTEGER",
                "datatype": 0,
                "dataformat": 5,
                "str": "450",
                "value": {
                    "integer": 450
                }
            }
        },
        "10": {
            "instruction": "EQ",
            "code": 15,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "PHY",
                "datatype": 0,
                "dataformat": 11,
                "str": "%M1.0",
                "value": {
                    "prefix": 2,
                    "datatype": 0,
                    "phy_a": 1,
                    "phy_b": 0
                }
            }
        },
        "11": {
            "instruction": "JMP",
            "code": 19,
            "conditional": 1,
            "negate": 1,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "NONE",
                "datatype": 0,
                "dataformat": 15,
                "value": {
                    "none": 0
                }
            }
        },
        "12": {
            "instruction": "LD",
            "code": 1,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "UINT",
                "dataformat_str": "INTEGER",
                "datatype": 6,
                "dataformat": 5,
                "str": "A5",
                "value": {
                    "integer": 165
                }
            }
        },
        "13": {
            "instruction": "LT",
            "code": 18,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "PHY",
                "datatype": 0,
                "dataformat": 11,
                "str": "%M1.0",
                "value": {
                    "prefix": 2,
                    "datatype": 0,
                    "phy_a": 1,
                    "phy_b": 0
                }
            }
        },
        "14": {
            "instruction": "JMP",
            "code": 19,
            "conditional": 1,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "NONE",
                "datatype": 0,
                "dataformat": 15,
                "value": {
                    "none": 0
                }
            }
        },
        "15": {
            "instruction": "LD",
            "code": 1,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "REAL",
                "datatype": 0,
                "dataformat": 6,
                "str": "23.6",
                "value": {
                    "real": 23.600000000000001
                }
            }
        },
        "16": {
            "instruction": "SUB",
            "code": 10,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "PHY",
                "datatype": 0,
                "dataformat": 11,
                "str": "%M1.0",
                "value": {
                    "prefix": 2,
                    "datatype": 0,
                    "phy_a": 1,
                    "phy_b": 0
                }
            }
        },
        "17": {
            "instruction": "ST",
            "code": 2,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "PHY",
                "datatype": 0,
                "dataformat": 11,
                "str": "%M0.0",
                "value": {
                    "prefix": 2,
                    "datatype": 0,
                    "phy_a": 0,
                    "phy_b": 0
                }
            }
        },
        "18": {
            "instruction": "JMP",
            "code": 19,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "NONE",
                "datatype": 0,
                "dataformat": 15,
                "value": {
                    "none": 0
                }
            }
        },
        "19": {
            "instruction": "LD",
            "code": 1,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "REAL",
                "datatype": 0,
                "dataformat": 6,
                "str": "-12E-5",
                "value": {
                    "real": -0.00012
                }
            }
        },
        "20": {
            "instruction": "SUB",
            "code": 10,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "PHY",
                "datatype": 0,
                "dataformat": 11,
                "str": "%M0.0",
                "value": {
                    "prefix": 2,
                    "datatype": 0,
                    "phy_a": 0,
                    "phy_b": 0
                }
            }
        },
        "21": {
            "instruction": "ST",
            "code": 2,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "PHY",
                "datatype": 0,
                "dataformat": 11,
                "str": "%M1.0",
                "value": {
                    "prefix": 2,
                    "datatype": 0,
                    "phy_a": 1,
                    "phy_b": 0
                }
            }
        },
        "22": {
            "instruction": "JMP",
            "code": 19,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "NONE",
                "datatype": 0,
                "dataformat": 15,
                "value": {
                    "none": 0
                }
            }
        },
        "23": {
            "instruction": "LD",
            "code": 1,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "VAR",
                "datatype": 0,
                "dataformat": 13,
                "str": "a_Variable_23",
                "value": {
                    "variable": "a_Variable_23"
                }
            }
        },
        "24": {
            "instruction": "ST",
            "code": 2,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "PHY",
                "datatype": 0,
                "dataformat": 11,
                "str": "%QW5",
                "value": {
                    "prefix": 1,
                    "datatype": 2,
                    "phy_a": 5,
                    "phy_b": 0
                }
            }
        },
        "25": {
            "instruction": "ST",
            "code": 2,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "PHY",
                "datatype": 0,
                "dataformat": 11,
                "str": "%Q0.0",
                "value": {
                    "prefix": 1,
                    "datatype": 0,
                    "phy_a": 0,
                    "phy_b": 0
                }
            }
        },
        "26": {
            "instruction": "LD",
            "code": 1,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "TOD",
                "dataformat_str": "TIME_OF_DAY",
                "datatype": 18,
                "dataformat": 3,
                "str": "11:36:15.20",
                "value": {
                    "msec": 20,
                    "sec": 15,
                    "min": 36,
                    "hour": 11
                }
            }
        },
        "27": {
            "instruction": "LD",
            "code": 1,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "DURATION",
                "datatype": 0,
                "dataformat": 1,
                "str": "1H 15M 30S 60MS",
                "value": {
                    "msec": 60,
                    "sec": 30,
                    "min": 15,
                    "hour": 1
                }
            }
        },
        "28": {
            "instruction": "CAL",
            "code": 20,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "CAL",
                "datatype": 0,
                "dataformat": 14,
                "function": "FUN_3",
                "variables": {
                    "var1": {
                        "str": "TIME#1h_15m_30s_60ms",
                        "datatype": 0,
                        "dataformat": 1,
                        "value": {
                            "msec": 60,
                            "sec": 30,
                            "min": 15,
                            "hour": 1
                        }
                    },
                    "PV": {
                        "str": "DATE_AND_TIME#2001-04-09-11:36:15.20",
                        "datatype": 19,
                        "dataformat": 4,
                        "value": {
                            "msec": 20,
                            "sec": 15,
                            "min": 36,
                            "hour": 11,
                            "day": 9,
                            "month": 4,
                            "year": 2001
                        }
                    },
                    "CU": {
                        "str": "-12e6",
                        "datatype": 0,
                        "dataformat": 6,
                        "value": {
                            "real": -12000000
                        }
                    },
                    "DT": {
                        "str": "DATE#2001-04-09",
                        "datatype": 0,
                        "dataformat": 2,
                        "value": {
                            "day": 9,
                            "month": 4,
                            "year": 2001
                        }
                    }
                }
            }
        },
        "29": {
            "instruction": "LD",
            "code": 1,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "DURATION",
                "datatype": 0,
                "dataformat": 1,
                "str": "18MS",
                "value": {
                    "msec": 18,
                    "sec": 0,
                    "min": 0,
                    "hour": 0
                }
            }
        },
        "30": {
            "instruction": "LD",
            "code": 1,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "DATE",
                "datatype": 0,
                "dataformat": 2,
                "str": "2001-04-09",
                "value": {
                    "day": 9,
                    "month": 4,
                    "year": 2001
                }
            }
        },
        "31": {
            "instruction": "LD",
            "code": 1,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "DT",
                "dataformat_str": "DATE_AND_TIME",
                "datatype": 19,
                "dataformat": 4,
                "str": "2001-04-09-11:36:15.20",
                "value": {
                    "msec": 20,
                    "sec": 15,
                    "min": 36,
                    "hour": 11,
                    "day": 9,
                    "month": 4,
                    "year": 2001
                }
            }
        },
        "32": {
            "instruction": "LD",
            "code": 1,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "STRING",
                "datatype": 0,
                "dataformat": 12,
                "str": "this is a string",
                "value": {
                    "string": "this is a string"
                }
            }
        },
        "33": {
            "instruction": "LD",
            "code": 1,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "STRING",
                "datatype": 0,
                "dataformat": 12,
                "str": "a",
                "value": {
                    "string": "a"
                }
            }
        },
        "34": {
            "instruction": "LD",
            "code": 1,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "STRING",
                "datatype": 0,
                "dataformat": 12,
                "str": "AaBb",
                "value": {
                    "string": "AaBb"
                }
            }
        },
        "35": {
            "instruction": "CAL",
            "code": 20,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "CAL",
                "datatype": 0,
                "dataformat": 14,
                "function": "CTU_1",
                "variables": {
                    "RESET": {
                        "str": "%IX3.6",
                        "datatype": 0,
                        "dataformat": 11,
                        "value": {
                            "prefix": 0,
                            "datatype": 0,
                            "phy_a": 3,
                            "phy_b": 6
                        }
                    },
                    "PVv_5": {
                        "str": "Limit",
                        "datatype": 0,
                        "dataformat": 13,
                        "value": {
                            "variable": "Limit"
                        }
                    },
                    "_aCU": {
                        "str": "145",
                        "datatype": 0,
                        "dataformat": 5,
                        "value": {
                            "integer": 145
                        }
                    },
                    "_sTR_": {
                        "str": "string",
                        "datatype": 0,
                        "dataformat": 12,
                        "value": {
                            "string": "string"
                        }
                    }
                }
            }
        },
        "36": {
            "instruction": "END",
            "code": 31,
            "conditional": 0,
            "negate": 0,
            "push": 0,
            "argument": {
                "datatype_str": "NULL",
                "dataformat_str": "NONE",
                "datatype": 0,
                "dataformat": 15,
                "str": "",
                "value": {
                    "none": 0
                }
            }
        }
    }
}

< END test2 >
--------------------------------------------
```
