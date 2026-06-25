#pragma once

#include <cstdint>
#include <cstring>

#include "server/telemetry_server.hpp"

#pragma pack(push, 1)
struct TelemetryPacket
{
    using s8 = int8_t;
    using s16 = int16_t;
    using s32 = int32_t;
    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using f32 = float;
    using f64 = double;

    s32 IsRaceOn;  // = 1 when race is on. = 0 when in menus/race stopped.
    u32 Timestamp; // Timestamp in miliiseconds, can overflow to 0 eventually

    f32 EngineMaxRpm;     // Engine max RPM values
    f32 EngineIdleRpm;    // Engine idle RPM values
    f32 CurrentEngineRpm; // Current engine RPM values

    f32 AccelerationX; // In the car's local space; X = right, Y = up, Z = forward
    f32 AccelerationY; // In the car's local space; X = right, Y = up, Z = forward
    f32 AccelerationZ; // In the car's local space; X = right, Y = up, Z = forward

    f32 VelocityX; // In the car's local space; X = right, Y = up, Z = forward
    f32 VelocityY; // In the car's local space; X = right, Y = up, Z = forward
    f32 VelocityZ; // In the car's local space; X = right, Y = up, Z = forward

    f32 AngularVelocityX; // Angular velocity in the car's local space (rad/s); X = pitch, Y = yaw, Z = roll
    f32 AngularVelocityY; // Angular velocity in the car's local space (rad/s); X = pitch, Y = yaw, Z = roll
    f32 AngularVelocityZ; // Angular velocity in the car's local space (rad/s); X = pitch, Y = yaw, Z = roll

    f32 Yaw;   // Car orientation (radians)
    f32 Pitch; // Car orientation (radians)
    f32 Roll;  // Car orientation (radians)

    f32 NormalizedSuspensionTravelFrontLeft; // Suspension travel normalized: 0.0f = max stretch; 1.0 = max compression
    f32 NormalizedSuspensionTravelFrontRight; // Suspension travel normalized: 0.0f = max stretch; 1.0 = max compression
    f32 NormalizedSuspensionTravelRearLeft; // Suspension travel normalized: 0.0f = max stretch; 1.0 = max compression
    f32 NormalizedSuspensionTravelRearRight; // Suspension travel normalized: 0.0f = max stretch; 1.0 = max compression

    f32 TireSlipRatioFrontLeft; // Tire normalized slip ratio, = 0 means 100% grip and |ratio| > 1.0 means loss of grip
    f32 TireSlipRatioFrontRight; // Tire normalized slip ratio, = 0 means 100% grip and |ratio| > 1.0 means loss of grip
    f32 TireSlipRatioRearLeft; // Tire normalized slip ratio, = 0 means 100% grip and |ratio| > 1.0 means loss of grip
    f32 TireSlipRatioRearRight; // Tire normalized slip ratio, = 0 means 100% grip and |ratio| > 1.0 means loss of grip

    f32 WheelRotationSpeedFrontLeft;  // Wheel rotation speed radians/sec
    f32 WheelRotationSpeedFrontRight; // Wheel rotation speed radians/sec
    f32 WheelRotationSpeedRearLeft;   // Wheel rotation speed radians/sec
    f32 WheelRotationSpeedRearRight;  // Wheel rotation speed radians/sec

    s32 WheelOnRumbleStripFrontLeft; // = 1 when wheel is on rumble strip, = 0 when off
    s32 WheelOnRumbleStripFrontRight; // = 1 when wheel is on rumble strip, = 0 when off
    s32 WheelOnRumbleStripRearLeft; // = 1 when wheel is on rumble strip, = 0 when off
    s32 WheelOnRumbleStripRearRight; // = 1 when wheel is on rumble strip, = 0 when off

    s32 WheelInPuddleFrontLeft;  // = 1 when wheel is in a puddle, = 0 when not
    s32 WheelInPuddleFrontRight; // = 1 when wheel is in a puddle, = 0 when not
    s32 WheelInPuddleRearLeft;   // = 1 when wheel is in a puddle, = 0 when not
    s32 WheelInPuddleRearRight;  // = 1 when wheel is in a puddle, = 0 when not

    f32 SurfaceRumbleFrontLeft; // Non-dimensional surface rumble values passed to controller force feedback
    f32 SurfaceRumbleFrontRight; // Non-dimensional surface rumble values passed to controller force feedback
    f32 SurfaceRumbleRearLeft; // Non-dimensional surface rumble values passed to controller force feedback
    f32 SurfaceRumbleRearRight; // Non-dimensional surface rumble values passed to controller force feedback

    f32 TireSlipAngleFrontLeft; // Tire normalized slip angle, = 0 means 100% grip and |angle| > 1.0 means loss of grip
    f32 TireSlipAngleFrontRight; // Tire normalized slip angle, = 0 means 100% grip and |angle| > 1.0 means loss of grip
    f32 TireSlipAngleRearLeft; // Tire normalized slip angle, = 0 means 100% grip and |angle| > 1.0 means loss of grip
    f32 TireSlipAngleRearRight; // Tire normalized slip angle, = 0 means 100% grip and |angle| > 1.0 means loss of grip

    f32 TireCombinedSlipFrontLeft; // Tire normalized combined slip, = 0 means 100% grip and |slip| > 1.0 means loss of grip
    f32 TireCombinedSlipFrontRight; // Tire normalized combined slip, = 0 means 100% grip and |slip| > 1.0 means loss of grip
    f32 TireCombinedSlipRearLeft; // Tire normalized combined slip, = 0 means 100% grip and |slip| > 1.0 means loss of grip
    f32 TireCombinedSlipRearRight; // Tire normalized combined slip, = 0 means 100% grip and |slip| > 1.0 means loss of grip

    f32 SuspensionTravelMetersFrontLeft;  // Actual suspension travel in meters
    f32 SuspensionTravelMetersFrontRight; // Actual suspension travel in meters
    f32 SuspensionTravelMetersRearLeft;   // Actual suspension travel in meters
    f32 SuspensionTravelMetersRearRight;  // Actual suspension travel in meters

    s32 CarOrdinal; // Unique ID of the car make/model
    s32 CarClass; // Between 0 (D -- worst cars) and 7 (X class -- best cars) inclusive
    s32 CarPerformanceIndex; // Between 100 (worst car) and 999 (best car) inclusive
    s32 DrivetrainType;      // 0 = FWD, 1 = RWD, 2 = AWD
    s32 NumCylinder;         // Number of cylinders/rotors in the engine
    u32 CarGroup;            // Car group identifier

    f32 SmashableVelDiff; // Velocity loss from smashable object collision (m/s)
    f32 SmashableMass;    // Mass of recently hit smashable object (kg)

    f32 PositionX; // Position in world space (meters)
    f32 PositionY; // Position in world space (meters)
    f32 PositionZ; // Position in world space (meters)

    f32 Speed;  // Speed in meters per second
    f32 Power;  // Power in watts
    f32 Torque; // Torque in newton-meters

    f32 TireTempFrontLeft;  // Tire temperature (Fahrenheit)
    f32 TireTempFrontRight; // Tire temperature (Fahrenheit)
    f32 TireTempRearLeft;   // Tire temperature (Fahrenheit)
    f32 TireTempRearRight;  // Tire temperature (Fahrenheit)

    f32 Boost; // Turbo/supercharger boost (PSI above atmospheric)

    f32 Fuel; // Fuel level (0.0 = empty, 1.0 = full)

    f32 DistanceTraveled; // Total distance traveled (meters)

    f32 BestLap;    // Lap times (seconds); 0.0 if not applicable
    f32 LastLap;    // Lap times (seconds); 0.0 if not applicable
    f32 CurrentLap; // Lap times (seconds); 0.0 if not applicable

    f32 CurrentRaceTIme; // Total race time (seconds since driving started)

    u16 LapNumber;   // Number of laps completed
    u8 RacePosition; // Current race position

    u8 Accel;     // Player input (0 to 255)
    u8 Break;     // Player input (0 to 255)
    u8 Clutch;    // Player input (0 to 255)
    u8 HandBrake; // Player input (0 to 255)

    u8 Gear; // Current gear

    s8 Steer; // Steering input (-127 = full left, 0 = center, 127 = full right)

    s8 NormalizedDrivingLine; // Normalized driving line position (-127 to 127)
    s8 NormalizedAIBrakeDifference; // Normalized AI braking difference (-127 to 127)

    // Create packet from a buffer
    static TelemetryPacket FromBuffer(const TelemetryServer::buffer_t& buffer)
    {
        TelemetryPacket p;
        std::memcpy(&p, buffer.data(), sizeof(p));
        return p;
    }
};
#pragma pack(pop)

// Get car class name from class index
constexpr const char* GetCarClassString(int index)
{
    switch (index)
    {
        case 0:
            return "D";
        case 1:
            return "C";
        case 2:
            return "B";
        case 3:
            return "A";
        case 4:
            return "S1";
        case 5:
            return "S2";
        case 6:
            return "R";
        case 7:
            return "X";
        default:
            return "N/A";
    }
}

// Get drivetrain from drivetrain index
constexpr const char* GetDrivetrainString(int index)
{
    switch (index)
    {
        case 0:
            return "FWD";
        case 1:
            return "RWD";
        case 2:
            return "AWD";
        default:
            return "N/A";
    }
}
