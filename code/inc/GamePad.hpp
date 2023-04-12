#pragma once

///Code from Jeremiah van Oosten, 03/01/2023
///https://github.com/jpvanoosten/SoftwareRasterizer/blob/main/graphics/inc/Graphics/GamePad.hpp

#include "GamePadState.hpp"

namespace Graphics
{
    /// <summary>
    /// Dead-zone mode.
    /// </summary>
    enum class DeadZone
    {
        IndependentAxis = 0,
        Circular,
        None,
    };

    class GamePad
    {
    public:
        static constexpr int MAX_PLAYERS = 2;

        static GamePadState getState(int player, DeadZone deadZoneMode = DeadZone::IndependentAxis);

        static bool setVibration(int player, float leftMotor, float rightMotor/*,
            float leftTrigger = 0.0f, float rightTrigger = 0.0f*/);

        // Static class, delete constructors and assignment operators.
        GamePad() = delete;
        GamePad(const GamePad&) = delete;
        GamePad(GamePad&&) = delete;
        ~GamePad() = delete;

        const GamePad& operator=(const GamePad&) const = delete;
        const GamePad& operator=(GamePad&&) const = delete;
    };
}  // namespace Graphics