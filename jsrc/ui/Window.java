package ui;
import javax.swing.*;

public class Window{

    public static void createWindow(){
        JFrame frame = new JFrame("Graph Partitioning");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Panel.setupPanel(frame);
        frame.setSize(900, 900);
        frame.setLocationRelativeTo(null);
        
        //Display the window.
        
        frame.setVisible(true);
        
    }


    
}