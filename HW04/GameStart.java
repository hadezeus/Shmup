/**
 * A class to show the Gui from the other class.
 * 
 * @author (Paolo Chua) 
 * @version (12/29/11)
 */
import javax.swing.*;
import java.util.Random;
public class GameStart extends Thread
{
    static GameGui f;
    public static void main( String args[] )
    {
        f = new GameGui(800, 650);
        f.setSize( 800,650 );
        f.setTitle( "Bullet Hell" );
        f.setResizable( false );
        f.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
        f.setVisible( true );
        (new GameStart()).start();
    }
    
    public void run()
    {
        Random ran = new Random();
        long startTime = System.currentTimeMillis();
        long currentTime = System.currentTimeMillis();
        System.out.println("start1");
        while( !f.start ) //if the players hasn't move then don't start timer and bullet generation
        {
            System.out.println("start2");
        }
        Bullet temp = new Bullet(400, 150, Math.PI*1/2);
        f.addBul(temp);
        while( startTime +(75*1000) > currentTime) //runs for 75 sec
        {
           //ran.nextInt(1);
           
           System.out.println("start3");
           try {
               Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
                // handle the exception...        
                // For example consider calling Thread.currentThread().interrupt(); here.
            }
           
           f.moveBul();
           f.updateScore();
           currentTime = System.currentTimeMillis();
        }
        System.out.println( f.num );
        try 
        {
            Thread.sleep(4000);
        } 
        catch (InterruptedException e) 
        {
            e.printStackTrace();
            // handle the exception...        
            // For example consider calling Thread.currentThread().interrupt(); here.
        }
        
        System.exit(0);
    }
    
    
}

