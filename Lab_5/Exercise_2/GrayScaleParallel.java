/*  Image          1 core  2 cores  4 cores  8 cores
 *  1house.jpg     356ms   233ms    194ms    148ms
 *  2aerial.jpg    893ms   571ms    926ms    389ms
 *  3tiger.jpg     2045ms  1457ms   1135ms   975ms
 *  4food.jpg      2044ms  1306ms   981ms    775ms
 *  5landscape.jpg 1570ms  936ms    895ms    641ms
 *  6berries.jpg   1825ms  1274ms   953ms    780ms
 *  7lake.jpg      2324ms  1532ms   1242ms   1019ms
 */

import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.awt.image.BufferedImage;
import java.util.Objects;
import javax.imageio.ImageIO;

public class GrayScaleParallel {
    public static void main(String[] args) {

        String fileNameR = null;
        String fileNameW = null;
        int ThreadCount;
        //Input and Output files using command line arguments
        if (args.length != 3) {
            System.out.println("Usage: java RGBtoGrayScale <file to read> <file to write> <threads>");
            System.exit(1);
        }
        fileNameR = args[0];
        fileNameW = args[1];
        try{
            ThreadCount = Integer.parseInt(args[2]);
        } catch (NumberFormatException e) {
            System.err.println("Argument 2 must be a number");
            throw new RuntimeException(e);
        }

        //The same without command line arguments
        // fileNameR = "original.jpg";
        // fileNameW = "new.jpg";

        //Reading Input file to an image
        BufferedImage img = null;
        try {
            img = ImageIO.read(new File(fileNameR));
        } catch (IOException e) {}

        //Start timing
        long start = System.currentTimeMillis();

        int Height = Objects.requireNonNull(img).getHeight();
        int block = Height / ThreadCount;

        ThreadGray[] threadPool = new ThreadGray[ThreadCount];
        for(int i = 0; i < ThreadCount; i++){
            int from = i * block;
            int to = Math.min(i * block + block, Height);
            threadPool[i] = new ThreadGray(from, to, img);
            threadPool[i].start();
        }

        for(int i = 0; i < ThreadCount; i++){
            try {
                threadPool[i].join();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }
        //Stop timing
        long elapsedTimeMillis = System.currentTimeMillis()-start;

        //Saving the modified image to Output file
        try {
            File file = new File(fileNameW);
            ImageIO.write(img, "jpg", file);
        } catch (IOException _) {}

        System.out.println("Done...");
        System.out.println("time in ms = "+ elapsedTimeMillis);
    }

    static class ThreadGray extends Thread{
        private final int fromY;
        private final int toY;
        private final BufferedImage img;

        public ThreadGray(int fromY, int toY, BufferedImage img) {
            this.fromY = fromY;
            this.toY = toY;
            this.img = img;
        }

        @Override
        public void run() {
            double redCoefficient = 0.299;
            double greenCoefficient = 0.587;
            double blueCoefficient = 0.114;

            for (int y = fromY; y < toY; y++) {
                for (int x = 0; x < img.getWidth(); x++) {
                    int pixel = img.getRGB(x, y);
                    Color color = new Color(pixel, true);
                    int gray = (int)(color.getRed() * redCoefficient +
                                    color.getGreen() * greenCoefficient +
                                    color.getBlue() * blueCoefficient
                    );
                    Color grayColor = new Color(gray, gray, gray);
                    img.setRGB(x, y, grayColor.getRGB());
                }
            }
        }
    }

}
