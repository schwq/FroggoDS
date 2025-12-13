#include "ARM7TDMI.hpp"
#include <cstdio>
#include <stdexcept>
#include <type_traits>

using namespace Froggo;

void ARM7TDMI::Execute()
{
  u32 instruction = 0;

  switch (m_CpuState) {
    case ARM7_State::ARM:
      ExecuteARMState();
      break;
    case Froggo::ARM7_State::THUMB:
      ExecuteThumbState();
      break;
  }
}

void ARM7TDMI::ExecuteARMState()
{
  ARM7_ARMInstruction inst;
  u32 instruction = 0;

  if (!CheckInstructionCondition(inst)) {
    // Skip
  }

  if (TEST(instruction, 27, 25, 0b000) && TEST(instruction, 7, 4, 0b1001)) {
    u8 opcode = BITS(instruction, 24, 21);
    // MUL, MLA Multiply instruction
    ExARM_Multiply(inst);
  }

  if (TEST(instruction, 27, 25, 0b000) && (BIT(instruction, 7) == 1) &&
      BIT(instruction, 4) == 1) {
    // Halfword and Signed Data trasfer
    // STRH, LDRH, LDRSB, LDRSH
    ExARM_SignedDataTransfer(inst);
  }

  // @TODO: Check for the 7-4 bits, because the ARMv4T only supports BX, and not BXJ, and BLX
  if (TEST(instruction, 27, 8, 0b00010010111111111111) &&
      TEST(instruction, 7, 4, 0b0001)) {
    ExARM_BX(inst);
  }

  if (TEST(instruction, 27, 26, 0b00) && TEST(instruction, 24, 23, 0b10) &&
      (BIT(instruction, 20) == 0)) {
    // PSR Transfer MRS, MSR
    ExARM_PSRTransfer(inst);
  }

  if (TEST(instruction, 27, 26, 0b00) && BIT(instruction, 7) == 0) {

    ExARM_ALU(inst);
  }

  if (TEST(instruction, 27, 26, 0b01)) {
    // Single Data transfer LDR, STR
    ExARM_SigleDataTransfer(inst);
  }

  if (TEST(instruction, 27, 25, 0b100)) {
    // LDM, STM
    // Memory Block Data Transfer
    ExARM_BlockDataTransfer(inst);
  }

  if (TEST(instruction, 27, 25, 0b101)) {
    u8 opcode = BIT(instruction, 24);
    opcode ? ExARM_BL(inst) : ExARM_B(inst);
  }

  if (TEST(instruction, 27, 24, 0b1111)) {
    // SWI {cond}
    ExARM_SWI(inst);
  }

  if (TEST(instruction, 27, 23, 0b00010) && TEST(instruction, 21, 20, 0b00) &&
      TEST(instruction, 11, 4, 0b00001001)) {
    // SWP Single Data Swap (ARMv7 is optional)
    u8 opcode = BIT(instruction, 22);
    opcode ? ExARM_SWP(inst) : ExARM_SWPB(inst);
  }

  if (TEST(instruction, 27, 25, 0b011)) {
    // Undefined instruction
    ExARM_UND(inst);
  }
}

void ARM7TDMI::ExARM_SWP(const ARM7_ARMInstruction& instruction) {}
void ARM7TDMI::ExARM_SWPB(const ARM7_ARMInstruction& instruction) {}
void ARM7TDMI::ExARM_UND(const ARM7_ARMInstruction& instruction) {}

void ARM7TDMI::ExARM_ALU(const ARM7_ARMInstruction& instruction)
{
  const u8 bits = BITS(instruction.data, 24, 21);
  const ARM7_ALUOpcode opcode = static_cast<ARM7_ALUOpcode>(bits);
  const bool immediateOpFlag =
      BIT(instruction.data, 25);  // 0 = Register , 1 = Immediate
  const u8 rd = BITS(instruction.data, 15, 12);
  const u8 rn = BITS(instruction.data, 19, 16);
  const u8 rm = BITS(instruction.data, 3, 0);
  const bool setConditionCodes = BIT(instruction.data, 20);  // 0 = No, 1 = Yes
  const bool shiftRegisterFlag = BIT(instruction.data, 4);

  u8 value = 0;
  if (immediateOpFlag) {
    // op2 Immediate value
    const u8 rotate = BITS(instruction.data, 11, 8);
    const u8 nn = BITS(instruction.data, 7, 0);
  }
  else {
    // op2 Register
    const u8 shiftType = BITS(instruction.data, 6, 5);

    if (shiftRegisterFlag) {  // Shift by register
      const u8 rs = BITS(instruction.data, 11, 8);
    }
    else {  // Shift by immediate
      const u8 is = BITS(instruction.data, 11, 7);
    }
  }

  switch (opcode) {
    case ARM7_ALUOpcode::AND:
    {

    } break;
    case ARM7_ALUOpcode::EOR:
    {

    } break;
    case ARM7_ALUOpcode::SUB:
    {

    } break;
    case ARM7_ALUOpcode::RSB:
    {

    } break;
    case ARM7_ALUOpcode::ADD:
    {

    } break;
    case ARM7_ALUOpcode::ADC:
    {

    } break;
    case ARM7_ALUOpcode::SBC:
    {

    } break;
    case ARM7_ALUOpcode::RSC:
    {

    } break;
    case ARM7_ALUOpcode::TST:
    {
      if (rd != 0 || rd != 0xF)
        return;
      if (!immediateOpFlag)
        return;
    } break;
    case ARM7_ALUOpcode::TEQ:
    {
      if (rd != 0 || rd != 0xF)
        return;
      if (!immediateOpFlag)
        return;
    } break;
    case ARM7_ALUOpcode::CMP:
    {
      if (rd != 0 || rd != 0xF)
        return;
      if (!immediateOpFlag)
        return;
    } break;
    case ARM7_ALUOpcode::CMN:
    {
      if (rd != 0 || rd != 0xF)
        return;
      if (!immediateOpFlag)
        return;
    } break;
    case ARM7_ALUOpcode::ORR:
    {

    } break;
    case ARM7_ALUOpcode::MOV:
    {
      if (rn != 0)
        return;
    } break;
    case ARM7_ALUOpcode::BIC:
    {

    } break;
    case ARM7_ALUOpcode::MVN:
    {
      if (rn != 0)
        return;
    } break;
  }
}

bool ARM7TDMI::CheckInstructionCondition(const ARM7_ARMInstruction& instruction)
{
  ARM7_CondFieldSuffix condition =
      static_cast<ARM7_CondFieldSuffix>(ARM7_OpcodeCond(instruction.data));
  switch (condition) {
    case ARM7_CondFieldSuffix::AL:
      return true;
    case ARM7_CondFieldSuffix::EQ:
    {
      u8 z = m_RegisterSet.GetFlag(ARM7_CpuFlag::Zero);
      return z;
    }
    case ARM7_CondFieldSuffix::NE:
    {
      u8 z = m_RegisterSet.GetFlag(ARM7_CpuFlag::Zero);
      return !z;
    }
    case ARM7_CondFieldSuffix::CS_HS:
    {
      u8 c = m_RegisterSet.GetFlag(ARM7_CpuFlag::Carry);
      return c;
    }
    case ARM7_CondFieldSuffix::CC_LO:
    {
      u8 c = m_RegisterSet.GetFlag(ARM7_CpuFlag::Carry);
      return !c;
    }
    case ARM7_CondFieldSuffix::MI:
    {
      u8 n = m_RegisterSet.GetFlag(ARM7_CpuFlag::Sign);
      return n;
    }
    case ARM7_CondFieldSuffix::PL:
    {
      u8 n = m_RegisterSet.GetFlag(ARM7_CpuFlag::Sign);
      return !n;
    }
    case ARM7_CondFieldSuffix::VS:
    {
      u8 v = m_RegisterSet.GetFlag(ARM7_CpuFlag::Overflow);
      return v;
    }
    case ARM7_CondFieldSuffix::VC:
    {
      u8 v = m_RegisterSet.GetFlag(ARM7_CpuFlag::Overflow);
      return !v;
    }
    case ARM7_CondFieldSuffix::HI:
    {
      u8 c = m_RegisterSet.GetFlag(ARM7_CpuFlag::Carry);
      u8 z = m_RegisterSet.GetFlag(ARM7_CpuFlag::Zero);
      return (c && !z);
    }
    case ARM7_CondFieldSuffix::LS:
    {
      u8 c = m_RegisterSet.GetFlag(ARM7_CpuFlag::Carry);
      u8 z = m_RegisterSet.GetFlag(ARM7_CpuFlag::Zero);
      return (!c || z);
    }
    case ARM7_CondFieldSuffix::GE:
    {
      u8 n = m_RegisterSet.GetFlag(ARM7_CpuFlag::Sign);
      u8 v = m_RegisterSet.GetFlag(ARM7_CpuFlag::Overflow);
      return (n == v);
    }
    case ARM7_CondFieldSuffix::LT:
    {
      u8 n = m_RegisterSet.GetFlag(ARM7_CpuFlag::Sign);
      u8 v = m_RegisterSet.GetFlag(ARM7_CpuFlag::Overflow);
      return (n != v);
    }
    case ARM7_CondFieldSuffix::GT:
    {
      u8 n = m_RegisterSet.GetFlag(ARM7_CpuFlag::Sign);
      u8 v = m_RegisterSet.GetFlag(ARM7_CpuFlag::Overflow);
      u8 z = m_RegisterSet.GetFlag(ARM7_CpuFlag::Zero);
      return !z && (n == v);
    }
    case ARM7_CondFieldSuffix::LE:
    {
      u8 n = m_RegisterSet.GetFlag(ARM7_CpuFlag::Sign);
      u8 v = m_RegisterSet.GetFlag(ARM7_CpuFlag::Overflow);
      u8 z = m_RegisterSet.GetFlag(ARM7_CpuFlag::Zero);
      return z || (n != v);
    }
    case ARM7_CondFieldSuffix::NV:
      // Log information
      return false;
    default:
      throw std::runtime_error("Cannot find the condition for the instruction");
      return false;
  }
}
