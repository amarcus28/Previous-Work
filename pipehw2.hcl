// Adam Marcus aem2nj
// pipehw2.hcl

########## the PC and condition codes registers #############
register fF { predPC:64 = 0; }

########## PC and Status updates #############

f_predPC = [
	 f_icode in {JXX} : f_valC;
	 f_Stat != STAT_AOK : pc;
	 1 : f_valP;
];

########## Fetch #############

pc = [
     (M_icode == JXX) && (!M_Cnd) : M_valP;
     1 : F_predPC;
];
stall_F = loadUse;
bubble_D = (E_icode == JXX) && (!e_Cnd);
bubble_E = ((E_icode == JXX) && (!e_Cnd)) || loadUse;

f_icode = i10bytes[4..8];
f_ifun = i10bytes[0..4];
f_rA = i10bytes[12..16];
f_rB = i10bytes[8..12];

f_valC = [
	f_icode in { JXX } : i10bytes[8..72];
	1 : i10bytes[16..80];
];

wire offset:64;
offset = [
	f_icode in { HALT, NOP, RET } : 1;
	f_icode in { RRMOVQ, OPQ, PUSHQ, POPQ } : 2;
	f_icode in { JXX, CALL } : 9;
	1 : 10;
];

f_valP = pc + offset;

f_Stat = [
       f_icode == HALT	: STAT_HLT;
       f_icode > 0xb	: STAT_INS;
       f_icode in {NOP, HALT, IRMOVQ, RRMOVQ, OPQ, CMOVXX, RMMOVQ, MRMOVQ, JXX} : STAT_AOK;
       1       : STAT_INS;
];

wire loadUse:1;
loadUse = (E_icode in {MRMOVQ}) && (E_dstM in {reg_srcA, reg_srcB});

########## Decode #############

register fD {
	 Stat:3 = STAT_AOK;
	 icode:4 = NOP;
	 ifun:4 = ALWAYS;
	 rA:4 = REG_NONE;
	 rB:4 = REG_NONE;
	 valC:64 = 0;
	 valP:64 = 0;
}
stall_D = loadUse;

d_icode = D_icode;
d_ifun = D_ifun;
d_valC = D_valC;
d_Stat = D_Stat;
d_valP = D_valP;

# source selection
d_srcA = [
        D_icode in {RMMOVQ, RRMOVQ, OPQ} : D_rA;
        1 : REG_NONE;
];

d_srcB = [
        D_icode in {RMMOVQ, MRMOVQ, OPQ} : D_rB;
        1 : REG_NONE;
];

reg_srcA = d_srcA;
reg_srcB = d_srcB;

d_valA = [
	reg_srcA == REG_NONE: 0;
	reg_srcA == e_dstE : e_valE;
	reg_srcA == m_dstM : m_valM; # forward post-memory
	reg_srcA == W_dstM : W_valM; # forward pre-writeback
	reg_srcA == m_dstE : m_valE;
	reg_srcA == W_dstE : W_valE;
	1: reg_outputA;
];

d_valB = [
        reg_srcB == REG_NONE : 0;
	reg_srcB == e_dstE : e_valE;
        reg_srcB == m_dstM : m_valM;
	reg_srcB == W_dstM : W_valM;
	reg_srcB == m_dstE : m_valE;
	reg_srcB == W_dstE : W_valE;
	1: reg_outputB;
];

d_dstE = [
       D_icode in {IRMOVQ, RRMOVQ, OPQ} : D_rB;
       1: REG_NONE;
];

d_dstM = [ 
        D_icode in {MRMOVQ} : D_rA;
        1: REG_NONE;
];
########## Execute #############

register dE {
	 Stat:3 = STAT_AOK;
	 icode:4 = NOP;
	 ifun:4 = ALWAYS;
	 valC:64 = 0;
	 valA:64 = 0;
	 valB:64 = 0;
	 dstE:4 = REG_NONE;
	 dstM:4 = REG_NONE;
	 srcA:4 = REG_NONE;
	 srcB:4 = REG_NONE;
	 valP:64 = 0;
}
e_icode = E_icode;
e_ifun = E_ifun;
e_Stat = E_Stat;
e_dstM = E_dstM;
e_valP = E_valP;

e_valE = [
       (E_icode == OPQ) && (E_ifun == XORQ) : E_valA ^ E_valB;
       (E_icode == OPQ) && (E_ifun == ADDQ) : E_valA + E_valB;
       (E_icode == OPQ) && (E_ifun == SUBQ) : E_valB - E_valA;
       (E_icode == OPQ) && (E_ifun == ANDQ) : E_valA & E_valB;
       E_icode in {IRMOVQ} : E_valC;
       E_icode in {RRMOVQ, CMOVXX} : E_valA;
       E_icode in { MRMOVQ, RMMOVQ } : E_valC + E_valB;
       1     : 0xBADBADBAD;
];

e_valA = [
        E_icode in { RMMOVQ, MRMOVQ } : E_valA;
        1 : 0;
];

register cC {
	 SF:1 = 0;
	 ZF:1 = 1;
}
c_ZF = (e_valE == 0);
c_SF = (e_valE >= 0x8000000000000000);
stall_C = (e_icode != OPQ);

e_Cnd = [
	      e_ifun == LE : C_SF || C_ZF;
	      e_ifun == LT : C_SF && !C_ZF;
	      e_ifun == EQ : C_ZF;
	      e_ifun == NE : !C_ZF;
	      e_ifun == GE : !C_SF || C_ZF;
	      e_ifun == GT : !C_SF && !C_ZF;
	      e_ifun == ALWAYS : 1;
	      1	   : 0;
];

e_dstE = [
       (e_icode == CMOVXX) && (!e_Cnd) : REG_NONE;
       1 : E_dstE;
];

########## Memory #############

register eM {
	 Stat:3 = STAT_AOK;
	 icode:4 = NOP;
	 ifun:4 = ALWAYS;
	 Cnd:1 = 0;
	 valE:64 = 0;
	 valA:64 = 0;
	 dstE:4 = REG_NONE;
	 dstM:4 = REG_NONE;
	 valP:64 = 0;
}
m_icode = M_icode;
m_Stat = M_Stat;
m_dstM = M_dstM;
m_valE = M_valE;
m_dstE = M_dstE;


mem_readbit = M_icode in { MRMOVQ };
mem_writebit = M_icode in { RMMOVQ };
mem_addr = [
        M_icode in { MRMOVQ, RMMOVQ } : M_valE;
        1: 0xBADBADBAD;
];
mem_input = [
        M_icode in { RMMOVQ } : M_valA;
        1: 0xBADBADBAD;
];

m_valM = mem_output;

########## Writeback #############

register mW {
	 Stat:3 = STAT_AOK;
	 icode:4 = NOP;
	 valE:64 = 0;
	 dstE:4 = REG_NONE;
	 dstM:4 = REG_NONE;
	 valM:64 = 0;
}

reg_dstM = [ 
        W_icode in {MRMOVQ} : W_dstM;
        1: REG_NONE;
];

reg_inputM = [
        W_icode in {MRMOVQ} : W_valM;
        1: 0xBADBADBAD;
];

reg_inputE = [
	W_icode in {RRMOVQ, IRMOVQ, OPQ, CMOVXX} : W_valE;
        1: 0xBADBADBAD;
];

reg_dstE = W_dstE;

Stat = W_Stat;



