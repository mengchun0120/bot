import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.awt.RenderingHints;
import javax.imageio.ImageIO;

class CreateTransientCircle {

public static void createCircle(int radius, int red, int green, int blue, String file)
{
	BufferedImage bi = new BufferedImage(radius * 2, radius * 2, BufferedImage.TYPE_4BYTE_ABGR);
	Graphics2D g2d = bi.createGraphics();
    g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                         RenderingHints.VALUE_ANTIALIAS_ON);

	int w = 2 * radius, h = 2 * radius;
	float alpha = 0;
    int x = 0, y = 0;
	float deltaAlpha = 255.0f / (float)radius;
	Color color = null;
	
	for (int i = 0; i < radius; ++i)
	{
		alpha += deltaAlpha;
		color = new Color(red, green, blue, (int)alpha);
		g2d.setColor(color);
		g2d.fillOval(x, y, w, h);
        ++x;
        ++y;
		w -= 2;
		h -= 2;
	}

	color = new Color(red, green, blue, 255);
	g2d.setColor(color);
	g2d.fillOval(x, y, w, h);

	g2d.dispose();

    try 
    {
        File f = new File(file);
        ImageIO.write(bi, "png", f);
    } 
    catch(IOException e) 
    {
        System.err.println(e.getLocalizedMessage());
    }
}

public static void main(String[] args) 
{
    if(args.length != 3) 
    {
        System.err.println("java CreateTransientCircle radius color outputFile");
        System.exit(1);
    }

    int radius = Integer.parseInt(args[0]);

    String[] parts = args[1].split(":");
    if(parts.length != 3) 
    {
        throw new RuntimeException("Invaid color format");
    }

    int r = Integer.parseInt(parts[0]);
    if(r < 0 || r > 255) 
    {
        throw new RuntimeException("Invalid red value");
    }

    int g = Integer.parseInt(parts[1]);
    if(g < 0 || g > 255) 
    {
        throw new RuntimeException("Invalid green value");
    }

    int b = Integer.parseInt(parts[2]);
    if(b < 0 || b > 255)
    {
        throw new RuntimeException("Invaid blue value");
    }

    createCircle(radius, r, g, b, args[2]);
}

}