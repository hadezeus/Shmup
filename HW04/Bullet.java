import java.awt.event.*;
import java.awt.*;
/**
 * Write a description of class Bullet here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class Bullet 
{
    int x, y, speed, radius;
    double angle;
    Color c1;
    public Bullet(int x, int y, double angle)
    {
        this.x = x;
        this.y = y;
        speed = 5;
        this.angle = angle;
        radius = 40;
        c1 = new Color( 255, 0, 0 );
    }

    public void move()
    {
        //if(      if the bullet is outside stage stop moving/drawing
        x = x +(int) (speed * Math.cos( angle ) );
        y = y +(int) (speed * Math.sin( angle ) );
    }
    
    public void draw( Graphics g )
    {
        //if(
        g.setColor( c1 );
        g.fillOval( x, y, radius, radius );
    }
}
