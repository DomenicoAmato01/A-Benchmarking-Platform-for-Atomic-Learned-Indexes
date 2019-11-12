package SABT;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.PrintWriter;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Stack;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Use of a top-down splay tree implemented by http://www.link.cs.cmu.edu/splay/
 * Author: Danny Sleator <sleator@cs.cmu.edu>
 * This code is in the public domain.
 */
class BinaryNode implements Serializable {

    BinaryNode(Comparable theKey) {
        key = theKey;
        left = right = null;
    }
    public Comparable key;          // The data in the node
    public BinaryNode left;         // Left child
    public BinaryNode right;        // Right child
}

public class SplayTree implements Serializable {

    private BinaryNode root;

    public SplayTree() {
        root = null;
    }

    public int nodesNumber() {
        Stack<BinaryNode> sizeStack = new Stack();
        int count = 1;//includes the n node
        if (root == null) {
            return 0;
        }
        sizeStack.push(root);
        while (!sizeStack.isEmpty()) {
            BinaryNode node = sizeStack.pop();
            while (node != null) {
                count++;
                if (node.right != null) {
                    sizeStack.push(node.right);
                }
                node = node.left;
            }
        }
        return count;
    }

    /**
     * Insert into the tree.
     * @param x the item to insert.
     * @throws DuplicateItemException if x is already present.
     */
    public void insert(Comparable key) {
        BinaryNode n;
        int c;
        if (root == null) {
            root = new BinaryNode(key);
            return;
        }
        splay(key);
        if ((c = key.compareTo(root.key)) == 0) {
            //	    throw new DuplicateItemException(x.toString());	    
            return;
        }
        n = new BinaryNode(key);
        if (c < 0) {
            n.left = root.left;
            n.right = root;
            root.left = null;
        } else {
            n.right = root.right;
            n.left = root;
            root.right = null;
        }
        root = n;
    }

    /**
     * Remove from the tree.
     * @param x the item to remove.
     * @throws ItemNotFoundException if x is not found.
     */
    public void remove(Comparable key) {
        BinaryNode x;
        splay(key);
        if (key.compareTo(root.key) != 0) {
            //            throw new ItemNotFoundException(x.toString());
            return;
        }
        // Now delete the root
        if (root.left == null) {
            root = root.right;
        } else {
            x = root.right;
            root = root.left;
            splay(key);
            root.right = x;
        }
    }

    /**
     * Find the smallest item in the tree.
     */
    public Comparable findMin() {
        BinaryNode x = root;
        if (root == null) {
            return null;
        }
        while (x.left != null) {
            x = x.left;
        }
        splay(x.key);
        return x.key;
    }

    /**
     * Find the largest item in the tree.
     */
    public Comparable findMax() {
        BinaryNode x = root;
        if (root == null) {
            return null;
        }
        while (x.right != null) {
            x = x.right;
        }
        splay(x.key);
        return x.key;
    }

    /**
     * Find an item in the tree.
     */
    public Comparable find(Comparable key) {
        if (root == null) {
            return null;
        }
        splay(key);
        if (root.key.compareTo(key) != 0) {
            return null;
        }
        return root.key;
    }

    /**
     * Test if the tree is logically empty.
     * @return true if empty, false otherwise.
     */
    public boolean isEmpty() {
        return root == null;
    }

    /** this method just illustrates the top-down method of
     * implementing the move-to-root operation 
     */
    private void moveToRoot(Comparable key) {
        BinaryNode l, r, t, y;
        l = r = header;
        t = root;
        header.left = header.right = null;
        for (;;) {
            if (key.compareTo(t.key) < 0) {
                if (t.left == null) {
                    break;
                }
                r.left = t;                                 /* link right */
                r = t;
                t = t.left;
            } else if (key.compareTo(t.key) > 0) {
                if (t.right == null) {
                    break;
                }
                l.right = t;                                /* link left */
                l = t;
                t = t.right;
            } else {
                break;
            }
        }
        l.right = t.left;                                   /* assemble */
        r.left = t.right;
        t.left = header.right;
        t.right = header.left;
        root = t;
    }
    private static BinaryNode header = new BinaryNode(null); // For splay

    /**
     * Internal method to perform a top-down splay.
     * 
     *   splay(key) does the splay operation on the given key.
     *   If key is in the tree, then the BinaryNode containing
     *   that key becomes the root.  If key is not in the tree,
     *   then after the splay, key.root is either the greatest key
     *   < key in the tree, or the lest key > key in the tree.
     *
     *   This means, among other things, that if you splay with
     *   a key that's larger than any in the tree, the rightmost
     *   node of the tree becomes the root.  This property is used
     *   in the delete() method.
     */
    private void splay(Comparable key) {
        BinaryNode l, r, t, y;
        l = r = header;
        t = root;
        header.left = header.right = null;
        for (;;) {
            if (key.compareTo(t.key) < 0) {
                if (t.left == null) {
                    break;
                }
                if (key.compareTo(t.left.key) < 0) {
                    y = t.left;                            /* rotate right */
                    t.left = y.right;
                    y.right = t;
                    t = y;
                    if (t.left == null) {
                        break;
                    }
                }
                r.left = t;                                 /* link right */
                r = t;
                t = t.left;
            } else if (key.compareTo(t.key) > 0) {
                if (t.right == null) {
                    break;
                }
                if (key.compareTo(t.right.key) > 0) {
                    y = t.right;                            /* rotate left */
                    t.right = y.left;
                    y.left = t;
                    t = y;
                    if (t.right == null) {
                        break;
                    }
                }
                l.right = t;                                /* link left */
                l = t;
                t = t.right;
            } else {
                break;
            }
        }
        l.right = t.left;                                   /* assemble */
        r.left = t.right;
        t.left = header.right;
        t.right = header.left;
        root = t;
    }
    // test code stolen from Weiss
    static ArrayList<Long> dataset;

    public static void main(String[] args) {


        if (args.length == 0) {
            System.out.println("Usage: program filename queryFile");
            System.exit(1);
        }

        //Load the dataset
        ArrayList<Long> readIntCsv = readIntCsv(args[0]);
        Collections.shuffle(readIntCsv);
        System.out.println("Checking... (no bad output means success)");
        //long datasetSize = readIntCsv.size() * Long.BYTES;
        long predStart = System.nanoTime();
        SplayTree t = new SplayTree();
        for (long v : readIntCsv) {
            t.insert(v);
        }
        long predFinish = System.nanoTime();
        long ElapsedTime = predFinish - predStart;
        double v = (double) ElapsedTime / 1000000000;
        System.out.println("Inserts complete seconds " + v);



        //calculate object size
        // long deepSizeOf = SizeOf.deepSizeOf(t.root);
        //System.out.println("Tree size=" + deepSizeOf);
        long sizeDataset = readIntCsv.size() * 24;
        int nodesNumber = t.nodesNumber();
        System.out.println("#Elementi =" + readIntCsv.size());
        System.out.println("Datasetsize =" + sizeDataset);
        System.out.println("#nodi =" + nodesNumber);
        long dimension = nodesNumber * 48 - sizeDataset;
        System.out.println("SizeOf =" + dimension);

        if (args.length > 1) {
            File qFile = new File(args[1]);
            System.out.println("Elaborazione query " + qFile.getName() + " per " + args[0]);
            ArrayList<Long> queryDataset;
            queryDataset = readIntCsv(args[1]);
            System.out.println("Query dataset size:" + queryDataset.size());
            int correct = 0;
            double predElapsedTime = 0.0;
            for (int i = 0; i < queryDataset.size(); i++) {
                predStart = System.nanoTime();
                Comparable find = t.find(queryDataset.get(i));
                if (find != null && find.equals(queryDataset.get(i))) {
                    correct++;
                }
                predFinish = System.nanoTime();
                predElapsedTime = predElapsedTime + (predFinish - predStart);
            }
            predElapsedTime = predElapsedTime / queryDataset.size();
            double wrong = queryDataset.size() - (double) correct;
            //System.out.println("Error Query: " + wrong);
            StringBuilder predTimeString = new StringBuilder();
            predTimeString.append("Seconds");
            predTimeString.append(",");
            predTimeString.append("Minutes");
            predTimeString.append(",");
            predTimeString.append("Hours");
            predTimeString.append(",");
            predTimeString.append("Dataset size");
            predTimeString.append(",");
            predTimeString.append("#Correct");
            predTimeString.append(",");
            predTimeString.append("#Wrong");
            predTimeString.append("\n");
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
            System.out.println(predTimeString.toString());
        }

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

    public static int sizeOfNode() {
        long v = 0;
        BinaryNode r = new BinaryNode(v);
        ObjectOutputStream objectOutputStream = null;
        try {
            ByteArrayOutputStream byteOutputStream = new ByteArrayOutputStream();
            objectOutputStream = new ObjectOutputStream(byteOutputStream);
            objectOutputStream.writeObject(r);
            objectOutputStream.flush();
            objectOutputStream.close();
            return byteOutputStream.toByteArray().length;
        } catch (IOException ex) {
            Logger.getLogger(SplayTree.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            try {
                objectOutputStream.close();
            } catch (IOException ex) {
                Logger.getLogger(SplayTree.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        return 0;
    }

    public static int sizeOfNode(Object r) {
        ObjectOutputStream objectOutputStream = null;
        try {
            ByteArrayOutputStream byteOutputStream = new ByteArrayOutputStream();
            objectOutputStream = new ObjectOutputStream(byteOutputStream);
            objectOutputStream.writeObject(r);
            objectOutputStream.flush();
            objectOutputStream.close();
            return byteOutputStream.toByteArray().length;
        } catch (IOException ex) {
            Logger.getLogger(SplayTree.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            try {
                objectOutputStream.close();
            } catch (IOException ex) {
                Logger.getLogger(SplayTree.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        return 0;
    }
}
