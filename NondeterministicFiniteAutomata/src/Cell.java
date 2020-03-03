import java.util.*;

public class Cell {
    HashSet<Nodes> initial;
   static HashMap<String,Nodes> allNodesConverted;
    HashSet<String> initconverted;
    List<HashSet<String>> nodesconverted;
    List<HashSet<Nodes>> nodeSet;
     void print(){

         initconverted=new HashSet<>();
         nodesconverted=new LinkedList<>();
         for(Nodes nod:initial)
         {
             initconverted.add(nod.name);
         }
         System.out.format("|%10s|",initconverted);
         for(HashSet<Nodes> set:nodeSet){
             HashSet<String> tempset=new HashSet<>();
             for(Nodes nod:set){
                    tempset.add(nod.name);
             }
                nodesconverted.add(tempset);
         }

        for(HashSet<String> set:nodesconverted){
            System.out.format("\t|%10s|",set);
        }
         System.out.println();
     }
}
