import java.util.Scanner;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/*
public class Main {

    public static void main(String[] args) {
        ConfidenceCalculator calculator = new ConfidenceCalculator();

        // Setup scheduler to run subtractConfidence every 6 hours (or use 10 seconds for demo)
        ScheduledExecutorService scheduler = Executors.newScheduledThreadPool(1);
        Runnable task = () -> calculator.subtractConfidence();
        scheduler.scheduleAtFixedRate(task, 0, 6, TimeUnit.HOURS); // Change to SECONDS for testing

        // Allow user to input new reports
        Scanner scanner = new Scanner(System.in);
        System.out.println("Enter a report type (OPEN, CLOSED, UNKNOWN), or type 'exit' to quit:");

        while (true) {
            System.out.print("Your input: ");
            String input = scanner.nextLine().trim();

            if (input.equalsIgnoreCase("exit")) {
                break;
            }

            float updated = calculator.updateConfidence(input);
            System.out.println("Updated confidence: " + updated);
        }

        // Cleanup
        System.out.println("Shutting down...");
        scheduler.shutdown();
        scanner.close();
    }
}
*/
public class ConfidenceCalculator { // prototype/proof of concept for confidence calculator
    
    float confidence;
    String lastReport; 
    public ConfidenceCalculator(){
        confidence = 1.0;
        lastReport = "None";
    }
    
    public float getConfidence() {
        return confidence;
    }
    
    public void SubtractConfidence() {
        if (!lastReport.equals("None") && !lastReport.equals("UNKNOWN")){
            confidence -= 0.5;
        }
    };
    
    public float updateConfidence(String report)
    {
        
        if (report.equals("UNKNOWN")){
            
            confidence = 1.0;
        }
        else if (report.equals("OPEN") || report.equals("CLOSED"))
        {
            if(lastReport.equals("None")){
                confidence = 0.5;
            }
            else {
                if(report.equals(lastReport)){
                    confidence += 0.5;
                }
                else{
                    confidence -= 0.5;
                }
            }
        } 
        lastReport = report; 
        if (confidence < 0){
            confidence = 0;
        }
        else if (confidence > 1){
            confidence = 1;
        }
        return confidence;
    }
    
}