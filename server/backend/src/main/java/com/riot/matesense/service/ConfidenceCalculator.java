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
    Status gateStatusArray[];
    Status workerStatusArray[];

    public ConfidenceCalculator()
    {
        confidence = 100;
        prevStatus = Status.NONE;
        for(int i = 0; i < 5; i++)
        {
            gateStatusArray[i] = Status.NONE;
            workerStatusArray[i] = Status.NONE;
        }
    }

    public void shuffleConfidence(Status status) // will need to update once we know the origin of a report
    {
        for(int i = 1; i < 5; i++)
        {
            gateStatusArray[i] = gateStatusArray[i-1];
            workerStatusArray[i] = workerStatusArray[i-1];
        }

        gateStatusArray[0] = status;
        workerStatusArray[0] = status;
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

    public int updateConfidenceArray(Status status)
    {
        if (status == Status.UNKNOWN || status == Status.NONE)
        {
            confidence = 100;
        }
        else
        {
            for(int i = 0; i < 5; i++)
            {
                if (status == gateStatusArray[i] && gateStatusArray[i] != Status.NONE)
                {
                    confidence += 10;
                }
                else
                {
                    confidence -= 10;
                }

                if (status == workerStatusArray[i] && workerStatusArray[i] != Status.NONE)
                {
                    confidence += 15;
                }
                else
                {
                    confidence -= 15;
                }
            }
        }

        shuffleConfidence(status);

        confidence = Math.max(0, confidence);
        confidence = Math.min(100, confidence);

        return confidence;
    }

    @Scheduled(fixedRate = 21600000) // six hours in milliseconds
    public void subtractConfidence()
    {
        confidence -= 5; //subtract five percent every six hours
    }


    public ConfidenceQuality determineQuality() {
        
        if (confidence >= 90){
            return ConfidenceQuality.HIGH;
        }

        else if (confidence >= 80 && confidence < 90){
            return ConfidenceQuality.MED_HIGH;
        }

        else if (confidence >= 70 && confidence < 80){
            return ConfidenceQuality.MED;
        }

        else if (confidence >= 60 && confidence < 70){
            return ConfidenceQuality.MED_LOW;
        }

        else {
            return ConfidenceQuality.LOW;
        }

    }
}