import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;


public class MapReducePatternMatching {

    public static void main(String[] args) throws IOException {
        if (args.length != 2) {
            System.out.println("BruteForceStringMatch <file name> <pattern>");
            System.exit(1);
        }

        String fileString = new String(Files.readAllBytes(Paths.get(args[0])));//, StandardCharsets.UTF_8);
        char[] text = new char[fileString.length()];
        int n = fileString.length();
        for (int i = 0; i < n; i++) {
            text[i] = fileString.charAt(i);
        }

        String patternString = args[1];
        char[] pattern = new char[patternString.length()];
        int m = patternString.length();
        for (int i = 0; i < m; i++) {
            pattern[i] = patternString.charAt(i);
        }

        int matchLength = n - m;
        char[] match = new char[matchLength+1];
        for (int i = 0; i <= matchLength; i++) {
            match[i] = '0';
        }

        // get current time
        long start = System.currentTimeMillis();
        int tCount = Runtime.getRuntime().availableProcessors();
        int StringBlockSize = matchLength / tCount;
        MatchMapThread[] threadPool = new MatchMapThread[tCount];
        for(int i = 0; i < tCount; i++){
            int from = StringBlockSize * i;
            int to = StringBlockSize * (i + 1);
            threadPool[i] = new MatchMapThread(from, to, text, pattern, match);
            threadPool[i].start();
        }
        for(int i = 0; i < tCount; i++){
            try {
                threadPool[i].join();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }
        int matchCount = 0;
        // get current time and calculate elapsed time
        long elapsedTimeMillis = System.currentTimeMillis()-start;
        System.out.println("time in ms = "+ elapsedTimeMillis);

        for (int i = 0; i < matchLength; i++) {
            if (match[i] == '1') System.out.print(i+" ");
            matchCount++;
        }
        System.out.println();
        System.out.println("Total matches " + matchCount);
    }

    static class MatchMapThread extends Thread{
        private int from;
        private int to;
        private int matchCount;
        private char[] text;
        private char[] pattern;
        private char[] match;
        private ArrayList<Character> finds;
        public MatchMapThread(int from, int to, char[] text, char[] pattern, char[] match) {
            this.from = from;
            this.to = to;
            this.text = text;
            this.pattern = pattern;
            this.match = match;
            matchCount = 0;
            finds = new ArrayList<>();
        }

        public void run() {
            int m = pattern.length;
            for (int j = from; j < to; j++) {
                if (j + m > text.length) break;
                int i;
                for (i = 0; i < m && pattern[i] == text[i + j]; i++);
                if (i >= m) {
                    match[j] = '1';
                }
            }
        }
    }
}


