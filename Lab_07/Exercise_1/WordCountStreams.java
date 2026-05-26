import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.*;
import java.util.stream.*;

public class WordCountStreams {
    public static void main(String[] args) throws Exception {
        FileInputStream fileInputStream = new FileInputStream("bible.txt");
        BufferedReader in = new BufferedReader(new InputStreamReader(fileInputStream));

        HashMap<Object, Integer> mapper = new HashMap<>();

        Stream<String> stream = in.lines();
        
        stream.filter(str -> str.length() > 10).flatMap(
                str -> Arrays.stream(str.split(" "))
        ).forEach(str -> {
            if(mapper.get(str) == null) {
                mapper.put(str, 1);
            } else {
                mapper.put(str, mapper.get(str) + 1);
            }
        });

        stream.close();
        in.close();
        fileInputStream.close();

        for(Object key : mapper.keySet()) {
            System.out.println(key + ":" + mapper.get(key));
        }
    }
}
