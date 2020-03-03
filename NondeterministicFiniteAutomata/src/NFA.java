import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

public class NFA {

    static String[] transitionArray;
    static Nodes initial;
    static Nodes finalNode;
    public static void main (String[] args) {
        Nodes temp;


        File file = new File("input.txt"); //the file
        Scanner scanner= null;
        try {
            scanner = new Scanner(file); //scan from file
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        String allnodes = scanner.nextLine();

        String[] arrOfNodes=allnodes.split(" ");
        for (int i=0;i<arrOfNodes.length;i++) {
            temp=new Nodes(arrOfNodes[i]);
            Nodes.allNodes.put(arrOfNodes[i],temp);
        }

        String allterminals = scanner.nextLine();

        Nodes.arrOfterminals=allterminals.split(" ");

        String string = scanner.nextLine();
        initial=Nodes.allNodes.get(string);
        //////////////////////////////////////


        string = scanner.nextLine();
        finalNode=Nodes.allNodes.get(string);
        ///////////////////////////////////////////////

        String transitions;

        while (scanner.hasNext()) {
            transitions= scanner.nextLine();
            transitionArray=transitions.split(" ");
            Nodes.assign(transitionArray[0],transitionArray[1],transitionArray[2]);

        }
        for (Nodes node:Nodes.allNodes.values()
             ) {
         //   System.out.println(node.name);
         //   System.out.println(node.nextstates);
        }
        Nodes.generateTable1(initial);


        ////////////////////////////////////////////////////////////////////
        int lines=20*Nodes.arrOfterminals.length;
        for(int i=0;i<lines;i++)
        System.out.print("-");
        System.out.println();
        System.out.format("%10s","State:");
        for(String term:Nodes.arrOfterminals){
            System.out.format("%15s",term);
        }
        System.out.println();
        for(int i=0;i<lines;i++)
            System.out.print("-");
        System.out.println();
        for(Cell row:Nodes.rows){
            row.print();
        }
        for(int i=0;i<lines;i++)
            System.out.print("-");
        System.out.println();
    }
    //Nodes q0=new Nodes();
}
