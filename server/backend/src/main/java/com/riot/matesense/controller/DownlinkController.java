package com.riot.matesense.controller;

import com.riot.matesense.service.DownlinkService;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Map;
//NOTE: check again and rework if needed
@RestController
@RequestMapping("/api/downlink")
public class DownlinkController {

    private final DownlinkService downlinkService;

    public DownlinkController(DownlinkService downlinkService) {
        this.downlinkService = downlinkService;
    }
    @PostMapping("/{deviceId}")
    public ResponseEntity<String> sendDownlink(@PathVariable String deviceId, @RequestBody Map<String, Object> payload) {
        try {
            //downlinkService.sendDownlinkToDevice(deviceId, payload); // this need to be tested
            return ResponseEntity.ok("Downlink vorbereitet.");
        } catch (Exception e) {
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body("Fehler: " + e.getMessage());
        }
    }
}
