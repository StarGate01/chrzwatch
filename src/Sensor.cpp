/**
 * @file SensorService.cpp
 * @author Christoph Honal
 * @brief Implements the functions defined in SensorService.h
 * @version 0.1
 * @date 2020-05-21
 */

#include "SensorService.h"

SensorService::SensorService(DisplayService& display_service):
    _event_queue(16 * EVENTS_EVENT_SIZE),
    _display_service(display_service),
    _battery(PIN_BATTERY),
    _charging(PIN_CHARGE),
    _hr_value(100),
    _battery_value(50),
    _charging_value(false),
    _button1(PIN_BUTTON1),
    _button2(PIN_BUTTON2),
    _cancel_timeout(true)
{ 
    // //Wake from poweroff via buttons
    // nrf_gpio_cfg_sense_input(PIN_BUTTON1, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_HIGH);
    // nrf_gpio_cfg_sense_input(PIN_BUTTON2, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_HIGH);
    //Handle dispatching events
    _event_queue.call_every(100, this, &SensorService::_poll);
    _event_queue.call_every(5000, this, &SensorService::_handleDisplayTimeout);
    _event_thread.start(callback(&_event_queue, &EventQueue::dispatch_forever));
}

uint16_t SensorService::getHRValue()
{
    return _hr_value;
}

uint8_t SensorService::getBatteryPercent()
{
    return (uint8_t)round(((_battery_value * 1.024f) - 0.330f) / 0.0009f);
}

bool SensorService::getBatteryCharging()
{
    return _charging_value;
}

void SensorService::_poll()
{
    // Update sensors
    _battery_value = _battery.read();
    _charging_value = (_charging.read() == 0);

    // Check buttons
    int button1_now = _button1.read();
    int button2_now = _button2.read();

    if((button1_now == 0 && _last_button1 == 1) || 
        (button2_now == 0 && _last_button2 == 1))
    {
        _handleButton();
    } 

    _last_button1 = button1_now;
    _last_button2 = button2_now;
}

void SensorService::_handleButton()
{
    _hr_value = 100;
    _display_service.vibrate(BUTTON_VIBRATION_LENGTH);
    _display_service.setPower(true);
    _cancel_timeout = true;
}

void SensorService::_handleDisplayTimeout()
{
    if(!_cancel_timeout)
    {
        _display_service.setPower(false);
    }
    _cancel_timeout = false;
}