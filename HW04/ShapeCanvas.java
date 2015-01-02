import java.awt.*;
import java.awt.event.*;
/**
 * Write a description of class Canvas here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class ShapeCanvas extends Canvas
{
    Bullet[] bul = new Bullet[1000];
    Fighter fight;
    int count;
    public ShapeCanvas(int Scr_width, int Scr_height)
    {
        fight = new Fighter( Scr_width/2, Scr_height*3/4, Scr_width-50, Scr_height);
        count = 0;//bullet count
        Color backG = new Color( 255, 255, 255 );
        setBackground( backG );
    }

    public void paint( Graphics g )
    {
        fight.draw(g);
        for(int i = 0; i<count; i++)
        {
            bul[i].draw(g);
        }
        g.fillOval( 0, 0, 20, 20 );
    }
    
    public void addBul( Bullet bullet )
    {
        bul[count++] = bullet;
    }
    
    public Bullet[] getBul()
    {
        return bul;
    }
    
    public Fighter getFight()
    {
        return fight;
    }
    
    public void moveBul()
    {
        for(int i = 0; i<count; i++)
        {
            bul[i].move();
        }
    }
    
    public int checkCollision()
    {
        int num = 0;
        for(int i = 0; i<count; i++)
        {
            System.out.println(Math.sqrt( Math.pow( Math.abs(bul[0].x-(fight.x+fight.width/2)), 2) + Math.pow( Math.abs(bul[0].y-(fight.y+fight.height/2)), 2) ) );
            System.out.println((bul[i].radius + fight.width ));
            if( Math.sqrt( Math.pow( Math.abs( (bul[i].x+(bul[i].radius/2))-(fight.x+fight.width/2)), 2) + Math.pow( Math.abs( ( bul[i].y + (bul[i].radius/2) )-(fight.y+fight.height/2)), 2) )< (bul[i].radius + fight.width )/2 )
            {
                
                num++;
            }
        }
        return num;
    }
}
