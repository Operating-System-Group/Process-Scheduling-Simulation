import java.io.*;
import java.util.*;

class Process {
    int pid, arrival, burst, priority;

    public Process(int pid, int arrival, int burst, int priority) {
        this.pid = pid;
        this.arrival = arrival;
        this.burst = burst;
        this.priority = priority;
    }
}

public class PriorityScheduling {
    public static void main(String[] args) {
        List<Process> processes = readProcesses("processes.txt");
        priorityScheduling(processes);
    }

    static List<Process> readProcesses(String filename) {
        List<Process> processes = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            String line;
            br.readLine(); // Skip header
            while ((line = br.readLine()) != null) {if (line.trim().isEmpty() || line.contains("PID")) continue; // skip header or empty lines
                String[] parts = line.split("\\s+");
                processes.add(new Process(
                    Integer.parseInt(parts[0]), 
                    Integer.parseInt(parts[1]), 
                    Integer.parseInt(parts[2]), 
                    Integer.parseInt(parts[3])
                ));
            }
        } catch (IOException e) {
            System.err.println("Error reading file.");
        }
        return processes;
    }

    static void priorityScheduling(List<Process> processes) {
        processes.sort(Comparator.comparingInt(p -> p.arrival)); // Sort by arrival time

        int time = 0;
        List<String> gantt = new ArrayList<>();
        Map<Integer, Integer> waitingTime = new HashMap<>();
        Map<Integer, Integer> turnaroundTime = new HashMap<>();
        boolean[] completed = new boolean[processes.size()];
        int completedCount = 0;

        while (completedCount < processes.size()) {
            List<Process> available = new ArrayList<>();
            for (Process p : processes) {
                if (p.arrival <= time && !completed[p.pid - 1]) {
                    available.add(p);
                }
            }

            if (!available.isEmpty()) {
                available.sort(Comparator.comparingInt(p -> p.priority)); // Sort by priority
                Process current = available.get(0);

                gantt.add("| P" + current.pid + " ");
                waitingTime.put(current.pid, time - current.arrival);
                turnaroundTime.put(current.pid, waitingTime.get(current.pid) + current.burst);

                time += current.burst;
                completed[current.pid - 1] = true;
                completedCount++;
            } else {
                time++;
            }
        }

        // Print Gantt Chart
        System.out.println("\nGantt Chart:");
        gantt.forEach(System.out::print);
        System.out.println("|\n");

        // Print Waiting Time & Turnaround Time
        double totalWT = 0, totalTAT = 0;
        System.out.println("Waiting Time (WT):");
        for (var entry : waitingTime.entrySet()) {
            System.out.println("P" + entry.getKey() + ": " + entry.getValue());
            totalWT += entry.getValue();
        }
        System.out.println("Turnaround Time (TAT):");
        for (var entry : turnaroundTime.entrySet()) {
            System.out.println("P" + entry.getKey() + ": " + entry.getValue());
            totalTAT += entry.getValue();
        }

        // Print Averages
        System.out.println("Average Waiting Time: " + (totalWT / waitingTime.size()));
        System.out.println("Average Turnaround Time: " + (totalTAT / turnaroundTime.size()));
    }
}
