import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;


public class Main {
  /*  public static String string="AbAbA";
    static HashSet<String> set = new HashSet<>();
    public static void backtrack(String str, HashSet<String> set, char ch,int n){
        if (n==str.length()) return;
        for (int i=0;i<str.length();i++){
            if (str.charAt(i)==ch){
                set.add(str);
                String str2 = str.substring(0,i)+str.substring(i+1);
                backtrack(str2,set,ch,i);
                str2 = str.substring(0,i)+str.substring(i);
                backtrack(str2,set,ch,i);
            }
        }
    }
    public static void main (String[] args) {
        backtrack(string,set,'B',0);
        for (String s : set) {
            System.out.println(s);
        }
    }*/


    // Non-Terminal elements
    public static String[] Vn;
    public static String[] Vt;
    public static HashMap<String, HashSet<String>> productions=new HashMap<String, HashSet<String>>();
    static String  nextline;
    static Scanner scanner;
    public static void main (String[] args) {
        try {
            File file = new File("src/input2.txt");
            scanner = new Scanner(file);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        //reading nonterminals
        nextline = scanner.nextLine();
        Vn=nextline.split(" ");


        //reading terminals
        nextline = scanner.nextLine();
        Vt=nextline.split(" ");
        while (scanner.hasNextLine()){


        nextline=scanner.nextLine();
        String[] strarr=nextline.split(" ");
            HashSet<String> set= productions.get(strarr[0]);
            if(set==null){
                set=new HashSet<String>();
                productions.put(strarr[0],set);
            }
            for (int i = 1;i<strarr.length;i++) {
                set.add(strarr[i]);
            }

        }
        System.out.println(productions);

        EpsilonRemovment eps = new EpsilonRemovment(Vn,Vt,productions);

    }

}
