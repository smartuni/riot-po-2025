package com.riot.matesense.service;
import com.riot.matesense.config.MqttProperties;
import com.riot.matesense.mqtt.TTNMqttPublisher;
import org.springframework.stereotype.Service;

import java.time.Instant;
import java.util.Arrays;
import java.util.Base64;
import java.util.List;
import java.util.Map;

//small example for some downlinks
//TODO: need to connect downlinks with request from the frontend
//TODO need to clean up a bit and connect with node team to
@Service
public class DownlinkService {

    private final TTNMqttPublisher mqttPublisher;
    private final CborConverter cborConverter;
    private final MqttProperties mqttProperties;

    public DownlinkService(TTNMqttPublisher mqttPublisher, CborConverter cborConverter, MqttProperties mqttProperties) {
        this.mqttPublisher = mqttPublisher;
        this.cborConverter = cborConverter;
        this.mqttProperties = mqttProperties;
    }
//    @SuppressWarnings("unchecked")
//    public void sendDownlinkToDevice(String deviceId, Map<String, Object> payloadData) {
//        //TODO need to check the right time
//        int timestamp = (int) Instant.now().getEpochSecond();
//        //Example if the payload is json format with keys
//        //TODO Change if payload is different (like raw bytes)
//        try {
//            //append Type
//            int messageType = (Integer) payloadData.get("messageType");
//            var statuses = (Iterable<Map<String, Object>>) payloadData.get("statuses");
//            //Create List for Gatee Statuses
//            var statusList = new java.util.ArrayList<>();
//            for (Map<String, Object> entry : statuses) {
//                int gateId = (Integer) entry.get("gateId");
//                int status = (Integer) entry.get("status");
//                statusList.add(java.util.Arrays.asList(gateId, status));
//            }
//
//            var finalPayload = java.util.Arrays.asList(
//                    messageType,
//                    timestamp,
//                    statusList
//            );
//
//            byte[] cbor = cborConverter.toCbor(finalPayload);
//            String base64 = Base64.getEncoder().encodeToString(cbor);
//
//            String topic = String.format("v3/%s/devices/%s/down/push",
//                    mqttProperties.getApplicationId(), deviceId);
//
//            String json = String.format("""
//        {
//          "downlinks": [
//            {
//              "f_port": 15,
//              "frm_payload": "%s",
//              "priority": "NORMAL"
//            }
//          ]
//        }
//        """, base64);
//
//            mqttPublisher.publishDownlink(json.getBytes(), topic);
//
//        } catch (Exception e) {
//            System.err.println("Fehler beim Senden des Downlinks: " + e.getMessage());
//        }
//    }

//    public void sendDownlinkToDevice(String deviceId, Map<String, Object> payloadData) {
//        //int timestamp = (int) Instant.now().getEpochSecond();
//        //TODO add Timestamp into the payload [
//        //    1,    # 1 is an example value for the message type
//        //    247,  # Timestamp
//        //    [     # The list with the "Soll Status" entries
//        //        [ # This is a "Soll Status" entry
//        //            187, # GateID
//        //            0,   # Soll Status
//        //        ],
//        //        [ # 2nd "Soll Status" entry
//        //            69,  # GateID
//        //            1,   # Soll Status
//        //        ]
//        //        # ... More entries
//        //    ]
//        //]
//        try {
//            byte[] cbor = cborConverter.toCbor(payloadData);
//            String base64 = Base64.getEncoder().encodeToString(cbor);
//
//            String topic = String.format("v3/%s@ttn/devices/%s/down/push", mqttProperties.getApplicationId(), deviceId);
//
//            String json = String.format("""
//            {
//              "downlinks": [
//                {
//                  "f_port": 15,
//                  "frm_payload": "%s",
//                  "priority": "NORMAL"
//                }
//              ]
//            }
//            """, base64);
//
//            mqttPublisher.publishDownlink(json.getBytes(), topic);
//
//        } catch (Exception e) {
//            System.err.println("Fehler beim Senden des Downlinks: " + e.getMessage());
//        }
//    }
    //example
    public void sendeDownlink() {
        try {
            //small payload examplee
            List<Object> payload = Arrays.asList(
                    1,
                    247,
                    Arrays.asList(
                            Arrays.asList(187, 0),
                            Arrays.asList(69, 1)
                    )
            ); // my payload that convert to a string base64
            byte[] cbor = cborConverter.toCbor(payload);
            String base64 = Base64.getEncoder().encodeToString(cbor);
            String json = String.format("""
                {
                  "downlinks": [
                    {
                      "f_port": 15,
                      "frm_payload": "%s",
                      "priority": "NORMAL"
                    }
                  ]
                }
                """, base64);

            mqttPublisher.publishDownlink(json.getBytes(), mqttProperties.getPublishTopic());
        } catch (Exception e) {
            System.err.println("Fehler beim Senden: " + e.getMessage());
        }
    }

}

