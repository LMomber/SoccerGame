#include "Controller.h"


Controller::Controller(int player)
    : player{player}
{}

void Controller::Update(float deltaTime)
{
    state = Graphics::GamePad::getState(player, Graphics::DeadZone::IndependentAxis);
    Graphics::GamePad::setVibration(player, vibration, vibration);
    vibration -= deltaTime;
}

const float Controller::GetThumbStickL() const
{
    return state.thumbSticks.leftX;
}

const bool Controller::GetButtonA() const
{
    if (state.buttons.a)
    {
        return true;
    }
    else return false;
}

void Controller::SetVibration(float vibration)
{
    this->vibration = vibration;
}

bool Controller::IsConnected()
{
    return state.connected;
}
