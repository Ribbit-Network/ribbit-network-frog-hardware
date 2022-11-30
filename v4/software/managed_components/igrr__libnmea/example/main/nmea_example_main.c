/* NMEA parsing example for ESP32.
 * Based on "parse_stdin.c" example from libnmea.
 * Copyright (c) 2015 Jack Engqvist Johansson.
 * Additions Copyright (c) 2017 Ivan Grokhotkov.
 * See "LICENSE" file in libnmea directory for license.
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "nmea.h"
#include "gpgll.h"
#include "gpgga.h"
#include "gprmc.h"
#include "gpgsa.h"
#include "gpvtg.h"
#include "gptxt.h"
#include "gpgsv.h"

#define UART_NUM                UART_NUM_1
#define UART_RX_PIN             21
#define UART_RX_BUF_SIZE        (1024)

static void uart_setup();
static void read_and_parse_nmea();

void app_main()
{
    uart_setup();
    read_and_parse_nmea();
}

static void uart_setup()
{
    uart_config_t uart_config = {
            .baud_rate = 9600,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM,
                    UART_PIN_NO_CHANGE, UART_RX_PIN,
                    UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, UART_RX_BUF_SIZE * 2, 0, 0, NULL, 0));
}


static void read_and_parse_nmea()
{
    // Configure a temporary buffer for the incoming data
    char *buffer = (char*) malloc(UART_RX_BUF_SIZE + 1);
    char fmt_buf[32];
    size_t total_bytes = 0;
    while (1) {
        // Read data from the UART
        int read_bytes = uart_read_bytes(UART_NUM,
                (uint8_t*) buffer + total_bytes,
                UART_RX_BUF_SIZE - total_bytes, 100 / portTICK_RATE_MS);
        if (read_bytes <= 0) {
            continue;
        }

        nmea_s *data;
        total_bytes += read_bytes;

        /* find start (a dollar sign) */
        char* start = memchr(buffer, '$', total_bytes);
        if (start == NULL) {
            total_bytes = 0;
            continue;
        }

        /* find end of line */
        char* end = memchr(start, '\r', total_bytes - (start - buffer));
        if (NULL == end || '\n' != *(++end)) {
            continue;
        }
        end[-1] = NMEA_END_CHAR_1;
        end[0] = NMEA_END_CHAR_2;

        /* handle data */
        data = nmea_parse(start, end - start + 1, 0);
        if (data == NULL) {
            printf("Failed to parse the sentence!\n");
            printf("  Type: %.5s (%d)\n", start+1, nmea_get_type(start));
        } else {
            if (data->errors != 0) {
                printf("WARN: The sentence struct contains parse errors!\n");
            }

            if (NMEA_GPGGA == data->type) {
                printf("GPGGA sentence\n");
                nmea_gpgga_s *gpgga = (nmea_gpgga_s *) data;
                printf("Number of satellites: %d\n", gpgga->n_satellites);
                printf("Altitude: %f %c\n", gpgga->altitude,
                        gpgga->altitude_unit);
            }

            if (NMEA_GPGLL == data->type) {
                printf("GPGLL sentence\n");
                nmea_gpgll_s *pos = (nmea_gpgll_s *) data;
                printf("Longitude:\n");
                printf("  Degrees: %d\n", pos->longitude.degrees);
                printf("  Minutes: %f\n", pos->longitude.minutes);
                printf("  Cardinal: %c\n", (char) pos->longitude.cardinal);
                printf("Latitude:\n");
                printf("  Degrees: %d\n", pos->latitude.degrees);
                printf("  Minutes: %f\n", pos->latitude.minutes);
                printf("  Cardinal: %c\n", (char) pos->latitude.cardinal);
                strftime(fmt_buf, sizeof(fmt_buf), "%H:%M:%S", &pos->time);
                printf("Time: %s\n", fmt_buf);
            }

            if (NMEA_GPRMC == data->type) {
                printf("GPRMC sentence\n");
                nmea_gprmc_s *pos = (nmea_gprmc_s *) data;
                printf("Longitude:\n");
                printf("  Degrees: %d\n", pos->longitude.degrees);
                printf("  Minutes: %f\n", pos->longitude.minutes);
                printf("  Cardinal: %c\n", (char) pos->longitude.cardinal);
                printf("Latitude:\n");
                printf("  Degrees: %d\n", pos->latitude.degrees);
                printf("  Minutes: %f\n", pos->latitude.minutes);
                printf("  Cardinal: %c\n", (char) pos->latitude.cardinal);
                strftime(fmt_buf, sizeof(fmt_buf), "%d %b %T %Y", &pos->date_time);
                printf("Date & Time: %s\n", fmt_buf);
                printf("Speed, in Knots: %f\n", pos->gndspd_knots);
                printf("Track, in degrees: %f\n", pos->track_deg);
                printf("Magnetic Variation:\n");
                printf("  Degrees: %f\n", pos->magvar_deg);
                printf("  Cardinal: %c\n", (char) pos->magvar_cardinal);
                double adjusted_course = pos->track_deg;
                if (NMEA_CARDINAL_DIR_EAST == pos->magvar_cardinal) {
                    adjusted_course -= pos->magvar_deg;
                } else if (NMEA_CARDINAL_DIR_WEST == pos->magvar_cardinal) {
                    adjusted_course += pos->magvar_deg;
                } else {
                    printf("Invalid Magnetic Variation Direction!\n");
                }

                printf("Adjusted Track (heading): %f\n", adjusted_course);
            }

            if (NMEA_GPGSA == data->type) {
                nmea_gpgsa_s *gpgsa = (nmea_gpgsa_s *) data;

                printf("GPGSA Sentence:\n");
                printf("  Mode: %c\n", gpgsa->mode);
                printf("  Fix:  %d\n", gpgsa->fixtype);
                printf("  PDOP: %.2lf\n", gpgsa->pdop);
                printf("  HDOP: %.2lf\n", gpgsa->hdop);
                printf("  VDOP: %.2lf\n", gpgsa->vdop);
            }

            if (NMEA_GPGSV == data->type) {
                nmea_gpgsv_s *gpgsv = (nmea_gpgsv_s *) data;

                printf("GPGSV Sentence:\n");
                printf("  Num: %d\n", gpgsv->sentences);
                printf("  ID:  %d\n", gpgsv->sentence_number);
                printf("  SV:  %d\n", gpgsv->satellites);
                printf("  #1:  %d %d %d %d\n", gpgsv->sat[0].prn, gpgsv->sat[0].elevation, gpgsv->sat[0].azimuth, gpgsv->sat[0].snr);
                printf("  #2:  %d %d %d %d\n", gpgsv->sat[1].prn, gpgsv->sat[1].elevation, gpgsv->sat[1].azimuth, gpgsv->sat[1].snr);
                printf("  #3:  %d %d %d %d\n", gpgsv->sat[2].prn, gpgsv->sat[2].elevation, gpgsv->sat[2].azimuth, gpgsv->sat[2].snr);
                printf("  #4:  %d %d %d %d\n", gpgsv->sat[3].prn, gpgsv->sat[3].elevation, gpgsv->sat[3].azimuth, gpgsv->sat[3].snr);
            }

            if (NMEA_GPTXT == data->type) {
                nmea_gptxt_s *gptxt = (nmea_gptxt_s *) data;

                printf("GPTXT Sentence:\n");
                printf("  ID: %d %d %d\n", gptxt->id_00, gptxt->id_01, gptxt->id_02);
                printf("  %s\n", gptxt->text);
            }

            if (NMEA_GPVTG == data->type) {
                nmea_gpvtg_s *gpvtg = (nmea_gpvtg_s *) data;

                printf("GPVTG Sentence:\n");
                printf("  Track [deg]:   %.2lf\n", gpvtg->track_deg);
                printf("  Speed [kmph]:  %.2lf\n", gpvtg->gndspd_kmph);
                printf("  Speed [knots]: %.2lf\n", gpvtg->gndspd_knots);
            }

            nmea_free(data);
        }

        /* buffer empty? */
        if (end == buffer + total_bytes) {
            total_bytes = 0;
            continue;
        }

        /* copy rest of buffer to beginning */
        if (buffer != memmove(buffer, end, total_bytes - (end - buffer))) {
            total_bytes = 0;
            continue;
        }

        total_bytes -= end - buffer;
    }
    free(buffer);
}
