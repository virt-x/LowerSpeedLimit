#include <xbyak/xbyak.h>

//idk if this does anything
struct SpeedLimit {
    static UINT64 GetSpeedMult(RE::Character* a_actor) { 
        float value = a_actor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kSpeedMult);
        value *= 0.01;
        if (value < 0.1)
        {
            value = 0.1;
        }
        return (UINT64)value & 0x7fffffffffffffff;
    }

    static UINT64 FirstHook(RE::Character* a_actor) { return GetSpeedMult(a_actor); }
    static inline REL::Relocation<decltype(GetSpeedMult)> _FirstHook;
    static UINT64 SecondHook(RE::Character* a_actor) { return GetSpeedMult(a_actor); }
    static inline REL::Relocation<decltype(GetSpeedMult)> _SecondHook;
    static UINT64 ThirdHook(RE::Character* a_actor) { return GetSpeedMult(a_actor); }
    static inline REL::Relocation<decltype(GetSpeedMult)> _ThirdHook;
    static UINT64 FourthHook(RE::Character* a_actor) { return GetSpeedMult(a_actor); }
    static inline REL::Relocation<decltype(GetSpeedMult)> _FourthHook;
};

struct Code : Xbyak::CodeGenerator
{
    Code(uintptr_t returnAddress, float minimumSpeed, float mult = 0.01) {
        mov(rax, *(uint32_t*)&mult);
        push(rax);
        movss(xmm0, ptr[rsp]);
        add(rsp, 8);
        mulss(xmm2, xmm0);
        mov(rax, *(uint32_t*)&minimumSpeed);
        push(rax);
        movss(xmm0, ptr[rsp]);
        add(rsp, 8);
        maxss(xmm2, xmm0);
        mov(rax, returnAddress);
        jmp(rax);
    }
};

void InstallHook() {
    auto& trampoline = SKSE::GetTrampoline();
    SpeedLimit::_FirstHook = trampoline.write_call<5>(
        REL::RelocationID(37608, 38561).address() + 0x103,
        reinterpret_cast<uintptr_t>(SpeedLimit::FirstHook));
    SpeedLimit::_SecondHook = trampoline.write_call<5>(REL::RelocationID(37631, 38584).address() + 0x2b,
                                                      reinterpret_cast<uintptr_t>(SpeedLimit::SecondHook));
    SpeedLimit::_ThirdHook = trampoline.write_call<5>(
        REL::RelocationID(37633, 38586).address() + 0x7cd,
        reinterpret_cast<uintptr_t>(SpeedLimit::ThirdHook));
    SpeedLimit::_FourthHook = trampoline.write_call<5>(
        REL::RelocationID(37633, 38586).address() + 0xc1a,
        reinterpret_cast<uintptr_t>(SpeedLimit::FourthHook));

    uintptr_t hookAddress = REL::RelocationID(36594, 37601).address() + 0xd1;
    uintptr_t returnAddress = hookAddress + 0x1c;
    static Code patch1(returnAddress, 0.05);
    trampoline.write_branch<5>(hookAddress, (uintptr_t)patch1.getCode());
    hookAddress = REL::RelocationID(37013, 38041).address() + 0x79;
    returnAddress = hookAddress + 0x1c;
    static Code patch2(returnAddress, 0.05);
    trampoline.write_branch<5>(hookAddress, (uintptr_t)patch2.getCode());
    hookAddress = REL::RelocationID(36918, 37943).address() + 0xa9;
    returnAddress = hookAddress + 0x1c;
    static Code patch3(returnAddress, 0.05);
    trampoline.write_branch<5>(hookAddress, (uintptr_t)patch3.getCode());
}

void OnMessage(SKSE::MessagingInterface::Message* message) {
    if (message->type == SKSE::MessagingInterface::kDataLoaded) {
        InstallHook();
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    SKSE::Init(skse);
    auto& trampoline = SKSE::GetTrampoline();
    trampoline.create(1 << 10);
    auto* messagingInterface = SKSE::GetMessagingInterface();
    messagingInterface->RegisterListener(OnMessage);
    return true;
}
