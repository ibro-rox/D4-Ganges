/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License.
 *          View this license at http://creativecommons.org/about/licenses/
 */

#include <util/twi.h>

#define I2C_READ    1
#define I2C_WRITE   0

void init_i2c_master(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_tx(uint8_t b);
uint8_t i2c_rx_ack(void);
uint8_t i2c_rx_nack(void);
