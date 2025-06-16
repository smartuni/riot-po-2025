package com.riot.matesense.service;

import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

import com.riot.matesense.enums.Status;
import com.riot.matesense.enums.ConfidenceQuality;
import lombok.Getter;

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


    public ConfidenceQuality DetermineQuality {
        
        if (confidence >= 90){
            return ConfidenceQuality.HIGH;
        }

        else if (confidence > 80 && confidence < 90){
            return ConfidenceQuality.MED_HIGH;
        }

        else if (confidence > 70 && confidence < 80){
            return ConfidenceQuality.MED;
        }

        else if (confidence > 60 && confidence < 70){
            return ConfidenceQuality.MED_LOW;
        }

        else {
            return ConfidenceQuality.LOW;
        }

    }
}