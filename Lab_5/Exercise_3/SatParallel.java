/*  Vector Size    1 Thread     2 Threads     4 Threads     8 Threads
 *  23             606ms        396ms         235ms         190ms
 *  24             1177ms       802ms         450ms         447ms
 *  25             2436ms       1387ms        952ms         805ms
 *  26             4834ms       2565ms        1824ms        1330ms
 */

import java.lang.Math;
import java.util.*;

public class SatParallel {

    public static void main(String[] args) {
        int size;

        if (args.length != 2) {
            System.out.println("Usage: java Main <vector size> <number of threads>");
            System.exit(1);
        }

        size = Integer.parseInt(args[0]);
        int numThreads = Integer.parseInt(args[1]);

        if (size <= 0 || numThreads <= 0) {
            System.out.println("Both size and number of threads must be positive.");
            System.exit(1);
        }

        int iterations = (int) Math.pow(2, size);
        int blockSize = iterations / numThreads;

        List<String> finalOutput = Collections.synchronizedList(new LinkedList<>());

        Thread[] threads = new Thread[numThreads];

        long start = System.currentTimeMillis();

        for (int i = 0; i < numThreads; i++) {
            int from = i * blockSize;
            int to = (i == numThreads - 1) ? iterations : from + blockSize;

            threads[i] = new Thread(new Worker(from, to, size, finalOutput));
            threads[i].start();
        }

        for (int i = 0; i < numThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }

        long elapsedTimeMillis = System.currentTimeMillis() - start;

        System.out.println(finalOutput);
        System.out.println("All done");
        System.out.println("Time in ms = " + elapsedTimeMillis);
    }

    static class Worker implements Runnable {
        private final int from;
        private final int to;
        private final int size;
        private final List<String> sharedOutput;

        public Worker(int from, int to, int size, List<String> sharedOutput) {
            this.from = from;
            this.to = to;
            this.size = size;
            this.sharedOutput = sharedOutput;
        }

        @Override
        public void run() {
            for (int i = from; i < to; i++) {
                check_circuit(i, size, sharedOutput);
            }
        }
    }

    static void check_circuit(int z, int size, List<String> output) {
        boolean[] v = new boolean[size];

        for (int i = size - 1; i >= 0; i--)
            v[i] = (z & (1 << i)) != 0;

        boolean value =
                (v[0] || v[1])
                        && (!v[1] || !v[3])
                        && (v[2] || v[3])
                        && (!v[3] || !v[4])
                        && (v[4] || !v[5])
                        && (v[5] || !v[6])
                        && (v[5] || v[6])
                        && (v[6] || !v[15])
                        && (v[7] || !v[8])
                        && (!v[7] || !v[13])
                        && (v[8] || v[9])
                        && (v[8] || !v[9])
                        && (!v[9] || !v[10])
                        && (v[9] || v[11])
                        && (v[10] || v[11])
                        && (v[12] || v[13])
                        && (v[13] || !v[14])
                        && (v[14] || v[15])
                        && (v[14] || v[16])
                        && (v[17] || v[1])
                        && (v[18] || !v[0])
                        && (v[19] || v[1])
                        && (v[19] || !v[18])
                        && (!v[19] || !v[9])
                        && (v[0] || v[17])
                        && (!v[1] || v[20])
                        && (!v[21] || v[20])
                        && (!v[22] || v[20])
                        && (!v[21] || !v[20])
                        && (v[22] || !v[20]);

        if (value) {
            output.add(formatResult(v, size, z));
        }
    }

    static String formatResult(boolean[] v, int size, int z) {
        StringBuilder result = new StringBuilder();
        result.append(z);
        for (int i = 0; i < size; i++) {
            result.append(v[i] ? " 1" : " 0");
        }
        result.append("\n");
        return result.toString();
    }
}
