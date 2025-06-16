package com.riot.matesense.service;

import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

import com.riot.matesense.enums.Status;
import lombok.Getter;
import lombok.Setter;

@Component
public class ConfidenceCalculator //retooled to work within existing framework
{
    @Getter
    int confidence;
    Status prevStatus;

    public ConfidenceCalculator()
    {
        confidence = 100;
        prevStatus = Status.UNKNOWN;
    }

    public int updateConfidence(Status status)
    {
        if (status == Status.UNKNOWN)
        {
            confidence = 100; //if we don't know, we're sure that we don't know
        }
        else
        {
            if(status == prevStatus) //update our confidence based on whether it matches the previous report
            {
                confidence += 50;
            }
            else
            {
                confidence -= 50;
            }
        }

        prevStatus = status;

        confidence = Math.max(0, confidence);
        confidence = Math.min(100, confidence); // normalization, we can't be more than 100% sure or less than 0% sure of the gate's status

        return confidence;
    }

    @Scheduled(fixedRate = 21600000) // six hours in milliseconds
    public void subtractConfidence()
    {
        confidence -= 5; //subtract five percent every six hours
    }
}