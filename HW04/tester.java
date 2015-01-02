import java.awt.event.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import javax.imageio.ImageIO;
import java.io.IOException;
/**
 * Write a description of class tester here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class tester
{
    public static void main(String args[])
    {
        int[] life = new int[1000];
        life[0] = 5;
        life[1] = 2;
        System.out.println(life.length);
        BufferedImage img = null;
        int width, height;
        width = 0;
        height = 0;
        try 
        {
            img = ImageIO.read(new File("fighter.bmp"));
        } catch (IOException e) 
        {
            
        }
        if(img != null)
        {
            width = img.getWidth();
            height = img.getHeight();
        }
        System.out.println( "width: " + width );
        System.out.println( "height: " + height );
    }
}
