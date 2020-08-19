/**
 * @file DisplayService.h
 * @author Christoph Honal
 * @brief Defines a class to interact with the LCD display and other actors
 * @version 0.1
 * @date 2020-05-21
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#define MBED_CONF_RTOS_PRESENT 1

#include <mbed.h>
#include <rtos.h>

#include <Adafruit_ST7735_Mini.h>

#include "HardwareConfiguration.h"


/**
 * @brief Holds the state of the display screen
 * 
 */
class ScreenModel
{

    public: 
        /**
         * @brief Construct a new Screen Model object
         * 
         */
        ScreenModel();

        /**
         * @brief Renders the model to a LCD
         */
        void render(Adafruit_ST7735_Mini &lcd);

        time_t epochTime; //!< Current time
        uint8_t batteryValue; //!< Battery remaining in percent
        bool batteryCharging; //!< Battery charging state

};


/**
 * @brief Provides methods to interact with the LCD display and other actors
 * 
 */
class DisplayService
{

    public:
        /**
         * @brief Construct a new Display Service object
         */
        DisplayService();

        /**
         * @brief Vibrates the motor
         * 
         * @param milliSeconds For how long to vibrate
         * 
         */
        void vibrate(uint16_t duration);

        /**
         * @brief Render the internal state to the LCD display
         * 
         */
        void render();

        /**
         * @brief Turns the display on or off to save energy
         * 
         * @param on True to turn on, false to turn off
         */
        void setPower(bool on);

        /**
         * @brief Return the power state
         * 
         * @return true 
         * @return false 
         */
        bool getPower();

        ScreenModel screenModel; //!< Contents of the screen

    protected:
        DigitalOut _vibration; //!< Vibration output
        Thread _vibration_thread; //!< Thread for vibration duration
        Semaphore _vibration_trigger; //!< Interlock to trigger vibration
        uint16_t _vibration_duration; //!< Duration of the vibration in ms

        Adafruit_ST7735_Mini _lcd; //!< LCD output
        PwmOut _lcd_bl;
        DigitalOut _lcd_pwr;

        bool _is_on; //!< Power state

        /**
         * @brief Waits for the vibration interlock and then vibrates the motor
         * 
         */
        void threadVibration();

};

#endif
