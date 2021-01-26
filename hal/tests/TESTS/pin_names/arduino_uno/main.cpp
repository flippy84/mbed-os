/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "mbed.h"

// #if !TARGET_FF_ARDUINO_UNO
#if !(defined (TARGET_FF_ARDUINO) || (TARGET_FF_ARDUINO_UNO))
#error [NOT_SUPPORTED] Test needs Arduino Uno form factor
#else

#if defined (TARGET_FF_ARDUINO)
#warning ARDUINO form factor should not be used any more => use ARDUINO_UNO
#endif

using namespace utest::v1;

template <PinName TestedPin>
void GPIO_test()
{
    printf("Pin 0x%x\n", TestedPin);

    TEST_SKIP_UNLESS_MESSAGE(TestedPin != (PinName)NC, "NC pin");
    TEST_SKIP_UNLESS_MESSAGE(TestedPin != USBTX, "USBTX pin");
    TEST_SKIP_UNLESS_MESSAGE(TestedPin != USBRX, "USBRX pin");

    const PinMap *maps = gpio_pinmap(); // hal/source/mbed_gpio.c
    while (maps->pin != (PinName)NC) {
        // printf("map pin 0x%x\n", maps->pin);
        if (maps->pin == TestedPin) {
            {
                DigitalOut   TEST1(maps->pin);
                TEST1 = !TEST1;
            }
            {
                DigitalIn   TEST1(maps->pin);
                TEST1.read();
            }
            {
                DigitalInOut   TEST1(maps->pin);
                TEST1.input();
                TEST1.output();
            }

            TEST_ASSERT(true);
            return;
        }
        maps++;
    }
    TEST_ASSERT(false);
}


template <PinName TestedPin>
void AnalogIn_test()
{
    printf("Pin 0x%x\n", TestedPin);

    const PinMap *maps = analogin_pinmap();
    while (maps->pin != (PinName)NC) {
        // printf("map pin 0x%x\n", maps->pin);
        if (maps->pin == TestedPin) {

            AnalogIn TEST(TestedPin);
            TEST.read_u16();

            TEST_ASSERT(true);
            return;
        }
        maps++;
    }
    TEST_ASSERT(false);
}


template <PinName TestedPin>
void PWM_test()
{
    printf("Pin 0x%x\n", TestedPin);

    const PinMap *maps = pwmout_pinmap();
    while (maps->pin != (PinName)NC) {
        // printf("map pin 0x%x\n", maps->pin);
        if (maps->pin == TestedPin) {

            PwmOut pwm(TestedPin);
            pwm.period(1.0f);
            pwm.write(0.5f);

            TEST_ASSERT(true);
            return;
        }
        maps++;
    }

    // PWM support is not mandatory
    TEST_SKIP_UNLESS_MESSAGE(false, "ARDUINO_UNO WARNING: NO PWM ");
}


template <PinName TX_pin, PinName RX_pin>
void UART_test()
{
    printf("TX Pin 0x%x RX Pin 0x%x\n", TX_pin, RX_pin);

    TEST_SKIP_UNLESS_MESSAGE(TX_pin != USBTX, "USBTX pin");
    TEST_SKIP_UNLESS_MESSAGE(RX_pin != USBRX, "USBRX pin");

    {
        const PinMap *maps = serial_tx_pinmap();
        while (maps->pin != (PinName)NC) {
            // printf("map pin 0x%x\n", maps->pin);
            if (maps->pin == TX_pin) {
                break;
            }
            maps++;
        }
        TEST_ASSERT_NOT_EQUAL(NC, maps->pin);
    }
    {
        const PinMap *maps = serial_rx_pinmap();
        while (maps->pin != (PinName)NC) {
            // printf("map pin 0x%x\n", maps->pin);
            if (maps->pin == RX_pin) {
                break;
            }
            maps++;
        }
        TEST_ASSERT_NOT_EQUAL(NC, maps->pin);
    }

    BufferedSerial TEST(TX_pin, RX_pin);
    TEST.set_baud(115200);
}


template <PinName SDA_pin, PinName SCL_pin>
void I2C_test()
{
    printf("SDA Pin 0x%x SCL Pin 0x%x\n", SDA_pin, SCL_pin);

    {
        const PinMap *maps = i2c_master_sda_pinmap();
        while (maps->pin != (PinName)NC) {
            // printf("map pin 0x%x\n", maps->pin);
            if (maps->pin == SDA_pin) {
                break;
            }
            maps++;
        }
        TEST_ASSERT_NOT_EQUAL(NC, maps->pin);
    }
    {
        const PinMap *maps = i2c_master_scl_pinmap();
        while (maps->pin != (PinName)NC) {
            // printf("map pin 0x%x\n", maps->pin);
            if (maps->pin == SCL_pin) {
                break;
            }
            maps++;
        }
        TEST_ASSERT_NOT_EQUAL(NC, maps->pin);
    }

    I2C i2c(SDA_pin, SCL_pin);
    i2c.read(0);
}


template <PinName MOSI_pin, PinName MISO_pin, PinName CLK_pin, PinName CS_pin>
void SPI_test()
{
    printf("MOSI Pin 0x%x MISO Pin 0x%x CLOCK Pin 0x%x CS Pin0x%x\n", MOSI_pin, MISO_pin, CLK_pin, CS_pin);

    {
        const PinMap *maps = spi_master_mosi_pinmap();
        while (maps->pin != (PinName)NC) {
            // printf("map pin 0x%x\n", maps->pin);
            if (maps->pin == MOSI_pin) {
                break;
            }
            maps++;
        }
        TEST_ASSERT_NOT_EQUAL(NC, maps->pin);
    }
    {
        const PinMap *maps = spi_master_miso_pinmap();
        while (maps->pin != (PinName)NC) {
            // printf("map pin 0x%x\n", maps->pin);
            if (maps->pin == MISO_pin) {
                break;
            }
            maps++;
        }
        TEST_ASSERT_NOT_EQUAL(NC, maps->pin);
    }
    {
        const PinMap *maps = spi_master_clk_pinmap();
        while (maps->pin != (PinName)NC) {
            // printf("map pin 0x%x\n", maps->pin);
            if (maps->pin == CLK_pin) {
                break;
            }
            maps++;
        }
        TEST_ASSERT_NOT_EQUAL(NC, maps->pin);
    }
    {
        const PinMap *maps = gpio_pinmap(); // CS pin could be a simple GPIO
        while (maps->pin != (PinName)NC) {
            // printf("map pin 0x%x\n", maps->pin);
            if (maps->pin == CS_pin) {
                break;
            }
            maps++;
        }
        TEST_ASSERT_NOT_EQUAL(NC, maps->pin);
    }

    SPI device(MOSI_pin, MISO_pin, CLK_pin);
    DigitalOut cs(CS_pin);
    device.frequency(10000000);
}


Case cases[] = {
    Case("GPIO A0", GPIO_test<ARDUINO_UNO_A0>),
    Case("GPIO A1", GPIO_test<ARDUINO_UNO_A1>),
    Case("GPIO A2", GPIO_test<ARDUINO_UNO_A2>),
    Case("GPIO A3", GPIO_test<ARDUINO_UNO_A3>),
    Case("GPIO A4", GPIO_test<ARDUINO_UNO_A4>),
    Case("GPIO A5", GPIO_test<ARDUINO_UNO_A5>),
    Case("GPIO D0", GPIO_test<ARDUINO_UNO_D0>),
    Case("GPIO D1", GPIO_test<ARDUINO_UNO_D1>),
    Case("GPIO D2", GPIO_test<ARDUINO_UNO_D2>),
    Case("GPIO D3", GPIO_test<ARDUINO_UNO_D3>),
    Case("GPIO D4", GPIO_test<ARDUINO_UNO_D4>),
    Case("GPIO D5", GPIO_test<ARDUINO_UNO_D5>),
    Case("GPIO D6", GPIO_test<ARDUINO_UNO_D6>),
    Case("GPIO D7", GPIO_test<ARDUINO_UNO_D7>),
    Case("GPIO D8", GPIO_test<ARDUINO_UNO_D8>),
    Case("GPIO D9", GPIO_test<ARDUINO_UNO_D9>),
    Case("GPIO D10", GPIO_test<ARDUINO_UNO_D10>),
    Case("GPIO D11", GPIO_test<ARDUINO_UNO_D11>),
    Case("GPIO D12", GPIO_test<ARDUINO_UNO_D12>),
    Case("GPIO D13", GPIO_test<ARDUINO_UNO_D13>),
    Case("GPIO D14", GPIO_test<ARDUINO_UNO_D14>),
    Case("GPIO D15", GPIO_test<ARDUINO_UNO_D15>),

    Case("ADC A0", AnalogIn_test<ARDUINO_UNO_A0>),
    Case("ADC A1", AnalogIn_test<ARDUINO_UNO_A1>),
    Case("ADC A2", AnalogIn_test<ARDUINO_UNO_A2>),
    Case("ADC A3", AnalogIn_test<ARDUINO_UNO_A3>),
    Case("ADC A4", AnalogIn_test<ARDUINO_UNO_A4>),
    Case("ADC A5", AnalogIn_test<ARDUINO_UNO_A5>),

    Case("PWM D3", PWM_test<ARDUINO_UNO_D3>),
    Case("PWM D5", PWM_test<ARDUINO_UNO_D5>),
    Case("PWM D6", PWM_test<ARDUINO_UNO_D6>),
    Case("PWM D9", PWM_test<ARDUINO_UNO_D9>),
    Case("PWM D10", PWM_test<ARDUINO_UNO_D10>),
    Case("PWM D11", PWM_test<ARDUINO_UNO_D11>),

    Case("UART", UART_test<ARDUINO_UNO_UART_TX, ARDUINO_UNO_UART_RX>),

    Case("I2C", I2C_test<ARDUINO_UNO_I2C_SDA, ARDUINO_UNO_I2C_SCL>),

    Case("SPI", SPI_test<ARDUINO_UNO_SPI_MOSI, ARDUINO_UNO_SPI_MISO, ARDUINO_UNO_SPI_SCK, ARDUINO_UNO_SPI_CS>),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(25, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif
