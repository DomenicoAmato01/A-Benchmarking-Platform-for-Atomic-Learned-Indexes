package paco;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;

import java.util.HashMap;
import java.util.Map;

/**
 * Hello world!
 *
 */
public class PACO {

    /**
     * 
     * @param csvFile File to read
     * @return 
     */
    private static ArrayList<Long> readIntCsv(String csvFile) {

        // String csvFile = "/Users/mkyong/csv/country.csv";
        BufferedReader br = null;
        String line = "";
        ArrayList<Long> dataset = new ArrayList<Long>();
        try {

            br = new BufferedReader(new FileReader(csvFile));
            while ((line = br.readLine()) != null) {

                Long number = Long.parseLong(line);
                dataset.add(number);
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (br != null) {
                try {
                    br.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return dataset;

    }

    private static void saveToCSV(String filename, StringBuilder predTime)
            throws IOException {
        File file;
        PrintWriter pw;
        file = new File(filename);
        pw = new PrintWriter(new FileWriter(file, true));
        if (file.canWrite()) {
            pw.write(predTime.toString());
        }
        pw.close();
    }

    public static void main(String[] args) throws Exception {
        int args_len = args.length;
        Map<String, String> params = new HashMap<String, String>();
        ArrayList<Long> dataset;
        PacoDataStructure pacoData;
      
        long hashStart;
        long hashFinish;
        long hashElapsedTime;
        long predStart;
        long predFinish;
        double predElapsedTime;
        StringBuilder hashTimeString = new StringBuilder();
        StringBuilder predTimeString = new StringBuilder();
        String method = new String("No Method");
        ArrayList<Long> result = new ArrayList<Long>();
        int correct = 0;

        /**
        main -f filename -m method [-q queryFile]
         * 
         * -f: name of the input file (required) 
         * -m: the hashing method to use: "lcp" (default) or "paco" (optional) 
         * -q: the query file (optional)
         */
        if (args_len > 0) {
            for (int i = 0; i < args_len; i++) {

                String param = args[i];
                if (param.equals("-f")) {
                    params.put("file", args[i + 1]);
                } else if (param.equals("-q")) {
                    params.put("query", args[i + 1]);
                } 

            }

            System.out.println("File: " + params.get("file"));

            // Check if params are provided
            if (!(params.containsKey("file")) || !(new File(params.get("file")).exists())) {
                throw new Exception("Not enoght input provided: usage main -f filename [-q queryFile]");
            }

            /*
             * Set Header for Hashing Csv
             */
            hashTimeString.append("Space (byte)");
            hashTimeString.append(",");
            hashTimeString.append("Costruction Time (s)");
            hashTimeString.append(",");
            hashTimeString.append("Costruction Time (min)");
            hashTimeString.append(",");
            hashTimeString.append("Costruction Time (h)");
            hashTimeString.append("\n");

            String fname = params.get("file");
            File inFile = new File(params.get("file"));
            //Legge il file
            dataset = readIntCsv(fname);

            /*
             * Set Header for Prediction Csv 
             */
            predTimeString = new StringBuilder();
            predTimeString.append("Mean Prediction Time (s)");
            predTimeString.append(",");
            predTimeString.append("Mean Prediction Time (min)");
            predTimeString.append(",");
            predTimeString.append("Mean Prediction Time (h)");
            predTimeString.append(",");
            predTimeString.append("Size");
            predTimeString.append(",");
            predTimeString.append("Correct");
            predTimeString.append(",");
            predTimeString.append("Wrong");
            predTimeString.append("\n");

            /*
             * Start Hashing
             */

          
              
                hashStart = System.nanoTime();
                pacoData = new PacoDataStructure(dataset);
                hashFinish = System.nanoTime();
                hashElapsedTime = hashFinish - hashStart;

                hashTimeString.append(pacoData.getBitDimension() / 8.0);
                hashTimeString.append(",");
                hashTimeString.append(Double.toString(hashElapsedTime / 1000000000.0));
                hashTimeString.append(",");
                hashTimeString.append(Double.toString(hashElapsedTime / 1000000000.0 / 60.0));
                hashTimeString.append(",");
                hashTimeString.append(Double.toString(hashElapsedTime / 1000000000.0 / 3600.0));
                hashTimeString.append("\n");
                String outFileName = "Hashing" + method + inFile.getName();
                saveToCSV(outFileName, hashTimeString);

                System.out.println("Tabella Hash Creata con metodo " + method);
                if (params.containsKey("query")) {
                    File qFile = new File(params.get("query"));
                    System.out.println("Elaborazione query " + qFile.getName() + " per " + fname);
                    ArrayList<Long> queryDataset;
                    queryDataset = readIntCsv(params.get("query"));
                    System.out.println("Query dataset size:" + queryDataset.size());
                    correct = 0;
                    predElapsedTime = 0.0;
                    for (long q : queryDataset) {
                        predStart = System.nanoTime();
                        long predecessorIndex = pacoData.getPredecessorIndex(q);
                        if (predecessorIndex >= 0 && dataset.get((int) predecessorIndex) == q) {
                            correct++;
                        }
                        predFinish = System.nanoTime();
                        predElapsedTime = predElapsedTime + (predFinish - predStart);
                    }

                    predElapsedTime = predElapsedTime / queryDataset.size();
                    double wrong = queryDataset.size() - (double) correct;
                    //System.out.println("Error Query: " + wrong);

                    predTimeString.append(Double.toString(predElapsedTime / 1000000000.0));
                    predTimeString.append(",");
                    predTimeString.append(Double.toString(predElapsedTime / 1000000000.0 / 60));
                    predTimeString.append(",");
                    predTimeString.append(Double.toString(predElapsedTime / 1000000000.0 / 3600));
                    predTimeString.append(",");
                    predTimeString.append(String.valueOf(queryDataset.size()));
                    predTimeString.append(",");
                    predTimeString.append(String.valueOf(correct));
                    predTimeString.append(",");
                    predTimeString.append(String.valueOf(wrong));
                    predTimeString.append("\n");
                    String queryFileName = "Query" + qFile.getName().replace(".", "") + method + inFile.getName();
                    saveToCSV(queryFileName, predTimeString);
                }
            }
        System.out.println(
                "Esecuzione Terminata");
    }
}
