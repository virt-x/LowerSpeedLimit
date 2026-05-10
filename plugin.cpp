#include <xbyak/xbyak.h>

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

    uintptr_t hookAddress = REL::RelocationID(36594, 37601).address() + 0xd1;
    uintptr_t returnAddress = hookAddress + 0x1c;
    static Code patch1(returnAddress, 0.1);
    trampoline.write_branch<5>(hookAddress, (uintptr_t)patch1.getCode());
    hookAddress = REL::RelocationID(37013, 38041).address() + 0x79;
    returnAddress = hookAddress + 0x1c;
    static Code patch2(returnAddress, 0.1);
    trampoline.write_branch<5>(hookAddress, (uintptr_t)patch2.getCode());
    hookAddress = REL::RelocationID(36918, 37943).address() + 0xa9;
    returnAddress = hookAddress + 0x1c;
    static Code patch3(returnAddress, 0.1);
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
