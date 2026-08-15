#pragma once

#include <cstdint>

namespace rbx::Offsets {
    namespace Core {
        constexpr std::uint64_t render_view_offset = 0xC8;
        constexpr std::uint64_t DatamodelHolder = 0x128;
        constexpr std::uint64_t Datamodel = 0x1C0;
        constexpr std::uint64_t visual_engine = 0x10;

        constexpr std::uint64_t Self = 0x8;

        constexpr std::uint64_t dimensions = 0xa70;
        constexpr std::uint64_t view_matrix = 0x140;

        constexpr std::uint64_t Value = 0xD0;
    }

    namespace task_scheduler {
        constexpr std::uint64_t FakeDataModelToDataModel = 0x1B0;
        constexpr std::uint64_t JobEnd = 0x1D8;
        constexpr std::uint64_t JobName = 0x18;
        constexpr std::uint64_t JobStart = 0x1D0;
        constexpr std::uint64_t max_fps = 0x1B0;
        constexpr std::uint64_t Pointer = 0x78E3008;
        constexpr std::uint64_t RenderJobToFakeDataModel = 0x38;
        constexpr std::uint64_t RenderJobToRenderView = 0x218;
    }

    namespace visual_engine {
        constexpr std::uint64_t dimensions = 0x720;
        constexpr std::uint64_t Pointer = 0x75278C0;
        constexpr std::uint64_t ToDataModel1 = 0x700;
        constexpr std::uint64_t ToDataModel2 = 0x1C0;
        constexpr std::uint64_t view_matrix = 0x4B0;
    }

    namespace FakeDataModel {
        constexpr std::uint64_t Pointer = 0x77A03A8;
        constexpr std::uint64_t RealDataModel = 0x1C0;
    }
    
    namespace Game {
        constexpr std::uint64_t GameID = 0x190;
        constexpr std::uint64_t place_id = 0x198;
        constexpr std::uint64_t server_ip = 0x5D8;
    }

    namespace Lighting {
        constexpr std::uint64_t Ambient = 0xD8;
        constexpr std::uint64_t Brightness = 0x120;
        constexpr std::uint64_t ClockTime = 0x1B8;
        constexpr std::uint64_t ColorShift_Bottom = 0xF0;
        constexpr std::uint64_t ColorShift_Top = 0xE4;
        constexpr std::uint64_t ExposureCompensation = 0x12C;
        constexpr std::uint64_t FogColor = 0xFC;
        constexpr std::uint64_t FogEnd = 0x134;
        constexpr std::uint64_t FogStart = 0x138;
        constexpr std::uint64_t GeographicLatitude = 0x190;
        constexpr std::uint64_t OutdoorAmbient = 0x108;
    }

    namespace instance {
        constexpr std::uint64_t ClassDescriptor = 0x18;
        constexpr std::uint64_t name = 0xB0;
        constexpr std::uint64_t Children = 0x70;
        constexpr std::uint64_t Parent = 0x68;
    }

    namespace Datamodel {
        constexpr std::uint64_t PlaceId = 0x198;
    }

    namespace humanoid {
        constexpr std::uint64_t health = 0x194;
        constexpr std::uint64_t max_health = 0x1B4;
        constexpr std::uint64_t WalkspeedA = 0x1D4;
        constexpr std::uint64_t WalkspeedB = 0x3A8;
        constexpr std::uint64_t JumpPower = 0x1B0;

        constexpr std::uint64_t rig_type = 0x1C8;
    }

    namespace part {
        constexpr std::uint64_t primitive = 0x148;
    }

    namespace primitive {
        constexpr std::uint64_t MoveDirection = 0x158;
        constexpr std::uint64_t Position = 0xE4;
        constexpr std::uint64_t Rotation = 0xC0;
        constexpr std::uint64_t Velocity = 0xF0;
        constexpr std::uint64_t Anchored = 0x310;
        constexpr std::uint64_t CanCollide = 0x312;
        constexpr std::uint64_t Transparency = 0xF0;
        constexpr std::uint64_t Size = 0x1B0;
    }

    namespace camera {
        constexpr std::uint64_t camera = 0x450;
        constexpr std::uint64_t Rotation = 0xF8;
        constexpr std::uint64_t Position = 0x11C;
        constexpr std::uint64_t FocusPosition = 0x14C;
        constexpr std::uint64_t Subject = 0xE8;
    }

    namespace Player {
        constexpr std::uint64_t local_player = 0x130;
        constexpr std::uint64_t model_instance = 0x380;
        constexpr std::uint64_t UserId = 0x2A8;
        constexpr std::uint64_t Team = 0x270;
    }

    namespace Input {
        constexpr std::uint64_t InputObject = 0x100;
        constexpr std::uint64_t MousePosition = 0xEC;
    }

    namespace GuiObject {
        constexpr std::uint64_t TextLabelText = 0xD98;
        constexpr std::uint64_t Position = 0x520;
        constexpr std::uint64_t Size = 0x540;
    }

    constexpr std::uint64_t Texture = 0x198;

    namespace Templates {
        constexpr std::uint64_t ShirtTemplate = 0x108;
        constexpr std::uint64_t PantsTemplate = 0xE0;
    }

    namespace Mesh {
        constexpr std::uint64_t MeshId = 0x2E0;
        constexpr std::uint64_t MeshTexture = 0x310;
    }

    namespace SpecialMesh {
        constexpr std::uint64_t MeshId = 0x108;
        constexpr std::uint64_t MeshTexture = 0x130;
    }

    namespace Weld {
        constexpr std::uint64_t WeldC0 = 0x128;
        constexpr std::uint64_t WeldC1 = 0x138;
    }

    namespace Model {
        constexpr std::uint64_t PrimaryPart = 0x278;
    }
}