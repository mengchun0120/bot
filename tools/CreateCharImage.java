import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.awt.AlphaComposite;
import java.awt.Font;
import java.awt.RenderingHints;
import java.awt.geom.Rectangle2D;
import java.awt.FontMetrics;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

class CreateCharImage {

public static boolean createImage(Font font, int ch, int width, int height, int y) 
{
    BufferedImage bi = new BufferedImage(width, height, 
                                         BufferedImage.TYPE_4BYTE_ABGR);
    Graphics2D g2d = bi.createGraphics();
    String str = Character.toString((char)ch);

    g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
                         RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

    g2d.setFont(font);
    g2d.setColor(new Color(255, 255, 255, 255));
    g2d.drawString(str, 0, y);
    g2d.dispose();

    try {
        String fileName = "ascii_" + ch + ".png";
        File file = new File(fileName);
        ImageIO.write(bi, "png", file);
    } catch(IOException e) {
        System.err.println(e.getLocalizedMessage());
        return false;
    }

    return true;
}

public static void main(String[] args) {
    int widths[] = {
        11, 14, 16, 28, 22, 32, 27, 9, 13, 13, 17, 28, 11, 
        12, 11, 11, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 
        11, 11, 28, 28, 28, 18, 34, 23, 23, 24, 26, 21, 20, 
        26, 26, 10, 10, 22, 19, 29, 25, 27, 21, 27, 24, 22, 
        21, 25, 23, 34, 23, 21, 23, 13, 11, 13, 28, 17, 17, 
        21, 22, 19, 22, 21, 12, 22, 22, 9, 9, 20, 9, 33, 22, 
        21, 22, 22, 14, 18, 13, 22, 20, 28, 20, 20, 18, 22, 
        11, 22, 28
    };
    Font font = new Font("Curier", Font.PLAIN, 34);
    int widthIdx = 0, height = 40, y = 31;

    for(int ch = 32; ch < 127; ++ch, ++widthIdx) {
        createImage(font, ch, widths[widthIdx], height, y);
    }
}

}
