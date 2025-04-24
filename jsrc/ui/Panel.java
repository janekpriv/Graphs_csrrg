package ui;
import core.GraphPartitioner;
import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Desktop;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;


public class Panel {
    private Panel() {} // prevent from making an instance

    private static final String MAIN_PANEL = "MainPanel";
    private static final String HELP_PANEL = "HelpPanel";
    private static JLabel mainLabel = new JLabel("Load .csrrg file");
    private static JButton stopButton;
    private static JButton downloadButton;


    public static void setupPanel(final JFrame frame){
        // CardLayout container
        JPanel cardPanel = new JPanel(new CardLayout());

        // Create panels
        JPanel mainPanel = createMainPanel(frame, cardPanel);
        JPanel helpPanel = createHelpPanel(cardPanel);

        // Add panels to the CardLayout container
        cardPanel.add(mainPanel, MAIN_PANEL);
        cardPanel.add(helpPanel, HELP_PANEL);

        JMenuBar mb = setupMenuBar(cardPanel, frame);
        
        frame.setJMenuBar(mb);
        frame.getContentPane().add(cardPanel);

        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                // Stop the process when the window is closing
                if (GraphPartitioner.partitionProcess != null) {
                    GraphPartitioner.stopProcess(null); // Stop the process when exiting
                }
                System.exit(0); // Exit the program after stopping the process
            }
        });
    
    }

    
    
    
    public static JPanel createMainPanel(JFrame frame, JPanel cardPanel) {
        JPanel panel = new JPanel(new GridBagLayout());
        panel.setBackground(Color.WHITE); 
        // Create constraints to align components within the GridBagLayout
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0;  // Center the components in the X axis
        gbc.gridy = 0;  // Start from the top in the Y axis
        gbc.anchor = GridBagConstraints.CENTER;  // Center components horizontally and vertically

        // Add the mainLabel to the panel
        mainLabel.setFont(new Font("SansSerif", Font.PLAIN, 24)); 

        panel.add(mainLabel, gbc);

        // Add some space between the mainLabel and the buttons
        gbc.gridy++;
        panel.add(Box.createVerticalStrut(10), gbc);

        // Create and configure the stopButton
        stopButton = new JButton("Cancel");
        stopButton.setVisible(false);  // Initially hide the button
        gbc.gridy++;
        panel.add(stopButton, gbc);  // Add stopButton under the mainLabel

        // Create and configure the downloadButton
        downloadButton = new JButton("Download Result");
        downloadButton.setVisible(false);  // Initially hide the button
        gbc.gridy++;
        panel.add(downloadButton, gbc);  // Add downloadButton below stopButton

        // Add the panel to the center of the frame's content
        frame.getContentPane().add(panel, BorderLayout.CENTER);

        downloadButton.addActionListener(e -> {
            JFileChooser folderChooser = new JFileChooser();
            folderChooser.setDialogTitle("Select Download Folder");
            folderChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
            folderChooser.setAcceptAllFileFilterUsed(false);

            int returnValue = folderChooser.showSaveDialog(frame); 
            if (returnValue == JFileChooser.APPROVE_OPTION) {
                File targetFolder = folderChooser.getSelectedFile();
                handleDownloadOfLatestFolder(frame, targetFolder);
            }
            });

        stopButton.addActionListener(e -> {
            // Call stopProcess to stop the background task
            GraphPartitioner.stopProcess(stopButton);
        });

        return panel;
    }

    public static JPanel createHelpPanel(JPanel cardPanel) {
        JPanel panel = new JPanel(new BorderLayout());
        
        String htmlContent = """
            <html>
                
                    <h1>Podział grafu</h1>
                    <p>Ten program rozwiązuje problem podziału ogólnokrajowej sieci logistycznej
                    firmy kurierskiej na trzy niezależne regiony operacyjne.</p>
                    <p> </p>
                    <p>Podział grafu wykonywany jest za pomocą algorytmu <b>Louvain</b>, który identyfikuje społeczności w grafie, minimalizując połączenia między nimi.</p>
                    <p><img src='file:res/louvain_partr.gif'></p>
                    <p><i>Ilustracja działania algorytmu Louvain</i></p>
                    <p> </p>
                    <p> </p>
                    <p>Aby rozpocząć, kliknij w menu: <b>File > Load File</b>.</p>
                </body>
            </html>
            """;

        JLabel label = new JLabel(htmlContent);
        label.setFont(new Font("SansSerif", Font.PLAIN, 16)); 
        label.setVerticalAlignment(SwingConstants.TOP);
        label.setOpaque(true);
        label.setBackground(Color.WHITE);
        
        // Use GridBagLayout for centering the label
        JPanel centerPanel = new JPanel(new GridBagLayout());
        centerPanel.setBackground(Color.WHITE);
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.anchor = GridBagConstraints.CENTER;  // Center the label
        centerPanel.add(label, gbc);  // Directly add the label instead of scrollPane
        
        // Create the back button
        JButton backButton = new JButton("← Wstecz");
        backButton.addActionListener(e -> {
            CardLayout cl = (CardLayout) cardPanel.getLayout();
            cl.show(cardPanel, MAIN_PANEL);
        });
        
        // Create the top bar with back button
        JPanel topBar = new JPanel(new FlowLayout(FlowLayout.LEFT));
        topBar.add(backButton);
        topBar.setBackground(Color.WHITE);
        
        panel.add(topBar, BorderLayout.NORTH);
        panel.add(centerPanel, BorderLayout.CENTER);  // Use centerPanel to center the label
        return panel;
    }
    
    
    public static JMenuBar setupMenuBar(JPanel cardPanel, JFrame frame){
        JMenuBar mb = new JMenuBar();

        JMenu m1 = new JMenu("FILE");
        JMenu m2 = new JMenu("Help");
       
        JMenuItem m11 = new JMenuItem("Load File");
        JMenuItem m21 = new JMenuItem("About");

        mb.add(m1);
        mb.add(m2);
        m1.add(m11);
        m2.add(m21);

        m11.addActionListener(e -> chooseFile(frame));
        // Help menu action switches to Help panel
        m21.addActionListener(e -> {
            CardLayout cl = (CardLayout) cardPanel.getLayout();
            cl.show(cardPanel, HELP_PANEL);
        });

        return mb;
    }


    private static void chooseFile(JFrame frame) {
        JFileChooser fileChooser = new JFileChooser(new File("."));

        // Add file filter
        fileChooser.setAcceptAllFileFilterUsed(false); // disables "All files (*.*)"
        fileChooser.setFileFilter(new FileNameExtensionFilter("CSRRG files", "csrrg"));

        int option = fileChooser.showOpenDialog(frame);
        if (option == JFileChooser.APPROVE_OPTION) {
            File sourceFile = fileChooser.getSelectedFile();
            if (!sourceFile.getName().toLowerCase().endsWith(".csrrg")) {
                JOptionPane.showMessageDialog(frame, "Invalid file type. Please select a .csrrg file.", "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }
            
            long fileSizeKB = sourceFile.length() / 1024;
            String fileInfo = String.format("Processing %s (%d KB)...", sourceFile.getName(), fileSizeKB);
            mainLabel.setText(fileInfo);
            try {
                // Create input directory if it doesn't exist
                File inputDir = new File("input");
                if (!inputDir.exists()) {
                    inputDir.mkdir();
                }

                // Target path inside input/
                Path targetPath = Path.of("input", sourceFile.getName());

                // Copy the file to input/ (overwrite if exists)
                Files.copy(sourceFile.toPath(), targetPath, StandardCopyOption.REPLACE_EXISTING);

                GraphPartitioner.runGraphPart(targetPath.toString(), mainLabel, stopButton, downloadButton);

            } catch (Exception ex) {
                mainLabel.setText("Failed to copy file: " + ex.getMessage());
                ex.printStackTrace();
            }

        } else {
            //mainLabel.setText("Open command canceled");
        }
    }

    private static void handleDownloadOfLatestFolder(JFrame frame, File targetFolder) {
        try {
            File outputDir = new File("output");
            File latestFolder = getLatestFolder(outputDir); // Find the latest folder
    
            if (latestFolder != null) {
                // Create a destination directory inside the selected folder
                File destinationDir = new File(targetFolder, latestFolder.getName());
                if (!destinationDir.exists()) {
                    destinationDir.mkdir();  // Create the destination directory if it doesn't exist
                }
    
                // Copy the contents of the latest folder to the selected folder
                copyDirectory(latestFolder, destinationDir);
                JOptionPane.showMessageDialog(frame, "Download completed!", "Success", JOptionPane.INFORMATION_MESSAGE);
                // Open folder after download
                if (Desktop.isDesktopSupported()) {
                    try {
                        Desktop.getDesktop().open(destinationDir);
                        
                    } catch (IOException ex) {
                        ex.printStackTrace();
                        JOptionPane.showMessageDialog(frame, "Failed to open folder.", "Error", JOptionPane.ERROR_MESSAGE);
                    }
                }
            } else {
                JOptionPane.showMessageDialog(frame, "No folder found to download.", "Error", JOptionPane.ERROR_MESSAGE);
            }
        } catch (IOException ex) {
            ex.printStackTrace();
            JOptionPane.showMessageDialog(frame, "Failed to copy the folder.", "Error", JOptionPane.ERROR_MESSAGE);
        }
    }
    
    private static File getLatestFolder(File outputDir) {
        File[] folders = outputDir.listFiles(File::isDirectory); // Get all directories in the output folder
        if (folders == null || folders.length == 0) {
            return null;
        }
    
        // Sort the directories by last modified time in descending order
        File latestFolder = folders[0];
        for (File folder : folders) {
            if (folder.lastModified() > latestFolder.lastModified()) {
                latestFolder = folder;
            }
        }
    
        return latestFolder;
    }
    
    private static void copyDirectory(File source, File destination) throws IOException {
        if (source.isDirectory()) {
            if (!destination.exists()) {
                destination.mkdir(); // Create the destination directory if it doesn't exist
            }
    
            // Get all files in the source directory
            String[] files = source.list();
            if (files != null) {
                for (String file : files) {
                    copyDirectory(new File(source, file), new File(destination, file)); // Recursively copy files
                }
            }
        } else {
            // If the source is a file, copy it
            Files.copy(source.toPath(), destination.toPath(), StandardCopyOption.REPLACE_EXISTING);
        }
    }
}
