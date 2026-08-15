# VibeOnHit

Roblox health drop → Lovense Lush max vibe.

Reads local player HP from `RobloxPlayerBeta.exe`. When you take a hit, the Lush vibrates at 20 for 2 seconds.

BLE only. Uses `LovenseBLE_Lib.dll` — no dongle.

## Requirements

- Windows 10+
- Roblox open, actually in a game
- Bluetooth on, Lush in pairing mode
- `LovenseBLE_Lib.dll` next to the exe  
  https://developer.lovense.com/docs/game-engine-plugins/windows_ble.html

## Build

Open `LovensePrototype.vcxproj` in Visual Studio, build **x64**. Output exe is `VibeOnHit.exe`.

Copy `LovenseBLE_Lib.dll` into the same folder as the exe (`x64\Release\` or `x64\Debug\`).

## Run

1. Join a Roblox game
2. Put the Lush in pairing mode
3. Run `VibeOnHit.exe`

It attaches to Roblox first, then scans for a Lovense toy. If health drops, it prints `[hit]` and vibes.

Offsets in `LovensePrototype.cpp` are mostly outdated. If HP reads as -1, those need a bump. 
You can find updated offsets in this website: [OFFSETS](https://offsets.imtheo.lol)

BLE reference: [Windows BLE DLL](https://developer.lovense.com/docs/game-engine-plugins/windows_ble.html#c-example)
