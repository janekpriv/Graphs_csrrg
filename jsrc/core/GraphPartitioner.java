package core;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.concurrent.ExecutionException;
import javax.swing.*;



public class GraphPartitioner {

    public static Process partitionProcess; // To store the running process

    private static boolean wasCancelled = false;

    public static void runGraphPart(String filePath, final JLabel mainLabel, JButton stopButton, JButton downloadButton) {
        wasCancelled = false; // reset before running
        downloadButton.setVisible(false);
        SwingWorker<Void, String> worker = new SwingWorker<>() {
            @Override
            protected Void doInBackground() throws Exception {
                //publish("Processing...");
                ProcessBuilder builder = new ProcessBuilder("./program", filePath);
                partitionProcess = builder.start();

                SwingUtilities.invokeLater(() -> stopButton.setVisible(true));

                int exitCode = partitionProcess.waitFor();

                if (wasCancelled) {
                    publish("Process cancelled.");
                } else if (exitCode != 0) {
                    BufferedReader errorReader = new BufferedReader(new InputStreamReader(partitionProcess.getErrorStream()));
                    StringBuilder errorOutput = new StringBuilder();
                    String line;
                    while ((line = errorReader.readLine()) != null) {
                        errorOutput.append(line).append("\n");
                    }
                    publish("Error: " + errorOutput.toString());
                } else {
                    publish("Partitioning Complete.");
                }

                return null;
            }

            @Override
            protected void process(java.util.List<String> chunks) {
                for (String chunk : chunks) {
                    mainLabel.setText("<html>" + chunk.replace("\n", "<br>") + "</html>");
                }
            }

            @Override

            protected void done() {
                try {
                    get(); // Wait for completion (if any exception occurs in doInBackground)
                } catch (InterruptedException | ExecutionException e) {
                    e.printStackTrace();
                }
                
                // Once he process finishes, show the "Download" buttont
                if (wasCancelled == false)
                    downloadButton.setVisible(true);  // Make the download button visible after processing
                stopButton.setVisible(false);    // Hide the stop button after processing
            }
        };

        worker.execute();
    }

    public static void stopProcess(JButton stopButton) {
        if (partitionProcess != null) {
            wasCancelled = true;
            partitionProcess.destroy(); // Terminate the process
            if (stopButton != null) {
                stopButton.setVisible(false); // Hide stop button after process is stopped
            }
        }
    }
}
