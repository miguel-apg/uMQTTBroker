/*
 * File:   mqtt_msg.h
 * Author: Minh Tuan
 *
 * Created on July 12, 2014, 1:05 PM
 */

#ifndef MQTT_MSG_H
#define MQTT_MSG_H
#include "user_config.h"
#include "./defaults.h"
#include "c_types.h"
#ifdef  __cplusplus
extern "C" {
#endif

/*
* Copyright (c) 2014, Stephen Robinson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holder nor the names of its
* contributors may be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*/
/* 7      6     5     4     3     2     1     0*/
/*|      --- Message Type----     |  DUP Flag |    QoS Level    | Retain  |
/*                    Remaining Length                 */

enum mqtt_message_type
{
  MQTT_MSG_TYPE_CONNECT = 1,
  MQTT_MSG_TYPE_CONNACK = 2,
  MQTT_MSG_TYPE_PUBLISH = 3,
  MQTT_MSG_TYPE_PUBACK = 4,
  MQTT_MSG_TYPE_PUBREC = 5,
  MQTT_MSG_TYPE_PUBREL = 6,
  MQTT_MSG_TYPE_PUBCOMP = 7,
  MQTT_MSG_TYPE_SUBSCRIBE = 8,
  MQTT_MSG_TYPE_SUBACK = 9,
  MQTT_MSG_TYPE_UNSUBSCRIBE = 10,
  MQTT_MSG_TYPE_UNSUBACK = 11,
  MQTT_MSG_TYPE_PINGREQ = 12,
  MQTT_MSG_TYPE_PINGRESP = 13,
  MQTT_MSG_TYPE_DISCONNECT = 14
};

enum mqtt_connect_return_code
{
  CONNECTION_ACCEPTED = 0,
  CONNECTION_REFUSE_PROTOCOL,
  CONNECTION_REFUSE_ID_REJECTED,
  CONNECTION_REFUSE_SERVER_UNAVAILABLE,
  CONNECTION_REFUSE_BAD_USERNAME,
  CONNECTION_REFUSE_NOT_AUTHORIZED
};

typedef struct mqtt_message
{
  uint8_t* data;
  uint16_t length;

} mqtt_message_t;

typedef struct mqtt_connection
{
  mqtt_message_t message;

  uint16_t message_id;
  uint8_t* buffer;
  uint16_t buffer_length;

} mqtt_connection_t;

typedef struct mqtt_connect_info
{
  char* client_id;
  char* username;
  char* password;
  char* will_topic;
  char* will_data;
  uint16_t will_data_len;
  uint32_t keepalive;
  int will_qos;
  int will_retain;
  int clean_session;

} mqtt_connect_info_t;

#define MQTT_MAX_FIXED_HEADER_SIZE 3

enum mqtt_connect_flag
{
  MQTT_CONNECT_FLAG_USERNAME = 1 << 7,
  MQTT_CONNECT_FLAG_PASSWORD = 1 << 6,
  MQTT_CONNECT_FLAG_WILL_RETAIN = 1 << 5,
  MQTT_CONNECT_FLAG_WILL = 1 << 2,
  MQTT_CONNECT_FLAG_CLEAN_SESSION = 1 << 1
};

struct __attribute((__packed__)) mqtt_connect_variable_header
{
  uint8_t lengthMsb;
  uint8_t lengthLsb;
#if defined(PROTOCOL_NAMEv31)
  uint8_t magic[6];
#elif defined(PROTOCOL_NAMEv311)
  uint8_t magic[4];
#else
#error "Please define protocol name"
#endif
  uint8_t version;
  uint8_t flags;
  uint8_t keepaliveMsb;
  uint8_t keepaliveLsb;
};

struct __attribute((__packed__)) mqtt_connect_variable_header3
{
  uint8_t lengthMsb;
  uint8_t lengthLsb;
  uint8_t magic[6];
  uint8_t version;
  uint8_t flags;
  uint8_t keepaliveMsb;
  uint8_t keepaliveLsb;
};

struct __attribute((__packed__)) mqtt_connect_variable_header4
{
  uint8_t lengthMsb;
  uint8_t lengthLsb;
  uint8_t magic[4];
  uint8_t version;
  uint8_t flags;
  uint8_t keepaliveMsb;
  uint8_t keepaliveLsb;
};

static inline int ICACHE_FLASH_ATTR mqtt_get_type(uint8_t* buffer) { return (buffer[0] & 0xf0) >> 4; }
static inline int ICACHE_FLASH_ATTR mqtt_get_connect_return_code(uint8_t* buffer) { return buffer[3]; }
static inline int ICACHE_FLASH_ATTR mqtt_get_dup(uint8_t* buffer) { return (buffer[0] & 0x08) >> 3; }
static inline int ICACHE_FLASH_ATTR mqtt_get_qos(uint8_t* buffer) { return (buffer[0] & 0x06) >> 1; }
static inline int ICACHE_FLASH_ATTR mqtt_get_retain(uint8_t* buffer) { return (buffer[0] & 0x01); }

void ICACHE_FLASH_ATTR mqtt_msg_init(mqtt_connection_t* connection, uint8_t* buffer, uint16_t buffer_length);
int ICACHE_FLASH_ATTR mqtt_get_total_length(uint8_t* buffer, uint16_t length);
char* ICACHE_FLASH_ATTR mqtt_get_str(uint8_t* buffer, uint16_t* length);
const char* ICACHE_FLASH_ATTR mqtt_get_publish_topic(uint8_t* buffer, uint16_t* length);
const char* ICACHE_FLASH_ATTR mqtt_get_publish_data(uint8_t* buffer, uint16_t* length);
uint16_t ICACHE_FLASH_ATTR mqtt_get_id(uint8_t* buffer, uint16_t length);

mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_connect(mqtt_connection_t* connection, mqtt_connect_info_t* info);
mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_connack(mqtt_connection_t* connection, enum mqtt_connect_return_code retcode);
mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_publish(mqtt_connection_t* connection, const char* topic, const char* data, int data_length, int qos, int retain, uint16_t* message_id);
mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_puback(mqtt_connection_t* connection, uint16_t message_id);
mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_pubrec(mqtt_connection_t* connection, uint16_t message_id);
mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_pubrel(mqtt_connection_t* connection, uint16_t message_id);
mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_pubcomp(mqtt_connection_t* connection, uint16_t message_id);
mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_subscribe(mqtt_connection_t* connection, const char* topic, int qos, uint16_t* message_id);
mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_suback(mqtt_connection_t* connection, uint8_t *ret_codes, uint8_t ret_codes_len, uint16_t message_id);
mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_unsubscribe(mqtt_connection_t* connection, const char* topic, uint16_t* message_id);
mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_unsuback(mqtt_connection_t* connection, uint16_t message_id);
mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_pingreq(mqtt_connection_t* connection);
mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_pingresp(mqtt_connection_t* connection);
mqtt_message_t* ICACHE_FLASH_ATTR mqtt_msg_disconnect(mqtt_connection_t* connection);


#ifdef  __cplusplus
}
#endif

#endif  /* MQTT_MSG_H */

