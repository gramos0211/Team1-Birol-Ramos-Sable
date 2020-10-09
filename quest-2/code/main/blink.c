/* ADC1 Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "driver/uart.h"
#include "esp_vfs_dev.h"
#include "driver/i2c.h"
#include <stdint.h>

//taken from https://github.com/adafruit/Adafruit_LED_Backpack/blob/master/Adafruit_LEDBackpack.cpp


// Master I2C
#define I2C_EXAMPLE_MASTER_SCL_IO          22   // gpio number for i2c clk
#define I2C_EXAMPLE_MASTER_SDA_IO          23   // gpio number for i2c data
#define I2C_EXAMPLE_MASTER_NUM             I2C_NUM_0  // i2c port
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE  0    // i2c master no buffer needed
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE  0    // i2c master no buffer needed
#define I2C_EXAMPLE_MASTER_FREQ_HZ         100000     // i2c master clock freq
#define WRITE_BIT                          I2C_MASTER_WRITE // i2c master write
#define READ_BIT                           I2C_MASTER_READ  // i2c master read
#define ACK_CHECK_EN                       true // i2c master will check ack
#define ACK_CHECK_DIS                      false// i2c master will not check ack
#define ACK_VAL                            0x00 // i2c ack value
#define NACK_VAL                           0xFF // i2c nack value

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

static esp_adc_cal_characteristics_t *adc_chars1;
static const adc_channel_t channel_sensor1 = ADC_CHANNEL_3;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten_sensor1 = ADC_ATTEN_DB_11;
static const adc_unit_t unit_sensor1 = ADC_UNIT_1;

static esp_adc_cal_characteristics_t *adc_chars2;
static const adc_channel_t channel_sensor2 = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten_sensor2 = ADC_ATTEN_DB_11;
static const adc_unit_t unit_sensor2 = ADC_UNIT_1;

static esp_adc_cal_characteristics_t *adc_chars3;
static const adc_channel_t channel_sensor3 = ADC_CHANNEL_0;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten_sensor3 = ADC_ATTEN_DB_11;
static const adc_unit_t unit_sensor3 = ADC_UNIT_1;



static void test_alpha_display() {

    // Debug
    int ret;
    int time = 0;
    printf(">> Test Alphanumeric Display: \n");


    // Continually writes the same command
    while (1) {
      uint32_t adc_reading_sensor1 = 0;
      uint32_t adc_reading_sensor2 = 0;
      uint32_t adc_reading_sensor3 = 0;
      //Multisampling
      for (int i = 0; i < NO_OF_SAMPLES; i++) {
          if (unit_sensor1 == ADC_UNIT_1 && unit_sensor2 == ADC_UNIT_1 && unit_sensor3 == ADC_UNIT_1) {
              adc_reading_sensor1 += adc1_get_raw((adc1_channel_t)channel_sensor1);
              adc_reading_sensor2 += adc1_get_raw((adc1_channel_t)channel_sensor2);
              adc_reading_sensor3 += adc1_get_raw((adc1_channel_t)channel_sensor3);
          } 
      }
      adc_reading_sensor1 /= NO_OF_SAMPLES;
      adc_reading_sensor2 /= NO_OF_SAMPLES;
      adc_reading_sensor3 /= NO_OF_SAMPLES;

      //Convert adc_reading to voltage in mV
      uint32_t voltage1 = esp_adc_cal_raw_to_voltage(adc_reading_sensor1, adc_chars1);
      double distance1 = ((-0.0619047619048*voltage1) + 165.238095238)/2;

      
      //Convert adc_reading to voltage in mV
      uint32_t voltage2 = esp_adc_cal_raw_to_voltage(adc_reading_sensor2, adc_chars2);
      double distance2 = ((double)voltage2 / 6.4) * .254;
      
      //Convert adc_reading to voltage in mV
      double voltage3 = esp_adc_cal_raw_to_voltage(adc_reading_sensor3, adc_chars3);
      double resistance = (33000.0/(voltage3/1000)) - 10000.0;
      double temperatureKelvin = -(1 / ((log(10000.0/resistance)/3435.0) - (1/298.0)));
      double temperatureCelsius = -(273.15 - temperatureKelvin);

      printf("%d,%f,%f,%f\n", time++, distance1, temperatureCelsius, distance2);


      if(ret == ESP_OK) {
        //printf("- wrote: T.D.C.L. \n\n");
      }

      vTaskDelay(pdMS_TO_TICKS(1000));

    }
}

static void check_efuse(void)
{
    //Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }

    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}

void app_main(void)
{
    //Check if Two Point or Vref are burned into eFuse
    check_efuse();

    //Configure ADC
    if (unit_sensor3 == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(channel_sensor3, atten_sensor3);
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel_sensor3, atten_sensor3);
    }

    //Characterize ADC
    adc_chars1 = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type1 = esp_adc_cal_characterize(unit_sensor1, atten_sensor1, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars1);
    print_char_val_type(val_type1);

    adc_chars2 = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type2 = esp_adc_cal_characterize(unit_sensor2, atten_sensor2, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars2);
    print_char_val_type(val_type2);

    adc_chars3 = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type3 = esp_adc_cal_characterize(unit_sensor3, atten_sensor3, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars3);
    print_char_val_type(val_type3);

        /* Install UART driver for interrupt-driven reads and writes */
    ESP_ERROR_CHECK( uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0) );

    /* Tell VFS to use UART driver */
    esp_vfs_dev_uart_use_driver(UART_NUM_0);

    xTaskCreate(test_alpha_display,"test_alpha_display", 4096, NULL, 5, NULL);
}

