#include "catch.hpp"

#include "../../src/cpu.h"
#include "../../src/op_codes.h"

using namespace Catch;

TEST_CASE("Running all the opcodes")
{
  CPU cpu;

  SECTION("All opcodes can be processed")
  {
    const uint8_t NUM_OP_CODES = 157;
    uint8_t program[NUM_OP_CODES] = { NOP, STAX_B, INR_B, DCR_B, LDX_B, INR_C, DCR_C, STAX_D, INR_D, DCR_D, LDX_D, INR_E, DCR_E, INR_H, DCR_H, DAA, INR_L, DCR_L, CMA, INR_M, DCR_M, STC, INR_A, DCR_A, CMC, MOV_B_B, MOV_B_C, MOV_B_D, MOV_B_E, MOV_B_H, MOV_B_L, MOV_B_M, MOV_B_A, MOV_C_B, MOV_C_C, MOV_C_D, MOV_C_E, MOV_C_H, MOV_C_L, MOV_C_M, MOV_C_A, MOV_D_B, MOV_D_C, MOV_D_D, MOV_D_E, MOV_D_H, MOV_D_L, MOV_D_M, MOV_D_A, MOV_E_B, MOV_E_C, MOV_E_D, MOV_E_E, MOV_E_H, MOV_E_L, MOV_E_M, MOV_E_A, MOV_H_B, MOV_H_C, MOV_H_D, MOV_H_E, MOV_H_H, MOV_H_L, MOV_H_M, MOV_H_A, MOV_L_B, MOV_L_C, MOV_L_D, MOV_L_E, MOV_L_H, MOV_L_L, MOV_L_M, MOV_L_A, MOV_M_B, MOV_M_C, MOV_M_D, MOV_M_E, MOV_M_H, MOV_M_L, MOV_M_M, MOV_M_A, MOV_A_B, MOV_A_C, MOV_A_D, MOV_A_E, MOV_A_H, MOV_A_L, MOV_A_M, MOV_A_A, ADD_B, ADD_C, ADD_D, ADD_E, ADD_H, ADD_L, ADD_M, ADD_A, ADC_B, ADC_C, ADC_D, ADC_E, ADC_H, ADC_L, ADC_M, ADC_A, SUB_B, SUB_C, SUB_D, SUB_E, SUB_H, SUB_L, SUB_M, SUB_A, SBB_B, SBB_C, SBB_D, SBB_E, SBB_H, SBB_L, SBB_M, SBB_A, ANA_B, ANA_C, ANA_D, ANA_E, ANA_H, ANA_L, ANA_M, ANA_A, XRA_B, XRA_C, XRA_D, XRA_E, XRA_H, XRA_L, XRA_M, XRA_A, ORA_B, ORA_C, ORA_D, ORA_E, ORA_H, ORA_L, ORA_M, ORA_A, CMP_B, CMP_C, CMP_D, CMP_E, CMP_H, CMP_L, CMP_M, CMP_A, RLC, RRC, RAL, RAR };

    cpu.processProgram(program, NUM_OP_CODES);
    REQUIRE(1 == 1);
  }
}
