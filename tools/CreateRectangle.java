import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.awt.RenderingHints;
import javax.imageio.ImageIO;

class CreateRectangle {

public static void createRectangle(int width, int height, Color color, String file)
{
    BufferedImage bi = new BufferedImage(width, height,
                                         BufferedImage.TYPE_4BYTE_ABGR);
    Graphics2D g2d = bi.createGraphics();
    g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                         RenderingHints.VALUE_ANTIALIAS_ON);
    g2d.setColor(color);
    g2d.fillRect(0, 0, width, height);
    g2d.dispose();

    try {
        File f = new File(file);
        ImageIO.write(bi, "png", f);
    } catch(IOException e) {
        System.err.println(e.getLocalizedMessage());
    }
}

public static Color parseColor(String s)
{
    String[] parts = s.split(":");
    if(parts.length != 4) {
        throw new RuntimeException("Invaid color format");
    }

    int r = Integer.parseInt(parts[0]);
    if(r < 0 || r > 255) {
        throw new RuntimeException("Invalid red value");
    }

    int g = Integer.parseInt(parts[1]);
    if(g < 0 || g > 255) {
        throw new RuntimeException("Invalid green value");
    }

    int b = Integer.parseInt(parts[2]);
    if(b < 0 || b > 255) {
        throw new RuntimeException("Invaid blue value");
    }

    int a = Integer.parseInt(parts[3]);
    if(a < 0 || a > 255) {
        throw new RuntimeException("Invalid alpha value");
    }

    return new Color(r, g, b, a);
}

public static void main(String[] args) {
    if(args.length != 4) {
        System.err.println("java CreateRectangle width height color outputFile");
        System.exit(1);
    }

    int width = Integer.parseInt(args[0]);
    int height = Integer.parseInt(args[1]);
    Color color = parseColor(args[2]);
    createRectangle(width, height, color, args[3]);
}

}
