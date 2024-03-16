#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <cstdint>
#include "c_usercmd.h"
#include "MinHook/MinHook.h"
#include "SourceConsole.h"
#if _WIN64 && _DEBUG
#pragma comment(lib, "MinHook/Debug/libMinHook.x64.lib")
#else
#pragma comment(lib, "MinHook/Release/libMinHook.x64.lib")
#endif
HMODULE g_Module = NULL;

class Ent {
public:
    int GetFlags()
    {
        return *(int*)((uintptr_t)this + 0x08C);
    }
};



class IEntityList {
public:
    Ent* GetClientEntity(int index) {
		return Call<Ent*>(this, 3, index);
	}
};



class EngineClient {
public:
    int GetLocalPlayer()
    {
        return Call<int>(this, 42);
    }
};

using CreateMoveFn = bool(__thiscall*)(void*, float, c_usercmd*);

CreateMoveFn oCreateMove;
EngineClient* engine;
IEntityList* entityList;
bool __stdcall CreateMoveHook(DWORD64* thisptr, float frameTime, c_usercmd* cmd) {
    auto r = oCreateMove(thisptr, frameTime, cmd);
    Ent* localPlayer = entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!localPlayer)
        return r;
    if (GetAsyncKeyState(VK_XBUTTON2)) {
        if (cmd->buttons & IN_JUMP && !(localPlayer->GetFlags() & FL_ONGROUND)) {
            cmd->buttons &= ~IN_JUMP;
        }
    }
    return r;

}
constexpr uintptr_t* Get(void* vmt, const std::uint32_t index) noexcept
{
    return (*static_cast<uintptr_t***>(vmt))[index];
}
#define P_CLIENTMODE 0xFB3780


int main() {
    
    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"client.dll");
    uintptr_t engineBase = (uintptr_t)GetModuleHandle(L"engine.dll");   

    engine = GetInterface<EngineClient*>("engine.dll", "VEngineClient013");
    entityList = GetInterface<IEntityList*>("client.dll", "VClientEntityList003");

    m_sourceConsole.reset(new SourceConsole());
    MH_STATUS status = MH_Initialize();
    auto client_mode = *reinterpret_cast<void**>(moduleBase + P_CLIENTMODE);
    m_sourceConsole->ColorPrint(SourceColor(0, 255, 255, 255), "[+] Bhop started \n");
    if (MH_CreateHook(Get(client_mode, 19), &CreateMoveHook, reinterpret_cast<void**>(&oCreateMove)) != MH_OK)
    {
        m_sourceConsole->ColorPrint(SourceColor(255, 0, 0, 255), "[-] Failed to hook \n");
        FreeLibraryAndExitThread(g_Module, 0);
        return 1;
	}

    MH_EnableHook(MH_ALL_HOOKS);

    while (true) {
        
        Sleep(100);

        engine = GetInterface<EngineClient*>("engine.dll", "VEngineClient013");
        entityList = GetInterface<IEntityList*>("client.dll", "VClientEntityList003");
        
        if(GetAsyncKeyState(VK_DELETE))
			break;

    }

    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    m_sourceConsole->ColorPrint(SourceColor(255, 0, 0,255), "[+] Unhooked \n");


    FreeLibraryAndExitThread(g_Module, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
           g_Module = hModule;
           CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

