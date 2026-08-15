

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <cstdint>
#include <Windows.h>

#include <memory/memory.h>
#include <safe_vector/safe_vector.h>

// Roblox offsets {OUTDATED - MUST UPDATE}
namespace Offsets {
    namespace Core {
        constexpr uint64_t render_view_offset = 0xC8;
        constexpr uint64_t DatamodelHolder = 0x128;
        constexpr uint64_t Datamodel = 0x1C0;
        constexpr uint64_t Self = 0x8;
    }
    namespace instance {
        constexpr uint64_t ClassDescriptor = 0x18;
        constexpr uint64_t Children = 0x70;
    }
    namespace humanoid {
        constexpr uint64_t health = 0x194;
        constexpr uint64_t max_health = 0x1B4;
    }
    namespace Player {
        constexpr uint64_t local_player = 0x130;
        constexpr uint64_t model_instance = 0x380;
    }
}

static std::string ReadRobloxString(uint64_t addr) {
    if (!memory.is_valid_addr(addr)) return {};
    int len = memory.read<int>(addr + 0x18);
    if (len <= 0 || len > 200) return {};
    uint64_t data = (len >= 16) ? memory.read<uint64_t>(addr) : addr;
    if (!memory.is_valid_addr(data)) return {};
    return memory.read_string(data, len);
}

static std::string GetClassName(uint64_t addr) {
    if (!addr) return {};
    uint64_t desc = memory.read<uint64_t>(addr + Offsets::instance::ClassDescriptor);
    if (!desc) return {};
    uint64_t ptr = memory.read<uint64_t>(desc + Offsets::Core::Self);
    return ReadRobloxString(ptr);
}

static uint64_t FindChildByClass(uint64_t parent, const char* className) {
    if (!parent) return 0;
    uint64_t childrenPtr = memory.read<uint64_t>(parent + Offsets::instance::Children);
    if (!childrenPtr) return 0;
    uint64_t start = memory.read<uint64_t>(childrenPtr);
    uint64_t end = memory.read<uint64_t>(childrenPtr + Offsets::Core::Self);
    if (!start || !end || end <= start) return 0;
    size_t size = end - start;
    std::vector<uint8_t> buf(size);
    if (!memory.read_buffer(start, buf.data(), size)) return 0;
    for (size_t i = 0; i < size; i += 0x10) {
        uint64_t childAddr = *(uint64_t*)&buf[i];
        if (!childAddr) continue;
        if (GetClassName(childAddr) == className) return childAddr;
    }
    return 0;
}

float get_local_player_health() {
    uint64_t rj = memory.find_pattern("52 65 6E 64 65 72 4A 6F 62 28 50 6F 73 74 52 65 6E 64 65 72 3B");
    if (!rj) return -1.f;
    uint64_t rv = memory.read<uint64_t>(rj + Offsets::Core::render_view_offset);
    if (!rv) return -1.f;
    uint64_t fdm = memory.read<uint64_t>(rv + Offsets::Core::DatamodelHolder);
    if (!fdm) return -1.f;
    uint64_t dm = memory.read<uint64_t>(fdm + Offsets::Core::Datamodel);
    if (!dm) return -1.f;
    uint64_t players = FindChildByClass(dm, "Players");
    if (!players) return -1.f;
    uint64_t localPlayer = memory.read<uint64_t>(players + Offsets::Player::local_player);
    if (!localPlayer) return -1.f;
    uint64_t character = memory.read<uint64_t>(localPlayer + Offsets::Player::model_instance);
    if (!character) return -1.f;
    uint64_t humanoid = FindChildByClass(character, "Humanoid");
    if (!humanoid) return -1.f;
    return memory.read<float>(humanoid + Offsets::humanoid::health);
}

float get_local_player_max_health() {
    uint64_t rj = memory.find_pattern("52 65 6E 64 65 72 4A 6F 62 28 50 6F 73 74 52 65 6E 64 65 72 3B");
    if (!rj) return -1.f;
    uint64_t rv = memory.read<uint64_t>(rj + Offsets::Core::render_view_offset);
    if (!rv) return -1.f;
    uint64_t fdm = memory.read<uint64_t>(rv + Offsets::Core::DatamodelHolder);
    if (!fdm) return -1.f;
    uint64_t dm = memory.read<uint64_t>(fdm + Offsets::Core::Datamodel);
    if (!dm) return -1.f;
    uint64_t players = FindChildByClass(dm, "Players");
    if (!players) return -1.f;
    uint64_t localPlayer = memory.read<uint64_t>(players + Offsets::Player::local_player);
    if (!localPlayer) return -1.f;
    uint64_t character = memory.read<uint64_t>(localPlayer + Offsets::Player::model_instance);
    if (!character) return -1.f;
    uint64_t humanoid = FindChildByClass(character, "Humanoid");
    if (!humanoid) return -1.f;
    return memory.read<float>(humanoid + Offsets::humanoid::max_health);
}

// dll part

typedef void(__cdecl* NotifyCallback)(int);
typedef void(__cdecl* ConnectChangedCallback)(const wchar_t*, bool);
typedef void(__cdecl* ToyAddCallback)(const wchar_t*);
typedef void(__cdecl* StartBLEScanFunc)();
typedef void(__cdecl* StopBLEScanFunc)();
typedef void(__cdecl* QuitFunc)();
typedef void(__cdecl* RegisterNotifyCallbackFunc)(NotifyCallback);
typedef void(__cdecl* RegisterConnectChangedCallbackFunc)(ConnectChangedCallback);
typedef void(__cdecl* RegisterToyAddCallbackFunc)(ToyAddCallback);
typedef void(__cdecl* ConnectToyFunc)(wchar_t*);
typedef void(__cdecl* SendCommandFunc)(wchar_t*, int, int);

enum { CMD_VIBRATE = 0 };
enum { SCAN_STOP = 1, CONNECTION_SUCCESS = 7 };

static std::wstring g_toyId;
static std::atomic<bool> g_scanStopped{ false };
static std::atomic<bool> g_connected{ false };
static StartBLEScanFunc _StartBLEScan = nullptr;
static StopBLEScanFunc _StopBLEScan = nullptr;
static QuitFunc _Quit = nullptr;
static RegisterNotifyCallbackFunc _RegisterNotifyCallback = nullptr;
static RegisterConnectChangedCallbackFunc _RegisterConnectChangedCallback = nullptr;
static RegisterToyAddCallbackFunc _RegisterToyAddCallback = nullptr;
static ConnectToyFunc _ConnectToy = nullptr;
static SendCommandFunc _SendCommand = nullptr;

static void __cdecl OnNotify(int t) {
    if (t == SCAN_STOP) g_scanStopped = true;
    if (t == CONNECTION_SUCCESS) g_connected = true;
}
static void __cdecl OnConnect(const wchar_t*, bool c) { if (c) g_connected = true; }
static void __cdecl OnToyAdd(const wchar_t* id) { if (id && g_toyId.empty()) g_toyId = id; }

static bool LoadBleDll() {
    HMODULE h = LoadLibraryW(L"LovenseBLE_Lib.dll");
    if (!h) return false;
    _StartBLEScan = (StartBLEScanFunc)GetProcAddress(h, "_StartBLEScan");
    _StopBLEScan = (StopBLEScanFunc)GetProcAddress(h, "_StopBLEScan");
    _Quit = (QuitFunc)GetProcAddress(h, "_Quit");
    _RegisterNotifyCallback = (RegisterNotifyCallbackFunc)GetProcAddress(h, "_RegisterNotifyCallback");
    _RegisterConnectChangedCallback = (RegisterConnectChangedCallbackFunc)GetProcAddress(h, "_RegisterConnectChangedCallback");
    _RegisterToyAddCallback = (RegisterToyAddCallbackFunc)GetProcAddress(h, "_RegisterToyAddCallback");
    _ConnectToy = (ConnectToyFunc)GetProcAddress(h, "_ConnectToy");
    _SendCommand = (SendCommandFunc)GetProcAddress(h, "_SendCommand");
    return _StartBLEScan && _StopBLEScan && _RegisterNotifyCallback && _RegisterToyAddCallback && _ConnectToy && _SendCommand;
}

static void VibrateLush(int level) {
    if (g_toyId.empty() || !_SendCommand) return;
    safe::vector<wchar_t> idBuf;
    std::vector<wchar_t> tmp(g_toyId.begin(), g_toyId.end());
    tmp.push_back(0);
    idBuf.replace(tmp);
    auto snap = idBuf.get_snapshot();
    if (snap->size() > 0)
        _SendCommand(snap->data(), CMD_VIBRATE, level);
}

int main() {
    std::cout << "VibeOnHit\nRoblox must be running.\n\n";

    if (!memory.init()) {
        std::cerr << "Roblox not found. Start a game first.\n";
        return 1;
    }
    if (!memory.attach()) {
        std::cerr << "Failed to attach to Roblox.\n";
        return 1;
    }

    if (!LoadBleDll()) {
        std::cerr << "LovenseBLE_Lib.dll not found.\n";
        return 1;
    }

    _RegisterNotifyCallback(OnNotify);
    _RegisterConnectChangedCallback(OnConnect);
    _RegisterToyAddCallback(OnToyAdd);
    _StartBLEScan();

    for (int i = 0; i < 40 && g_toyId.empty(); i++)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    if (g_toyId.empty()) {
        std::cerr << "No Lovense toy found.\n";
        if (_Quit) _Quit();
        return 1;
    }

    _StopBLEScan();
    while (!g_scanStopped) std::this_thread::sleep_for(std::chrono::milliseconds(50));

    safe::vector<wchar_t> idBuf;
    std::vector<wchar_t> tmp(g_toyId.begin(), g_toyId.end());
    tmp.push_back(0);
    idBuf.replace(tmp);

    g_connected = false;
    auto snap = idBuf.get_snapshot();
    if (snap->size() > 0)
        _ConnectToy(snap->data());

    for (int i = 0; i < 30 && !g_connected; i++)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    if (!g_connected) {
        std::cerr << "Connection timeout.\n";
        if (_Quit) _Quit();
        return 1;
    }

    std::cout << "Connected.\n\n";

    float prevHealth = get_local_player_health();
    if (prevHealth < 0) prevHealth = 100.f;

    while (true) {
        float health = get_local_player_health();
        float maxHealth = get_local_player_max_health();

        if (health >= 0 && maxHealth > 0) {
            if (health < prevHealth && prevHealth > 0) {
                std::cout << "[hit] Health " << prevHealth << " -> " << health << " | Lush MAX!\n";
                VibrateLush(20);
                std::this_thread::sleep_for(std::chrono::seconds(2));
                VibrateLush(0);
            }
            prevHealth = health;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    if (_Quit) _Quit();
    return 0;
}
