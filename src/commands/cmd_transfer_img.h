#pragma once

#include <Arduino.h>
#include "uart_commands.h"

namespace {
    constexpr int CHUNK_SIZE = 256;
    constexpr int CHECKSUM_SIZE = 1;
    constexpr int DATA_SIZE = CHUNK_SIZE - CHECKSUM_SIZE;
    constexpr int TIMEOUT_MS = 5000;
    constexpr uint8_t ACK = 0x06;
    constexpr uint8_t NACK = 0x15;

    HardwareSerial& transferSerial = Serial2;

    uint8_t calculate_checksum(const uint8_t* data, size_t length) {
        uint8_t checksum = 0;
        for (size_t i = 0; i < length; i++) {
            checksum ^= data[i];
        }
        return checksum;
    }

    command_result_t wait_for_ack() {
        unsigned long start_time = millis();
        while (millis() - start_time < TIMEOUT_MS) {
            if (transferSerial.available()) {
                uint8_t response = transferSerial.read();
                if (response == ACK) return CMD_OK;
                if (response == NACK) return CMD_ERROR;
            }
        }
        return CMD_TIMEOUT;
    }

    bool send_with_retry(const uint8_t* data, size_t length, int max_retries = 3) {
        for (int i = 0; i <= max_retries; i++) {
            transferSerial.write(data, length);
            if (wait_for_ack() == CMD_OK) return true;
        }
        return false;
    }

    void send_tag(const char* tag, bool is_closing = false) {
        transferSerial.print('<');
        if (is_closing) transferSerial.print('/');
        transferSerial.print(tag);
        transferSerial.print('>');
    }

    bool send_header(uint32_t img_size) {
        uint8_t header[] = {0xAA, 0x55, 
                           (uint8_t)(img_size >> 24), 
                           (uint8_t)(img_size >> 16),
                           (uint8_t)(img_size >> 8), 
                           (uint8_t)img_size};
        
        send_tag("HEADER");
        bool result = send_with_retry(header, sizeof(header));
        send_tag("HEADER", true);
        return result;
    }

    bool send_chunk(const uint8_t* img_data, uint32_t offset, uint32_t bytes_to_send) {
        uint8_t chunk[CHUNK_SIZE];
        memcpy(chunk, img_data + offset, bytes_to_send);
        chunk[bytes_to_send] = calculate_checksum(chunk, bytes_to_send);
        
        send_tag("DATA");
        bool result = send_with_retry(chunk, bytes_to_send + CHECKSUM_SIZE);
        send_tag("DATA", true);
        return result;
    }
}

command_result_t cmd_transfer_img(
    std::string *args, 
    uint8_t arg_count,
    std::string *response) 
{
    // TODO: load real image data
    uint32_t img_size = 1000;
    uint8_t *img_data = (uint8_t *)malloc(img_size);
    if (!img_data) return CMD_ERROR;
    
    // Generate dummy data
    for (uint32_t i = 0; i < img_size; i++) {
        img_data[i] = i % 256;
    }

    // Send header
    if (!send_header(img_size)) {
        free(img_data);
        return CMD_ERROR;
    }

    // Send chunks
    uint32_t chunks_needed = (img_size + DATA_SIZE - 1) / DATA_SIZE;
    for (uint32_t chunk_idx = 0; chunk_idx < chunks_needed; chunk_idx++) {
        uint32_t offset = chunk_idx * DATA_SIZE;
        uint32_t bytes_to_send = min(DATA_SIZE, img_size - offset);
        
        if (!send_chunk(img_data, offset, bytes_to_send)) {
            free(img_data);
            return CMD_ERROR;
        }
    }

    free(img_data);
    return CMD_OK;
}
