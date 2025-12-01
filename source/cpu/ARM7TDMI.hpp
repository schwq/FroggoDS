#pragma once

#include "../Common.hpp"

namespace Froggo
{
    class ARM7TDMI;

    enum class ARM7_OpMode
    {
        // TODO: Should we implement the old modes for the ARMv3 compatibility>
        USR = 0x10,
        FIQ = 0x11,
        IRQ = 0x12,
        SVC = 0x13,
        ABT = 0x17,
        UND = 0x1B,
        SYS = 0x1F
    };

    enum class ARM7_CpuFlag
    {
        Sign = 31,
        Zero = 30,
        Carry = 29,
        Overflow = 28
    };

    enum class ARM7_ControlBits
    {
        Endian = 9,
        Abort_Disable = 8,
        IRQ_Disable = 7,
        FIQ_Disable = 6,
        State_Bit = 5
    };

    struct ARM7_RegisterSet
    {
        u32 R[16];
        u32 R_fiq[7];
        u32 R_svc[2];
        u32 R_abt[2];
        u32 R_irq[2];
        u32 R_und[2];
        u32 CPSR;
        u32 SPSR_fiq;
        u32 SPSR_svc;
        u32 SPSR_abt;
        u32 SPSR_irq;
        u32 SPSR_und;

        u32 GetReg(ARM7TDMI *arm, int n)
        {
            ARM7_OpMode opmode = arm->GetOpMode();
            // TODO:
        }

        u32 PC()
        {
            return R[15]; // The same in all modes
        }

        bool GetFlag(ARM7_CpuFlag flag)
        {
            return BIT(CPSR, static_cast<int>(flag));
        }
        void SetFlag(ARM7_CpuFlag flag, bool on)
        {
            BIT_SET(CPSR, static_cast<int>(flag), on);
        }

        bool GetControlBit(ARM7_ControlBits bit)
        {
            return BIT(CPSR, static_cast<int>(bit));
        }
        void SetControlBit(ARM7_ControlBits bit, bool on)
        {
            BIT_SET(CPSR, static_cast<int>(bit), on);
        }
        ARM7_OpMode GetOpMode()
        {
            switch (CPSR & 0x1F)
            {
            case 0x10:
                return ARM7_OpMode::USR;
            case 0x11:
                return ARM7_OpMode::FIQ;
            case 0x12:
                return ARM7_OpMode::IRQ;
            case 0x13:
                return ARM7_OpMode::SVC;
            case 0x17:
                return ARM7_OpMode::ABT;
            case 0x1B:
                return ARM7_OpMode::UND;
            case 0x1F:
                return ARM7_OpMode::SYS;
            default:
                throw std::runtime_error("Invalid CPSR mode bits!");
            }
        }
        void SetOpMode(ARM7_OpMode mode)
        {
            CPSR &= ~0xF1;
            CPSR |= static_cast<int>(mode) & 0xF1;
        }
    };

    enum class ARM7_ExceptionType
    {
        Reset,
        Undefined_Inst,
        Software_Int, // SWI
        Prefetch_Abort,
        Data_Abort,
        Address_Exceeds_26bit,
        Normal_Int, // IRQ
        Fast_Int,   // FIQ
        Undefined
    };

    struct ARM7_Exception
    {
        ARM7_ExceptionType Type = ARM7_ExceptionType::Undefined;
        u32 Address = 0;
        unsigned int Priority = 0; // 0 means nothing Priority for simultaneously occuring exceptions ranges from Prio=1=Highest to Prio=7=Lowest.
        ARM7_OpMode ModeOnEntry = ARM7_OpMode::UND;
        std::string_view InterruptFlags = "--"; // I first and F second, values can be 1 0 or u for unchanged
    };

    static const std::array<ARM7_Exception, 8> sARM7_Exceptions = {
        ARM7_Exception{ARM7_ExceptionType::Reset, 0x00, 1, ARM7_OpMode::SVC, "11"},
        ARM7_Exception{ARM7_ExceptionType::Undefined_Inst, 0x04, 7, ARM7_OpMode::UND, "1u"},
        ARM7_Exception{ARM7_ExceptionType::Software_Int, 0x08, 6, ARM7_OpMode::SVC, "1u"},
        ARM7_Exception{ARM7_ExceptionType::Prefetch_Abort, 0x0C, 5, ARM7_OpMode::ABT, "1u"},
        ARM7_Exception{ARM7_ExceptionType::Data_Abort, 0x10, 2, ARM7_OpMode::ABT, "1u"},
        ARM7_Exception{ARM7_ExceptionType::Address_Exceeds_26bit, 0x14, 0, ARM7_OpMode::SVC, "1u"},
        ARM7_Exception{ARM7_ExceptionType::Normal_Int, 0x18, 4, ARM7_OpMode::IRQ, "1u"},
        ARM7_Exception{ARM7_ExceptionType::Fast_Int, 0x1C, 3, ARM7_OpMode::FIQ, "11"}};

    enum class ARM7_CondFieldSuffix
    {
        EQ = 0, // Z=1           equal (zero) (same)
        NE,     // Z=0           not equal (nonzero) (not same)
        CS_HS,  // C=1           unsigned higher or same (carry set)
        CC_LO,  // C=0           unsigned lower (carry cleared)
        MI,     // N=1           signed negative (minus)
        PL,     // N=0           signed positive or zero (plus)
        VS,     // V=1           signed overflow (V set)
        VC,     // V=0           signed no overflow (V cleared)
        HI,     // C=1 and Z=0   unsigned higher
        LS,     // C=0 or Z=1    unsigned lower or same
        GE,     // N=V           signed greater or equal
        LT,     // N<>V          signed less than
        GT,     // Z=0 and N=V   signed greater than
        LE,     // Z=1 or N<>V   signed less or equal
        AL,     //-              always (the "AL" suffix can be omitted)
        NV      //-              never (ARMv1,v2 only) (Reserved ARMv3 and up)
    };

    enum class ARM7_State
    {
        ARM,
        THUMB
    };

    class ARM7TDMI
    {

    public:
        ARM7_OpMode GetOpMode() const;

    private:
        using Byte = u8;
        using Halfword = u16;
        using Word = u32;

        bool ChangeCpuState();
        bool ForceChangeCpuState(ARM7_State state);

        ARM7_OpMode m_OperationMode = ARM7_OpMode::UND;
        ARM7_State m_CpuState = ARM7_State::ARM;
        ARM7_RegisterSet m_RegisterSet;
    };

}