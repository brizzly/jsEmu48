/*
 *     /
 *    /__  ___  ___  ____
 *   /  / /  / /__/ / / / /  /
 *  /  / /__/ /__  /   / /__/
 *      /
 *     /    version 0.9.0
 *
 * Copyright 2002 Daniel Nilsson
 *
 * This file is part of hpemu.
 *
 * Hpemu is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Hpemu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hpemu; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <string.h>
#include "types.h"
#include "cpu.h"
#include "bus.h"
#include "keyboard.h"
#include "opcodes.h"
#include "opinline.h"

//		      P  WP  XS   X   S   M   B   W
static int fs[16] = { 0,  0,  2,  0, 15,  3,  0,  0,
		      0,  0,  2,  0, 15,  3,  0,  0 };
static int fl[16] = { 1,  1,  1,  3,  1, 12,  2, 16,
		      1,  1,  1,  3,  1, 12,  2, 16 };
//			      0	 1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
static const int regr[16] = { A, B, C, D, B, C, A, C, A, B, C, D, B, C, A, C };
static const int regs[16] = { B, C, A, C, A, B, C, D, B, C, A, C, A, B, C, D };
static const int regt[16] = { A, B, C, D, A, B, C, D, B, C, A, C, A, B, C, D };
static const int regu[16] = { A, B, C, D, B, C, A, C, B, C, A, C, A, B, C, D };
static const int regv[16] = { B, C, A, C, A, B, C, D, A, B, C, D, B, C, A, C };

#define REGr(i)		(cpu.reg[regr[i]])
#define REGs(i)		(cpu.reg[regs[i]])
#define REGt(i)		(cpu.reg[regt[i]])
#define REGu(i)		(cpu.reg[regu[i]])
#define REGv(i)		(cpu.reg[regv[i]])
#define REGrF(i,f)	(REGr(i) + fs[f])
#define REGsF(i,f)	(REGs(i) + fs[f])
#define REGtF(i,f)	(REGt(i) + fs[f])
#define REGuF(i,f)	(REGu(i) + fs[f])
#define REGvF(i,f)	(REGv(i) + fs[f])

static __inline void update_fields(void)
{
    fs[0] = fs[8] = cpu.p;
    fl[1] = fl[9] = cpu.p + 1;
}

static void op00(byte *opc) // RTNSXM
{
    cpu.hst |= HST_XM;
    cpu.pc = rstk_pop();
    cpu.cycles += 9;
}

static void op01(byte *opc) // RTN
{
    cpu.pc = rstk_pop();
    cpu.cycles += 9;
}

static void op02_3(byte *opc) // RTNSC / RTNCC
{
    cpu.carry = (opc[1] & 1) ? FALSE : TRUE;
    cpu.pc = rstk_pop();
    cpu.cycles += 9;
}

static void op04_5(byte *opc) // SETHEX / SETDEC
{
    cpu.dec = (opc[1] & 1) ? TRUE : FALSE;
    cpu.pc += 2;
    cpu.cycles += 3;
}

static void op06(byte *opc) // RSTK=C
{
    rstk_push(nib_to_unsigned(cpu.reg[C], 5));
    cpu.pc += 2;
    cpu.cycles += 8;
}

static void op07(byte *opc) // C=RSTK
{
    unsigned_to_nib(cpu.reg[C], rstk_pop(), 5);
    cpu.pc += 2;
    cpu.cycles += 8;
}

static void op08(byte *opc) // CLRST
{
    reg_zero(cpu.st, 3);
    cpu.pc += 2;
    cpu.cycles += 5;
}

static void op09(byte *opc) // C=ST
{
    reg_cpy(cpu.reg[C], cpu.st, 3);
    cpu.pc += 2;
    cpu.cycles += 5;
}

static void op0A(byte *opc) // ST=C
{
    reg_cpy(cpu.st, cpu.reg[C], 3);
    cpu.pc += 2;
    cpu.cycles += 5;
}

static void op0B(byte *opc) // CSTEX
{
    reg_ex(cpu.reg[C], cpu.st, 3);
    cpu.pc += 2;
    cpu.cycles += 5;
}

static void op0C(byte *opc) // P=P+1
{
    if (cpu.p != 0xF) {
	cpu.p++;
	cpu.carry = FALSE;
    } else {
	cpu.p = 0;
	cpu.carry = TRUE;
    }
    update_fields();
    cpu.pc += 2;
    cpu.cycles += 3;
}

static void op0D(byte *opc) // P=P-1
{
    if (cpu.p) {
	cpu.p--;
	cpu.carry = FALSE;
    } else {
	cpu.p = 0xF;
	cpu.carry = TRUE;
    }
    update_fields();
    cpu.pc += 2;
    cpu.cycles += 3;
}

static void op0E (byte *opc) // r=r&s f/A / r=r!s f/A
{
    int len = (opc[2] == 0xF) ? 5 : fl[opc[2]];
    if (!(opc[3]&8)) {
	alu_and(REGrF(opc[3], opc[2]), REGsF(opc[3], opc[2]), len);
    } else {
	alu_or(REGrF(opc[3], opc[2]), REGsF(opc[3], opc[2]), len);
    }
    cpu.pc += 4;
    cpu.cycles += 4 + len;
}

static void op0F(byte *opc) // RTI
{
    // TODO: Implement RTI
    cpu.inte = TRUE;
    cpu.pc = rstk_pop();
    cpu.cycles += 9;
    if ((cpu.keyintp && cpu.keyscan) || kbd_on) {
	cpu.keyintp = FALSE;
	cpu_interrupt ();
    }
}

static void op10(byte *opc) // Ri=r
{
    int i = (opc[2] & 7) > 4 ? opc[2] & 3 : opc[2] & 7;
    reg_cpy(cpu.reg_r[i], cpu.reg[(opc[2]&8)>>2], 16);
    cpu.pc += 3;
    cpu.cycles += 19;
}

static void op11(byte *opc) // r=Ri
{
    int i = (opc[2] & 7) > 4 ? opc[2] & 3 : opc[2] & 7;
    reg_cpy(cpu.reg[(opc[2]&8)>>2], cpu.reg_r[i], 16);
    cpu.pc += 3;
    cpu.cycles += 19;
}

static void op12(byte *opc) // rRiEX
{
    int i = (opc[2] & 7) > 4 ? opc[2] & 3 : opc[2] & 7;
    reg_ex(cpu.reg[(opc[2]&8)>>2], cpu.reg_r[i], 16);
    cpu.pc += 3;
    cpu.cycles += 19;
}

static void op13a(byte *opc) // Di=r / rDiEX
{
    address tmp = cpu.d[opc[2]&1];
    cpu.d[opc[2]&1] = nib_to_unsigned(cpu.reg[(opc[2]&4)>>1], 5);
    if (opc[2]&2) unsigned_to_nib(cpu.reg[(opc[2]&4)>>1], tmp, 5);
    cpu.pc += 3;
    cpu.cycles += 8;
}

static void op13b(byte *opc) // Di=rS / rDiEXS
{
    address tmp = cpu.d[opc[2]&1];
    cpu.d[opc[2]&1] &= 0xF0000;
    cpu.d[opc[2]&1] |= nib_to_unsigned(cpu.reg[(opc[2]&4)>>1], 4);
    if (opc[2]&2) unsigned_to_nib(cpu.reg[(opc[2]&4)>>1], tmp, 4);
    cpu.pc += 3;
    cpu.cycles += 7;
}

static void op14(byte *opc) // DATi=r A/B / r=DATi A/B
{
    if (!(opc[2]&2)) {
	bus_write(cpu.reg[(opc[2]&4)>>1], cpu.d[opc[2]&1], (opc[2]&8) ? 2 : 5);
    } else {
	bus_read(cpu.reg[(opc[2]&4)>>1], cpu.d[opc[2]&1], (opc[2]&8) ? 2 : 5);
	cpu.cycles++;
    }
    cpu.pc += 3;
    cpu.cycles += (opc[2]&8) ? 14 : 17;
}

static void op15a(byte *opc) // DATi=r f / r=DATi f
{
    if (!(opc[2]&2)) {
	bus_write(cpu.reg[(opc[2]&4)>>1] + fs[opc[3]], cpu.d[opc[2]&1], fl[opc[3]]);
    } else {
	bus_read(cpu.reg[(opc[2]&4)>>1] + fs[opc[3]], cpu.d[opc[2]&1], fl[opc[3]]);
	cpu.cycles++;
    }
    cpu.pc += 4;
    cpu.cycles += 16 + fl[opc[3]];
}

static void op15b(byte *opc) // DATi=r n / r=DATi n
{
    if (!(opc[2]&2)) {
	bus_write(cpu.reg[(opc[2]&4)>>1], cpu.d[opc[2]&1], opc[3]+1);
    } else {
	bus_read(cpu.reg[(opc[2]&4)>>1], cpu.d[opc[2]&1], opc[3]+1);
	cpu.cycles++;
    }
    cpu.pc += 4;
    cpu.cycles += 15 + opc[3]+1;
}

static void op16_7(byte *opc) // Di=Di+
{
    cpu.d[opc[1]&1] += opc[2] + 1;
    if (cpu.d[opc[1]&1] & ~0xFFFFF) {
	cpu.d[opc[1]&1] &= 0xFFFFF;
	cpu.carry = TRUE;
    } else {
	cpu.carry = FALSE;
    }
    cpu.pc += 3;
    cpu.cycles += 7;
}

static void op18_C(byte *opc) // Di=Di-
{
    cpu.d[(opc[1]>>2)&1] -= opc[2] + 1;
    if (cpu.d[(opc[1]>>2)&1] & ~0xFFFFF) {
	cpu.d[(opc[1]>>2)&1] &= 0xFFFFF;
	cpu.carry = TRUE;
    } else {
	cpu.carry = FALSE;
    }
    cpu.pc += 3;
    cpu.cycles += 7;
}

static void op19_D(byte *opc) // Di=(2)
{
    cpu.d[(opc[1]>>2)&1] &= 0xFFF00;
    cpu.d[(opc[1]>>2)&1] |= nib_to_unsigned(opc+2, 2);
    cpu.pc += 4;
    cpu.cycles += 4;
}

static void op1A_E(byte *opc) // Di=(4)
{
    cpu.d[(opc[1]>>2)&1] &= 0xF0000;
    cpu.d[(opc[1]>>2)&1] |= nib_to_unsigned(opc+2, 4);
    cpu.pc += 6;
    cpu.cycles += 6;
}

static void op1B_F(byte *opc) // Di=(5)
{
    cpu.d[(opc[1]>>2)&1] = nib_to_unsigned(opc+2, 5);
    cpu.pc += 7;
    cpu.cycles += 7;
}

static void op2(byte *opc) // P=
{
    cpu.p = opc[1];
    update_fields();
    cpu.pc += 2;
    cpu.cycles += 2;
}

static void op3(byte *opc) // LC
{
    load(cpu.reg[C], opc+2, cpu.p, opc[1]+1);
    cpu.pc += 2 + opc[1]+1;
    cpu.cycles += 3 + opc[1]+1;
}

static void op4(byte *opc) // GOC
{
    goyes(opc, 1);
    cpu.cycles += 3;
}

static void op5(byte *opc) // GONC
{
    cpu.carry = !cpu.carry;
    goyes (opc, 1);
    cpu.carry = !cpu.carry;
    cpu.cycles += 3;
}

static void op6(byte *opc) // GOTO
{
    cpu.pc += nib_to_signed(opc+1, 3) + 1;
    cpu.cycles += 11;
}

static void op7(byte *opc) // GOSUB
{
    rstk_push(cpu.pc + 4);
    cpu.pc += nib_to_signed(opc+1, 3) + 4;
    cpu.cycles += 12;
}

static void op800_1(byte *opc) // OUT=CS / OUT=C
{
    reg_cpy(cpu.out, cpu.reg[C], opc[2] ? 3 : 1);
    cpu.pc += 3;
    cpu.cycles += opc[2] ? 6 : 4;
    kbd_out_changed();
}

static void op802_3(byte *opc) // r=IN
{
    reg_cpy(cpu.reg[(opc[2]&1)<<1], cpu.in, 4);
    if (!(cpu.pc & 1)) cpu.pc += 3; // Saturn bug!
    cpu.cycles += 7;
}

static void op804(byte *opc) // UNCNFG
{
    bus_unconfigure(nib_to_unsigned(cpu.reg[C], 5));
    cpu.pc += 3;
    cpu.cycles += 12;
}

static void op805(byte *opc) // CONFIG
{
    bus_configure(nib_to_unsigned(cpu.reg[C], 5));
    cpu.pc += 3;
    cpu.cycles += 11;
}

static void op806(byte *opc) // C=ID
{
    unsigned_to_nib(cpu.reg[C], bus_get_id(), 5);
    cpu.pc += 3;
    cpu.cycles += 11;
}

static void op807(byte *opc) // SHUTND
{
    // TODO: Fix SHUTDN
    if (!cpu.in[0] && !cpu.in[1] && !cpu.in[3]) {
	cpu.shutdown = TRUE;
    }
    cpu.pc += 3;
    cpu.cycles += 5;
}

static void op8080(byte *opc) // INTON
{
    cpu.keyscan = TRUE;
    cpu.pc += 4;
    cpu.cycles += 5;
    if (cpu.keyintp) {
	cpu.keyintp = FALSE;
	cpu_interrupt ();
    }
}

static void op8081(byte *opc) // RSI
{
    // TODO: Implement RSI
    // Note: Is the opcode 80810?
    // if (opc[4] == 0)
    cpu.pc += 5;
    cpu.cycles += 6;
}

static void op8082(byte *opc) // LA
{
    load(cpu.reg[A], opc+5, cpu.p, opc[4]+1);
    cpu.pc += 5 + opc[4]+1;
    cpu.cycles += 6 + opc[4]+1;
}

static void op8084_5_8_9(byte *opc) // rBIT=b
{
    reg_bit(cpu.reg[(opc[3]&8)>>2], opc[4], opc[3]&1);
    cpu.pc += 5;
    cpu.cycles += 6;
}

static void op8086_7_A_B (byte *opc) // ?rBIT=b
{
    comp_bit_zero(cpu.reg[(opc[3]&8)>>2], opc[4]);
    if (opc[3] & 1) cpu.carry = !cpu.carry;
    goyes(opc, 5);
    cpu.cycles += 9;
}

static void op808C_E(byte *opc) // PC=(r)
{
    byte tmp[5];
    bus_read(tmp, nib_to_unsigned(cpu.reg[opc[3]&2], 5), 5); 
    cpu.pc = nib_to_unsigned(tmp, 5);
    cpu.cycles += 23;
}

static void op808F(byte *opc) // INTOFF
{
    // TODO: Implement INTOFF
    cpu.keyscan = FALSE;
    cpu.pc += 4;
    cpu.cycles += 5;
}

static void op809(byte *opc) // C+P+1
{
    alu_add_con(cpu.reg[C], cpu.p, 0, 5);
    cpu.pc += 3;
    cpu.cycles += 8;
}

static void op80A(byte *opc) // RESET
{
    bus_reset();
    cpu.pc += 3;
    cpu.cycles += 6;
}

static void op80C(byte *opc) // C=P
{
    cpu.reg[C][opc[3]] = cpu.p;
    cpu.pc += 4;
    cpu.cycles += 6;
}

static void op80D(byte *opc) // P=C
{
    cpu.p = cpu.reg[C][opc[3]];
    update_fields();
    cpu.pc += 4;
    cpu.cycles += 6;
}

static void op80F(byte *opc) // CPEX
{
    byte tmp = cpu.reg[C][opc[3]];
    cpu.reg[C][opc[3]] = cpu.p;
    cpu.p = tmp;
    update_fields();
    cpu.pc += 4;
    cpu.cycles += 6;
}

static void op810_3(byte *opc) // rSLC
{
    alu_slc(cpu.reg[opc[2]&3], 16);
    cpu.pc += 3;
    cpu.cycles += 21;
}

static void op814_7(byte *opc) // rSRC
{
    alu_src(cpu.reg[opc[2]&3], 16);
    cpu.pc += 3;
    cpu.cycles += 21;
}

static void op818 (byte *opc) // r=r+CON f/A / r=r-CON f/A
{
    int len = (opc[3] == 0xF) ? 5 : ((fl[opc[3]] == 1) ? 17 : fl[opc[3]]);
    // Note: What happens if opc[4]&4
    if (!(opc[4]&8)) {
	alu_add_con(cpu.reg[opc[4]&3], opc[5], fs[opc[3]], len);
    } else {
	alu_sub_con(cpu.reg[opc[4]&3], opc[5], fs[opc[3]], len);
    }
    cpu.pc += 6;
    cpu.cycles += 5 + len;
}

static void op819(byte *opc) // rSRB f/A
{
    int len = (opc[3] == 0xF) ? 5 : fl[opc[3]];
    alu_srb(cpu.reg[opc[4]&3] + fs[opc[3]], len);
    cpu.pc += 5;
    cpu.cycles += 20;
}

static void op81Af0(byte *opc) // Ri=r f/A
{
    int i = (opc[5] & 7) > 4 ? opc[5] & 3 : opc[5] & 7;
    int len = (opc[3] == 0xF) ? 5 : fl[opc[3]];
    reg_cpy(cpu.reg_r[i] + fs[opc[3]], cpu.reg[(opc[5]&8)>>2] + fs[opc[3]], len);
    cpu.pc += 6;
    cpu.cycles += 6 + len;
}

static void op81Af1(byte *opc) // r=Ri f/A
{
    int i = (opc[5] & 7) > 4 ? opc[5] & 3 : opc[5] & 7;
    int len = (opc[3] == 0xF) ? 5 : fl[opc[3]];
    reg_cpy(cpu.reg[(opc[5]&8)>>2] + fs[opc[3]], cpu.reg_r[i] + fs[opc[3]], len);
    cpu.pc += 6;
    cpu.cycles += 6 + len;
}

static void op81Af2(byte *opc) // rRiEX f/A
{
    int i = (opc[5] & 7) > 4 ? opc[5] & 3 : opc[5] & 7;
    int len = (opc[3] == 0xF) ? 5 : fl[opc[3]];
    reg_ex(cpu.reg[(opc[5]&8)>>2] + fs[opc[3]], cpu.reg_r[i] + fs[opc[3]], len);
    cpu.pc += 6;
    cpu.cycles += 6 + len;
}

static void op81B2_3(byte *opc) // PC=r
{
    cpu.pc = nib_to_unsigned(cpu.reg[(opc[3]&1)<<1], 5);
    cpu.cycles += 16;
}

static void op81B4_5(byte *opc) // r=PC
{
    cpu.pc += 4;
    unsigned_to_nib(cpu.reg[(opc[3]&1)<<1], cpu.pc, 5);
    cpu.cycles += 9;
}

static void op81C_F(byte *opc) // rSRB
{
    alu_srb(cpu.reg[opc[2]&3], 16);
    cpu.pc += 3;
    cpu.cycles += 20;
}

static void op82(byte *opc) // HST=0
{
    cpu.hst &= ~opc[2];
    cpu.pc += 3;
    cpu.cycles += 3;
}

static void op83(byte *opc) // ?HST=0
{
    cpu.carry = (cpu.hst & opc[2]) ? FALSE : TRUE;
    goyes(opc, 3);
    cpu.cycles += 6;
}

static void op84_5(byte *opc) // ST=b
{
    reg_bit(cpu.st, opc[2], opc[1] & 1);
    cpu.pc += 3;
    cpu.cycles += 4;
}

static void op86_7(byte *opc) // ?ST=b
{
    comp_bit_zero(cpu.st, opc[2]);
    if (opc[1] & 1) cpu.carry = !cpu.carry;
    goyes(opc, 3);
    cpu.cycles += 7;
}

static void op88_9(byte *opc) // ?P# / ?P=
{
    cpu.carry = (cpu.p == opc[2]) == (opc[1]&1) ? TRUE : FALSE;
    goyes(opc, 3);
    cpu.cycles += 6;
}

static void op8A(byte *opc) // ?u=v A / ?u#v A / ?u=0 A / ?u#0 A
{
    if (!(opc[2] & 8)) {
	comp_eq(REGu(opc[2]), REGv(opc[2]), 5);
    } else {
	comp_zero (cpu.reg[opc[2]&3], 5);
    }
    if (opc[2] & 4) cpu.carry = !cpu.carry;
    goyes(opc, 3);
    cpu.cycles += 11;
}

static void op8B(byte *opc) // ?u>v A / u<v A / u>=v A / u<=v A
{
    comp_gt(REGu(opc[2]), REGv(opc[2]), 5);
    if (opc[2] & 8) cpu.carry = !cpu.carry;
    goyes(opc, 3);
    cpu.cycles += 11;
}

static void op8C(byte *opc) // GOLONG
{
    cpu.pc += nib_to_signed(opc+2, 4) + 2;
    cpu.cycles += 14;
}

static void op8D(byte *opc) // GOVLNG
{
    cpu.pc = nib_to_unsigned(opc+2, 5);
    cpu.cycles += 14;
}

static void op8E(byte *opc) // GOSUBL
{
    rstk_push(cpu.pc + 6);
    cpu.pc += nib_to_signed(opc+2, 4) + 6;
    cpu.cycles += 14;
}

static void op8F(byte *opc) // GOSBVL
{
    rstk_push(cpu.pc + 7);
    cpu.pc = nib_to_unsigned(opc+2, 5);
    cpu.cycles += 15;
}

static void op9a(byte *opc) // ?u=v f / ?u#v f / ?u=0 f / ?u#0 f
{
    if (!(opc[2] & 8)) {
	comp_eq(REGuF(opc[2], opc[1]), REGvF(opc[2], opc[1]), fl[opc[1]]);
    } else {
	comp_zero(cpu.reg[opc[2]&3] + fs[opc[1]], fl[opc[1]]);
    }
    if (opc[2] & 4) cpu.carry = !cpu.carry;
    goyes(opc, 3);
    cpu.cycles += 6 + fl[opc[1]];
}

static void op9b(byte *opc) // ?u>v f / u<v f / u>=v f / u<=v f
{
    comp_gt(REGuF(opc[2], opc[1]), REGvF(opc[2], opc[1]), fl[opc[1]]);
    if (opc[2] & 8) cpu.carry = !cpu.carry;
    goyes(opc, 3);
    cpu.cycles += 6 + fl[opc[1]];
}

static void opAa(byte *opc) // t=t+v f / t=t-1 f 
{
    switch (opc[2] & 0xC) {
    case 0x0:
    case 0x4:
    case 0x8:
	alu_add(REGtF(opc[2], opc[1]), REGvF(opc[2], opc[1]), fl[opc[1]]);
	break;
    case 0xC:
	alu_dec(REGtF(opc[2], opc[1]), fl[opc[1]]);
	break;
    }
    cpu.pc += 3;
    cpu.cycles += 3 + fl[opc[1]];
}

static void opAb(byte *opc) // t=0 f / t=r f / trEX f
{
    switch (opc[2] & 0xC) {
    case 0x0:
	reg_zero(REGtF(opc[2], opc[1]), fl[opc[1]]);
	break;
    case 0x4:
    case 0x8:
	reg_cpy(REGtF(opc[2], opc[1]), REGrF(opc[2], opc[1]), fl[opc[1]]);
	break;
    case 0xC:
	reg_ex(REGtF(opc[2], opc[1]), REGrF(opc[2], opc[1]), fl[opc[1]]);
	break;
    }
    cpu.pc += 3;
    cpu.cycles += 3 + fl[opc[1]];
}

static void opBa(byte *opc) // t=t-v f / t=t+1 f / t=v-t f 
{
    switch (opc[2] & 0xC) {
    case 0x0:
    case 0x8:
	alu_sub(REGtF(opc[2], opc[1]), REGvF(opc[2], opc[1]), fl[opc[1]]);
	break;
    case 0x4:
	alu_inc(REGtF(opc[2], opc[1]), fl[opc[1]]);
	break;
    case 0xC:
	alu_sub2(REGtF(opc[2], opc[1]), REGvF(opc[2], opc[1]), fl[opc[1]]);
	break;
    }
    cpu.pc += 3;
    cpu.cycles += 3 + fl[opc[1]];
}

static void opBb0_3(byte *opc) // rSL f
{
    alu_sl(cpu.reg[opc[2]&3] + fs[opc[1]], fl[opc[1]]);
    cpu.pc += 3;
    cpu.cycles += 3 + fl[opc[1]];
}

static void opBb4_7(byte *opc) // rSR f
{
    alu_sr(cpu.reg[opc[2]&3] + fs[opc[1]], fl[opc[1]]);
    cpu.pc += 3;
    cpu.cycles += 3 + fl[opc[1]];
}

static void opBb8_B(byte *opc) // r=-r f
{
    alu_neg(cpu.reg[opc[2]&3] + fs[opc[1]], fl[opc[1]]);
    cpu.pc += 3;
    cpu.cycles += 3 + fl[opc[1]];
}

static void opBbC_F(byte *opc) // r=-r-1 f
{
    alu_not(cpu.reg[opc[2]&3] + fs[opc[1]], fl[opc[1]]);
    cpu.pc += 3;
    cpu.cycles += 3 + fl[opc[1]];
}

static void opC(byte *opc) // t=t+v A / t=t-1 A
{
    switch (opc[1] & 0xC) {
    case 0x0:
    case 0x4:
    case 0x8:
	alu_add(REGt(opc[1]), REGv(opc[1]), 5);
	break;
    case 0xC:
	alu_dec(REGt(opc[1]), 5);
	break;
    }
    cpu.pc += 2;
    cpu.cycles += 7;
}

static void opD(byte *opc) // t=0 A / t=r A / trEX A
{
    switch (opc[1] & 0xC) {
    case 0x0:
	reg_zero(REGt(opc[1]), 5);
	break;
    case 0x4:
    case 0x8:
	reg_cpy(REGt(opc[1]), REGr(opc[1]), 5);
	break;
    case 0xC:
	reg_ex(REGt(opc[1]), REGr(opc[1]), 5);
	break;
    }
    cpu.pc += 2;
    cpu.cycles += 7;
}

static void opE(byte *opc) // t=t-v A / t=t+1 A / t=v-t A
{
    switch (opc[1] & 0xC) {
    case 0x0:
    case 0x8:
	alu_sub(REGt(opc[1]), REGv(opc[1]), 5);
	break;
    case 0x4:
	alu_inc(REGt(opc[1]), 5);
	break;
    case 0xC:
	alu_sub2(REGt(opc[1]), REGv(opc[1]), 5);
	break;
    }
    cpu.pc += 2;
    cpu.cycles += 7;
}

static void opF0_3(byte *opc) // rSL f
{
    alu_sl(cpu.reg[opc[1]&3], 5);
    cpu.pc += 2;
    cpu.cycles += 7;
}

static void opF4_7(byte *opc) // rSR f
{
    alu_sr(cpu.reg[opc[1]&3], 5);
    cpu.pc += 2;
    cpu.cycles += 7;
}

static void opF8_B(byte *opc) // r=-r A
{
    alu_neg(cpu.reg[opc[1]&3], 5);
    cpu.pc += 2;
    cpu.cycles += 7;
}

static void opFC_F(byte *opc) // r=-r-1 A
{
    alu_not(cpu.reg[opc[1]&3], 5);
    cpu.pc += 2;
    cpu.cycles += 7;
}

static Opcode opcodes0[16] = {
    { op00,     NULL,	    "RTNSXM" },
    { op01,     NULL,	    "RTN" },
    { op02_3,   NULL,	    "RTNSC" },
    { op02_3,   NULL,	    "RTNCC" },
    { op04_5,   NULL,	    "SETHEX" },
    { op04_5,   NULL,	    "SETDEC" },
    { op06,     NULL,	    "RSTK=C" },
    { op07,     NULL,	    "C=RSTK" },
    { op08,     NULL,	    "CLRST" },
    { op09,     NULL,	    "C=ST" },
    { op0A,     NULL,	    "ST=C" },
    { op0B,     NULL,	    "CSTEX" },
    { op0C,     NULL,	    "P=P+1" },
    { op0D,     NULL,	    "P=P-1" },
    { op0E,     NULL,	    "%r3=%r3%&3%s3 %F2A" },
    { op0F,     NULL,	    "RTI" }
};

static Opcode opcodes13[16] = {
    { op13a,    NULL,	    "D0=A" },
    { op13a,    NULL,	    "D1=A" },
    { op13a,    NULL,	    "AD0EX" },
    { op13a,    NULL,	    "AD1EX" },
    { op13a,    NULL,	    "D0=C" },
    { op13a,    NULL,	    "D1=C" },
    { op13a,    NULL,	    "CD0EX" },
    { op13a,    NULL,	    "CD1EX" },
    { op13b,    NULL,	    "D0=AS" },
    { op13b,    NULL,	    "D1=AS" },
    { op13b,    NULL,	    "AD0EXS" },
    { op13b,    NULL,	    "AD1EXS" },
    { op13b,    NULL,	    "D0=CS" },
    { op13b,    NULL,	    "D1=CS" },
    { op13b,    NULL,	    "CD0EXS" },
    { op13b,    NULL,	    "CD1EXS" }
};

static Opcode opcodes15[16] = {
    { op15a,    NULL,	    "%D2 %F3" },
    { op15a,    NULL,	    "%D2 %F3" },
    { op15a,    NULL,	    "%D2 %F3" },
    { op15a,    NULL,	    "%D2 %F3" },
    { op15a,    NULL,	    "%D2 %F3" },
    { op15a,    NULL,	    "%D2 %F3" },
    { op15a,    NULL,	    "%D2 %F3" },
    { op15a,    NULL,	    "%D2 %F3" },
    { op15b,    NULL,	    "%D2 %I3+1" },
    { op15b,    NULL,	    "%D2 %I3+1" },
    { op15b,    NULL,	    "%D2 %I3+1" },
    { op15b,    NULL,	    "%D2 %I3+1" },
    { op15b,    NULL,	    "%D2 %I3+1" },
    { op15b,    NULL,	    "%D2 %I3+1" },
    { op15b,    NULL,	    "%D2 %I3+1" },
    { op15b,    NULL,	    "%D2 %I3+1" }
};

static Opcode opcodes1[16] = {
    { op10,     NULL,	    "R%i2=%a2" },
    { op11,     NULL,	    "%a2=R%i2" },
    { op12,     NULL,	    "%a2R%i2EX" },
    { NULL,     opcodes13,  NULL },
    { op14,     NULL,	    "%D2 %B2" },
    { NULL,     opcodes15,  NULL },
    { op16_7,   NULL,	    "D0=D0+ %I2+1" },
    { op16_7,   NULL,	    "D1=D1+ %I2+1" },
    { op18_C,   NULL,	    "D0=D0- %I2+1" },
    { op19_D,   NULL,	    "D0=(2) %X22" },
    { op1A_E,   NULL,	    "D0=(4) %X24" },
    { op1B_F,   NULL,	    "D0=(5) %X25" },
    { op18_C,   NULL,	    "D1=D1- %I2+1" },
    { op19_D,   NULL,	    "D1=(2) %X22" },
    { op1A_E,   NULL,	    "D1=(4) %X24" },
    { op1B_F,   NULL,	    "D1=(5) %X25" },
};

static Opcode opcodes808[16] = {
    { op8080,   NULL,	    "INTON" },
    { op8081,   NULL,	    "RSI" },
    { op8082,   NULL,	    "LA #%N4" },
    { NULL,     NULL,	    NULL },
    { op8084_5_8_9, NULL,   "ABIT=0 %I4" },
    { op8084_5_8_9, NULL,   "ABIT=1 %I4" },
    { op8086_7_A_B, NULL,   "?ABIT=0 %I4, %G52YES %T52+5" },
    { op8086_7_A_B, NULL,   "?ABIT=1 %I4, %G52YES %T52+5" },
    { op8084_5_8_9, NULL,   "CBIT=0 %I4" },
    { op8084_5_8_9, NULL,   "CBIT=1 %I4" },
    { op8086_7_A_B, NULL,   "?CBIT=0 %I4, %G52YES %T52+5" },
    { op8086_7_A_B, NULL,   "?CBIT=1 %I4, %G52YES %T52+5" },
    { op808C_E, NULL,	    "PC=(A)" },
    { NULL,     NULL,	    NULL },
    { op808C_E, NULL,	    "PC=(C)" },
    { op808F,   NULL,	    "INTOFF" }
};

static Opcode opcodes80[16] = {
    { op800_1,  NULL,	    "OUT=CS" },
    { op800_1,  NULL,	    "OUT=C" },
    { op802_3,  NULL,	    "A=IN" },
    { op802_3,  NULL,	    "C=IN" },
    { op804,    NULL,	    "UNCONFIG" },
    { op805,    NULL,	    "CONFIG" },
    { op806,    NULL,	    "C=ID" },
    { op807,    NULL,	    "SHUTDN" },
    { NULL,     opcodes808, NULL },
    { op809,    NULL,	    "C+P+1" },
    { op80A,    NULL,	    "RESET" },
    { NULL,     NULL,	    NULL },
    { op80C,    NULL,	    "C=P %I3" },
    { op80D,    NULL,	    "P=C %I3" },
    { NULL,     NULL,	    NULL },
    { op80F,    NULL,	    "CPEX %I3" }
};

static Opcode opcodes81Af[16] = {
    { op81Af0,  NULL,	    "R%i5=%a5 %F3A" },
    { op81Af1,  NULL,	    "%a5=R%i5 %F3A" },
    { op81Af2,  NULL,	    "%a5R%i5EX %F3A" },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL }
};

static Opcode opcodes81A[16] = {
    { NULL,     opcodes81Af,NULL },
    { NULL,     opcodes81Af,NULL },
    { NULL,     opcodes81Af,NULL },
    { NULL,     opcodes81Af,NULL },
    { NULL,     opcodes81Af,NULL },
    { NULL,     opcodes81Af,NULL },
    { NULL,     opcodes81Af,NULL },
    { NULL,     opcodes81Af,NULL },
    { NULL,     opcodes81Af,NULL },
    { NULL,     opcodes81Af,NULL },
    { NULL,     opcodes81Af,NULL },
    { NULL,     opcodes81Af,NULL },
    { NULL,     opcodes81Af,NULL },
    { NULL,     opcodes81Af,NULL },
    { NULL,     opcodes81Af,NULL },
    { NULL,     opcodes81Af,NULL }
};

static Opcode opcodes81B[16] = {
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { op81B2_3, NULL,	    "PC=A" },
    { op81B2_3, NULL,	    "PC=C" },
    { op81B4_5, NULL,	    "A=PC" },
    { op81B4_5, NULL,	    "C=PC" },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL },
    { NULL,     NULL,	    NULL }
};

static Opcode opcodes81[16] = {
    { op810_3,  NULL,	    "ASLC" },
    { op810_3,  NULL,	    "BSLC" },
    { op810_3,  NULL,	    "CSLC" },
    { op810_3,  NULL,	    "DSLC" },
    { op814_7,  NULL,	    "ASRC" },
    { op814_7,  NULL,	    "BSRC" },
    { op814_7,  NULL,	    "CSRC" },
    { op814_7,  NULL,	    "DSRC" },
    { op818,    NULL,	    "%u4=%u4%~4CON %F3A, %I6+1" },
    { op819,    NULL,	    "%u4SRB %F3A" },
    { NULL,     opcodes81A, NULL },
    { NULL,     opcodes81B, NULL },
    { op81C_F,  NULL,	    "ASRB" },
    { op81C_F,  NULL,	    "BSRB" },
    { op81C_F,  NULL,	    "CSRB" },
    { op81C_F,  NULL,	    "DSRB" }
};

static Opcode opcodes8[16] = {
    { NULL,     opcodes80,  NULL },
    { NULL,     opcodes81,  NULL },
    { op82,     NULL,	    "HST=0 #%X21" },
    { op83,     NULL,	    "?HST=0 #%X21, %G32YES %T32+3" },
    { op84_5,   NULL,	    "ST=0 %I2" },
    { op84_5,   NULL,	    "ST=1 %I2" },
    { op86_7,   NULL,	    "?ST=0 %I2, %G32YES %T32+3" },
    { op86_7,   NULL,	    "?ST=1 %I2, %G32YES %T32+3" },
    { op88_9,   NULL,	    "?P# %I2, %G32YES %T32+3" },
    { op88_9,   NULL,	    "?P= %I2, %G32YES %T32+3" },
    { op8A,     NULL,	    "?%u2%#2%z2 A, %G32YES %T32+3" },
    { op8B,     NULL,	    "?%u2%>2%v2 A, %G32YES %T32+3" },
    { op8C,     NULL,	    "GOLONG %R24+2" },
    { op8D,     NULL,	    "GOVLNG %X25" },
    { op8E,     NULL,	    "GOSUBL %R24+6" },
    { op8F,     NULL,	    "GOSBVL %X25" },
};

static Opcode opcodes9[16] = {
    { op9a,     NULL,	    "?%u2%#2%z2 %F1, %G32YES %T32+3" },
    { op9a,     NULL,	    "?%u2%#2%z2 %F1, %G32YES %T32+3" },
    { op9a,     NULL,	    "?%u2%#2%z2 %F1, %G32YES %T32+3" },
    { op9a,     NULL,	    "?%u2%#2%z2 %F1, %G32YES %T32+3" },
    { op9a,     NULL,	    "?%u2%#2%z2 %F1, %G32YES %T32+3" },
    { op9a,     NULL,	    "?%u2%#2%z2 %F1, %G32YES %T32+3" },
    { op9a,     NULL,	    "?%u2%#2%z2 %F1, %G32YES %T32+3" },
    { op9a,     NULL,	    "?%u2%#2%z2 %F1, %G32YES %T32+3" },
    { op9b,     NULL,	    "?%u2%>2%v2 %F1, %G32YES %T32+3" },
    { op9b,     NULL,	    "?%u2%>2%v2 %F1, %G32YES %T32+3" },
    { op9b,     NULL,	    "?%u2%>2%v2 %F1, %G32YES %T32+3" },
    { op9b,     NULL,	    "?%u2%>2%v2 %F1, %G32YES %T32+3" },
    { op9b,     NULL,	    "?%u2%>2%v2 %F1, %G32YES %T32+3" },
    { op9b,     NULL,	    "?%u2%>2%v2 %F1, %G32YES %T32+3" },
    { op9b,     NULL,	    "?%u2%>2%v2 %F1, %G32YES %T32+3" },
    { op9b,     NULL,	    "?%u2%>2%v2 %F1, %G32YES %T32+3" }
};

static Opcode opcodesA[16] = {
    { opAa,     NULL,	    "%t2=%t2%+2%w2 %F1" },
    { opAa,     NULL,	    "%t2=%t2%+2%w2 %F1" },
    { opAa,     NULL,	    "%t2=%t2%+2%w2 %F1" },
    { opAa,     NULL,	    "%t2=%t2%+2%w2 %F1" },
    { opAa,     NULL,	    "%t2=%t2%+2%w2 %F1" },
    { opAa,     NULL,	    "%t2=%t2%+2%w2 %F1" },
    { opAa,     NULL,	    "%t2=%t2%+2%w2 %F1" },
    { opAa,     NULL,	    "%t2=%t2%+2%w2 %F1" },
    { opAb,     NULL,	    "%x2%=2%y2%E2 %F1" },
    { opAb,     NULL,	    "%x2%=2%y2%E2 %F1" },
    { opAb,     NULL,	    "%x2%=2%y2%E2 %F1" },
    { opAb,     NULL,	    "%x2%=2%y2%E2 %F1" },
    { opAb,     NULL,	    "%x2%=2%y2%E2 %F1" },
    { opAb,     NULL,	    "%x2%=2%y2%E2 %F1" },
    { opAb,     NULL,	    "%x2%=2%y2%E2 %F1" },
    { opAb,     NULL,	    "%x2%=2%y2%E2 %F1" }
};

static Opcode opcodesBb[16] = {
    { opBb0_3,  NULL,	    "ASL %F1" },
    { opBb0_3,  NULL,	    "BSL %F1" },
    { opBb0_3,  NULL,	    "CSL %F1" },
    { opBb0_3,  NULL,	    "DSL %F1" },
    { opBb4_7,  NULL,	    "ASR %F1" },
    { opBb4_7,  NULL,	    "BSR %F1" },
    { opBb4_7,  NULL,	    "CSR %F1" },
    { opBb4_7,  NULL,	    "DSR %F1" },
    { opBb8_B,  NULL,	    "A=-A %F1" },
    { opBb8_B,  NULL,	    "B=-B %F1" },
    { opBb8_B,  NULL,	    "C=-C %F1" },
    { opBb8_B,  NULL,	    "D=-D %F1" },
    { opBbC_F,  NULL,	    "A=-A-1 %F1" },
    { opBbC_F,  NULL,	    "B=-B-1 %F1" },
    { opBbC_F,  NULL,	    "C=-C-1 %F1" },
    { opBbC_F,  NULL,	    "D=-D-1 %F1" }
};

static Opcode opcodesB[16] = {
    { opBa,     NULL,	    "%t2=%p2%-2%q2 %F1" },
    { opBa,     NULL,	    "%t2=%p2%-2%q2 %F1" },
    { opBa,     NULL,	    "%t2=%p2%-2%q2 %F1" },
    { opBa,     NULL,	    "%t2=%p2%-2%q2 %F1" },
    { opBa,     NULL,	    "%t2=%p2%-2%q2 %F1" },
    { opBa,     NULL,	    "%t2=%p2%-2%q2 %F1" },
    { opBa,     NULL,	    "%t2=%p2%-2%q2 %F1" },
    { opBa,     NULL,	    "%t2=%p2%-2%q2 %F1" },
    { NULL,     opcodesBb,  NULL },
    { NULL,     opcodesBb,  NULL },
    { NULL,     opcodesBb,  NULL },
    { NULL,     opcodesBb,  NULL },
    { NULL,     opcodesBb,  NULL },
    { NULL,     opcodesBb,  NULL },
    { NULL,     opcodesBb,  NULL },
    { NULL,     opcodesBb,  NULL }
};

static Opcode opcodesF[16] = {
    { opF0_3,   NULL,	    "ASL A" },
    { opF0_3,   NULL,	    "BSL A" },
    { opF0_3,   NULL,	    "CSL A" },
    { opF0_3,   NULL,	    "DSL A" },
    { opF4_7,   NULL,	    "ASR A" },
    { opF4_7,   NULL,	    "BSR A" },
    { opF4_7,   NULL,	    "CSR A" },
    { opF4_7,   NULL,	    "DSR A" },
    { opF8_B,   NULL,	    "A=-A A" },
    { opF8_B,   NULL,	    "B=-B A" },
    { opF8_B,   NULL,	    "C=-C A" },
    { opF8_B,   NULL,	    "D=-D A" },
    { opFC_F,   NULL,	    "A=-A-1 A" },
    { opFC_F,   NULL,	    "B=-B-1 A" },
    { opFC_F,   NULL,	    "C=-C-1 A" },
    { opFC_F,   NULL,	    "D=-D-1 A" },
};

Opcode opcodes[16] = {
    { NULL,	opcodes0,   NULL },
    { NULL,     opcodes1,   NULL },
    { op2,      NULL,	    "P= %I1" },
    { op3,      NULL,	    "LC #%N1" },
    { op4,      NULL,	    "%G12C %T12+1" },
    { op5,      NULL,	    "%G12NC %T12+1" },
    { op6,      NULL,	    "GOTO %R13+1" },
    { op7,      NULL,	    "GOSUB %R13+4" },
    { NULL,     opcodes8,   NULL },
    { NULL,     opcodes9,   NULL },
    { NULL,     opcodesA,   NULL },
    { NULL,     opcodesB,   NULL },
    { opC,      NULL,	    "%t1=%t1%+1%w1 A" },
    { opD,      NULL,	    "%x1%=1%y1%E1 A" },
    { opE,      NULL,	    "%t1=%p1%-1%q1 A" },
    { NULL,     opcodesF,   NULL }
};
