import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.awt.AlphaComposite;
import java.awt.Font;
import java.awt.RenderingHints;
import java.awt.geom.Rectangle2D;
import java.awt.FontMetrics;

public class GetWidths {
public static void main(String[] args) {
    BufferedImage bi = new BufferedImage(100, 100, 
                                        BufferedImage.TYPE_4BYTE_ABGR);
    Graphics2D g2d = bi.createGraphics();
    Font font = new Font("Curier", Font.PLAIN, 34);
    g2d.setFont(font);
    FontMetrics fm = g2d.getFontMetrics();

    System.out.println("h=" + fm.getHeight());
    for(int ch = 32; ch < 127; ++ch) {
        System.out.print("" + fm.charWidth(ch) + ", ");
    }
}
}