import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
import java.util.*;
import java.io.*;
/**
 * Write a description of class GameGui here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class GameGui extends JFrame
{
    ShapeCanvas sc;
    int num = 0;
    JLabel score;
    Fighter fight;
    Bullet[] bul;
    boolean start = false;
    public GameGui(int Scr_width, int Scr_height)
    {
        setLayout( new BorderLayout() );
        sc = new ShapeCanvas( Scr_width, Scr_height);
        score = new JLabel("Score: "+ num);
        add( sc, "Center");
        add( score, "East");
        fight = sc.getFight();
        bul = sc.getBul();
        sc.addKeyListener(new KeyListener()
        {
            int key;
            int vert;
            int hori;
            public void keyPressed(KeyEvent e) 
            {
                key = e.getKeyCode();
                vert = 0;
                hori = 0;
                start = true;
                if (key == KeyEvent.VK_LEFT) 
                {
                    System.out.println("sigh");
                    hori = -1;
                }
                else
                if (key == KeyEvent.VK_RIGHT) 
                {
                    System.out.println("sigh2");
                    hori = 1;
                }

                if (key == KeyEvent.VK_UP) 
                {
                    System.out.println("sigh3");
                    vert = -1;
                }
                else
                if (key == KeyEvent.VK_DOWN) 
                {
                    System.out.println("sigh4");
                    vert = 1;
                }
                fight.move(hori, vert);
                sc.repaint();
            }
            
            public void keyTyped(KeyEvent e)
            {
                key = e.getKeyCode();
                if (key == KeyEvent.VK_LEFT) 
                {
                    System.out.println("sigh");
                    fight.move(-1, 0);
                }
                else
                if (key == KeyEvent.VK_RIGHT) 
                {
                    System.out.println("sigh2");
                    fight.move(1, 0);
                }

                if (key == KeyEvent.VK_UP) 
                {
                    System.out.println("sigh3");
                    fight.move(0, -1);
                }
                else
                if (key == KeyEvent.VK_DOWN) 
                {
                    System.out.println("sigh4");
                    fight.move(0, 1);
                }
            }
            
            public void keyReleased(KeyEvent e) 
            {
                
            }
            
        });
    }
    
    public void addBul(Bullet bullet)
    {
        sc.addBul(bullet);
        sc.repaint();
    }
    
    public void moveBul()
    {
        sc.moveBul();
        sc.repaint();
    }
    
    public void updateScore()
    {
        num += sc.checkCollision();
        
        score.setText("Score:" + num);
    }
}
