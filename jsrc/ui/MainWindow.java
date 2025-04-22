package ui;
import javax.swing.*;

public class MainWindow{

    public static void createWindow(){
        JFrame frame = new JFrame("Graph Partitioning");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        ControlPanel.setupControlPanel(frame);
        frame.setSize(640, 480);
        frame.setLocationRelativeTo(null);
        
        //Display the window.
        
        frame.setVisible(true);
        
    }


    
}