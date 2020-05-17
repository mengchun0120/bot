import java.io.PrintWriter;
import java.io.IOException;

class GenerateParticleEffect {
    public static void showUsageAndExit()
    {
        System.err.println("Usage: java GenerateParticleEffect --numParticles NUM --speed NUM " +
                           "--acceleration NUM --output file");
        System.exit(1);
    }

    public static void generateEffect(int numParticles, float speed, float acceleration, String outputFile)
    {
        final int NUM_FLOATS_PER_PARTICLE = 2;
        final float DELTA_THETA = (float)(Math.PI * 2.0) / numParticles;
        final int NUM_OF_FLOATS = NUM_FLOATS_PER_PARTICLE * numParticles;
        float[] particles = new float[NUM_OF_FLOATS];
        int i = 0;
        float theta = 0.0f;

        for (int j = 0; j < numParticles; ++j)
        {
            float directionX = (float)Math.cos(theta);
            float directionY = (float)Math.sin(theta);

            particles[i++] = directionX;
            particles[i++] = directionY;

            theta += DELTA_THETA;
        }

        PrintWriter writer = null;
        float duration = speed / acceleration;
        float coverBreathX = speed * speed / (2.0f * acceleration);
        float coverBreathY = coverBreathX;

        try
        {
            writer = new PrintWriter(outputFile);

            writer.printf("\"coverBreathX\": %f,\n", coverBreathX);
            writer.printf("\"coverBreathY\": %f,\n", coverBreathY);
            writer.printf("\"acceleration\": %f,\n", acceleration);
            writer.printf("\"duration\": %f,\n", duration);
            writer.printf("\"initSpeed\": %f,\n", speed);
            writer.printf("\"particles\": [\n");

            i = 0;
            for (int p = 0; p < numParticles; ++p)
            {
                if (p < numParticles - 1)
                {
                    writer.printf("    %f, %f,\n", particles[i], particles[i + 1]);
                    i += NUM_FLOATS_PER_PARTICLE;
                }
                else
                {
                    writer.printf("    %f, %f\n", particles[i], particles[i + 1]);
                }
            }

            writer.printf("]\n");
        }
        catch (IOException e)
        {
            System.err.println("Something happened while writing to file");
        }
        finally
        {
            if (writer != null)
            {
                writer.close();            
            }
        }
    }

    public static void main(String[] args)
    {
        int numParticles = 0;
        float speed = 0.0f, acceleration = 0.0f;
        String outputFile = null, color = null;

        int i = 0;
        while (i < args.length)
        {
            if (args[i].equals("--numParticles"))
            {
                if (i + 1 >= args.length)
                {
                    showUsageAndExit();
                }

                numParticles = Integer.parseInt(args[i + 1]);
                i += 2;
            }
            else if (args[i].equals("--speed"))
            {
                if (i + 1 >= args.length)
                {
                    showUsageAndExit();
                }

                speed = Float.parseFloat(args[i + 1]);
                i += 2;
            }
            else if (args[i].equals("--acceleration"))
            {
                if (i + 1 >= args.length)
                {
                    showUsageAndExit();
                }

                acceleration = Float.parseFloat(args[i + 1]);
                i += 2;
            }
            else if (args[i].equals("--output"))
            {
                if (i + 1 >= args.length)
                {
                    showUsageAndExit();
                }

                outputFile = args[i + 1];
                i += 2;
            }
            else
            {
                System.err.println("Invalid option " + args[i]);
                showUsageAndExit();
            }
        }

        if (numParticles <= 0)
        {
            System.err.println("Num of particles must be positive");
            System.exit(1);
        }

        if (speed <= 0.0f)
        {
            System.err.println("speed must be positive");
            System.exit(1);
        }

        if (acceleration <= 0.0f)
        {
            System.err.println("acceleration must be positive");
            System.exit(1);
        }

        if (outputFile == "")
        {
            System.err.println("output file must not be empty");
            System.exit(1);
        }

        generateEffect(numParticles, speed, acceleration, outputFile);
    }
}